#include"renderer.h"
#include<cstdlib>
namespace LemonCube{

Renderer::Renderer(const unsigned int& width, const unsigned int& height, const Vec3f& background_color)
    :
    time_per_tick_(1.0 / cv::getTickFrequency()),
    is_clock_running_(false),
    is_showing_rendering(false),
    is_MSAA_open_(false),
    start_time_(0.0), end_time_(0.0), duration_(0.0),
    canvas_width_(width), canvas_height_(height),
    background_color_(background_color),
    frame_buffer_(nullptr),
    z_buffer_(nullptr),
    eye_pos_{0.0f,0.0f,0.0f},
    points_(),
    lines_(),
    triangles_(),
    is_render_verties_(false),
    is_render_edges_(false),
    is_render_normals_(false),
    is_render_models_(false),
    eye_fov_(0.0f),
    aspect_ratio_(canvas_width_ / canvas_height_),
    view_port_width_half_(0.0f),
    view_port_height_half_(0.0f),
    z_near_(-1.0f),
    z_far_(500.0f),
    model_matrix_(),
    view_matrix_(),
    projection_matrix_(),
    lights_(),
    shader_ptr_(make_shared<IShader>()),
    sample_rate_(1),
    hitable_list_(),
    view_port_cord_(){
    frame_buffer_ = new Vec3f[width * height];
    z_buffer_ = new float[width * height];
}

Renderer::~Renderer(){
    if(frame_buffer_ != nullptr){
        delete[] frame_buffer_;
        frame_buffer_ = nullptr;
    }
    if(z_buffer_ != nullptr){
        delete[] z_buffer_;
        z_buffer_ = nullptr;
    }
}

void Renderer::StartClock(){
    if(!is_clock_running_){
        is_clock_running_ = true;
        start_time_ = cv::getTickCount();            
    }
}

void Renderer::StopClock(){
    if(is_clock_running_){
        is_clock_running_ = false;
        end_time_ = cv::getTickCount();
        duration_ += end_time_ - start_time_;
    }
}

void Renderer::ClearTimeCounter(){
    if(is_clock_running_){
        StopClock();
    }
    duration_ = 0;
}

void Renderer::GetTimeCost(){
    if(is_clock_running_){
            StopClock();
            StartClock();
    }
    cout<<"time cost: --"<<duration_ * time_per_tick_ *1000 <<" ms  --" << duration_ * time_per_tick_<<" s\n";
}

void Renderer::ShowImage(std::string window_name, const unsigned short delay_ms){
    StopClock();
    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::namedWindow(window_name);
    imshow(window_name, image);
    cv::waitKey(delay_ms);
    StartClock();
}

void Renderer::SaveImage(const std::string& filename){
    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
    cv::cvtColor(image, image, CV_RGB2BGR);
    cv::imshow("Rendering", image);
    cv::waitKey(3);      
    cout<<"\nPush 's to save image or push 'q' to quit\n";
    char k = cin.get();
    if(k == 's'){
        if(imwrite(filename, image*255.0f)){
            cout<<"Save the image sucessfully\n";
        }
        else{
            cout<<"Save the image failed\n";
        }
    }
    else{
        cout<<"quit\n";
    }
}

void Renderer::ClearCanvas(){
    unsigned int z_buffer_size = canvas_width_ * canvas_height_;    
    for(int i = 0;i < z_buffer_size; i++){
        frame_buffer_[i] = background_color_;
        z_buffer_[i] = z_far_;
    }

}

bool Renderer::Rendering(){

    unsigned int lines_num = lines_.size();
    for(unsigned int index = 0; index < lines_num; index++){
        Line line = lines_[index];
        DrawLine(line.vertices[0], line.vertices[1], line.colors[0], line.colors[1]);
    }

    unsigned int triangles_num = triangles_.size();
    for(unsigned int index = 0; index < triangles_num; index++){
        Triangle& triangle = triangles_[index];
        Vec3f vertex[3]{
            triangle.vertices_camera[0],
            triangle.vertices_camera[1],
            triangle.vertices_camera[2]
        };
        if(is_render_models_){
            RenderTriangles(triangle);            
        }
        if(is_render_verties_){
            Vec3f v0(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
            Vec3f v1(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
            Vec3f v2(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
            if(triangle.texture_ptr != nullptr){
                v0 = triangle.texture_ptr->GetColor(triangle.texture_coords[0].u, triangle.texture_coords[0].v);  
                v1 = triangle.texture_ptr->GetColor(triangle.texture_coords[1].u, triangle.texture_coords[1].v);
                v2 = triangle.texture_ptr->GetColor(triangle.texture_coords[2].u, triangle.texture_coords[2].v);
            }

            SetPixel(vertex[0].x, vertex[0].y, v0);
            SetPixel(vertex[1].x, vertex[1].y, v1);
            SetPixel(vertex[2].x, vertex[2].y, v2);
        }
        if(is_render_edges_){
            for(int i = 0; i < 3; i++){
                Vec3f v0(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
                Vec3f v1(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
                if(triangle.texture_ptr != nullptr){
                    v0 = triangle.texture_ptr->GetColor(triangle.texture_coords[0].u, triangle.texture_coords[0].v);  
                    v1 = triangle.texture_ptr->GetColor(triangle.texture_coords[1].u, triangle.texture_coords[1].v);
                }
                Draw2DLine(Vec2i(vertex[i].x, vertex[i].y), Vec2i(vertex[(i+1)%3].x, vertex[(i+1)%3].y), v0, v1);
                if(is_showing_rendering){
                    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
                    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);            
                    cv::imshow("Rendering", image);
                    cv::waitKey(1);
                }
            }
        }
        if(is_render_normals_){
            Matrix4f mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
            Vec3f end[3]{
                vertex[0] + triangle.normals[0]*canvas_width_ * mvp_matrix.toMatrix3().inversed() * 0.02f,
                vertex[1] + triangle.normals[1]*canvas_height_ * mvp_matrix.toMatrix3().inversed() * 0.02f,
                vertex[2] + triangle.normals[2]*mvp_matrix.toMatrix3().inversed() * z_far_ * 0.02f
            };
            for(int i = 0; i < 3; i++){
                if(vertex[i].z - 0.01f < z_buffer_[static_cast<int>(vertex[i].y) * canvas_width_+ static_cast<int>(vertex[i].x)]){
                    DrawLine(vertex[i], end[i], ((triangle.normals[i]* mvp_matrix.toMatrix3().inversed()+Vec3f(1.0f,1.0f,1.0f))*0.5f).normalized(), Vec3f(0.0f,0.0f,0.0f));
                }
                if(is_showing_rendering){
                    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
                    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);            
                    cv::imshow("Rendering", image);
                    cv::waitKey(1);
                }
            }
        }
        if(is_showing_rendering){
            cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);            
            cv::imshow("Rendering", image);
            cv::waitKey(1);
        }
    }

    return true;
}

bool Renderer::RayTracing(const unsigned int& bounce_times){
    
    Ray ray(eye_pos_, eye_pos_.normalized()*-1.0f);
    for(int  y = 0; y < canvas_height_; y++){
        for(int x = 0; x < canvas_width_; x++){
            if(!is_MSAA_open_){
                ray.direction = GetRayVector(x, y);
                SetPixel(x, y+1, GetRayColor(ray, bounce_times));
            }
            else{
                Vec3f pixel_color(0.0f,0.0f,0.0f);                
                for(unsigned int i = 0; i < sample_rate_; i++){
                    ray.direction = GetRayVector(x+(rand()%100)*0.01f, y+(rand()%100)*0.01f);
                    pixel_color += GetRayColor(ray, bounce_times);
                }             
                SetPixel(x, y+1, pixel_color / sample_rate_);                
            }
        }
        cout<<"Rendering : "<< y * 100.0f / canvas_height_ <<"% "<< endl ;
    } 

    return true;
}

bool Renderer::SetShader(const shared_ptr<IShader>& shader){
    if(shader == nullptr){
        return false;        
    }
    else{
        shader_ptr_ = shader;
    }
    return true;
}

bool Renderer::LoadModel(const string& filename, const string& texture_name = ""){

    shared_ptr<Model> model_ptr = make_shared<Model>(filename);
    int surface_size = model_ptr->GetSurfeceSize();
    Triangle triangle(make_shared<Texture>(texture_name));
    for(unsigned int index = 0; index < surface_size; index++){
        array<Vec3i, 3> indexes = model_ptr->GetSurfece(index);//v uv nor
        for(int i = 0; i < 3; i++){
            triangle.vertices_world[i] = model_ptr->GetVertex(indexes[i].vertex);
            triangle.normals[i] = model_ptr->GetNormal(indexes[i].normal);            
            triangle.texture_coords[i] = model_ptr->GetTexture(indexes[i].uv);
        }
        triangles_.push_back(triangle);
    }   
    return true;
}

bool Renderer::LoadPoint(const Point& point){
    points_.push_back(point);
    return true;
}
bool Renderer::LoadPoint(const vector<Point>& points){
    for(Point point : points){
        points_.push_back(point);
    }
    return true;
}
bool Renderer::LoadLine(const Line& line){
    lines_.push_back(line);
    return true;
}
bool Renderer::LoadLine(const vector<Line>& lines){
    for(Line line : lines){
        lines_.push_back(line);
    }
    return true;
}
bool Renderer::ClearLine(){
    lines_.clear();
    return true;
}

bool Renderer::LoadTriangle(const Triangle& triangle){
    triangles_.push_back(triangle);
    return true;
}    
bool Renderer::LoadTriangle(const vector<Triangle>& triangles){
    for(const Triangle& triangle : triangles){
        triangles_.push_back(triangle);
    }
    return true;
}

bool Renderer::LoadRectangle(const array<Vec3f,4>& vertices, const array<Vec3f,4>& normals){
    Triangle triangle;
    for(int i = 0; i < 3; i += 2){
        for(int j = 0; j < 3; j++){
            triangle.vertices_world[j] = vertices[(i+j)%4];
            triangle.normals[j] = normals[(i+j)%4];
        }        
        triangles_.push_back(triangle);
    }
    return true;
}

bool Renderer::LoadRectangle(const vector<array<Vec3f,4>>& rectangles){
    return true;
}


bool Renderer::LoadLightSource(const vector<LightSource>& lights){
    shader_ptr_->SetLights(lights);
    return true;
}
bool Renderer::LoadObjectPtr(const vector<shared_ptr<Hitable>>& obj_ptrs){
    for(const shared_ptr<Hitable>& obj_ptr : obj_ptrs){
        hitable_list_.AddObjectPtr(obj_ptr);
    }
    return true;
}

bool Renderer::VertexShader(){
    view_port_cord_ = model_matrix_.toMatrix3();
    eye_pos_ = eye_pos_ * view_port_cord_;
    view_port_cord_ = view_port_cord_.transposed();
    shader_ptr_->SetEyePosition(eye_pos_);

    float f1 = (z_far_ - z_near_) * 0.5f;
    float f2 = (z_far_ + z_near_) * 0.5f;
    Matrix4f mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
    
    unsigned int triangle_num = triangles_.size();
    for(unsigned int index = 0; index < triangle_num; index++){
        Triangle& triangle = triangles_[index];
        for(int i = 0; i < 3; i++){
            Vec4f v4 = mvp_matrix * triangle.vertices_world[i].toVec4(1.0f);
            v4 /= v4.w;
            Vec3f v3 = {
                0.5f * canvas_width_ * (v4.x + 1.0f),
                0.5f * canvas_height_ * (v4.y + 1.0f),
                v4.z * f1 + f2
            };
            triangle.vertices_camera[i] = v3;
        }
    }

    return true;
}


void Renderer::SetModelMatrix(const float& x_axis, const float& y_axis, const float& z_axis){
    float theta_x = x_axis * 3.1415926535898f / 180.0f;
    float theta_y = y_axis * 3.1415926535898f / 180.0f;
    Matrix4f model_matrix_x = {
        {1.0f, 0.0f,            0.0f,           0.0f},
        {0.0f, cos(theta_x),    -sin(theta_x),  0.0f},
        {0.0f, sin(theta_x),    cos(theta_x),   0.0f},
        {0.0f, 0.0f,            0.0f,           1.0f}
    };
    Matrix4f model_matrix_y = {
        {cos(theta_y),  0.0f,   sin(theta_y),   0.0f},
        {0.0f,          1.0f,   0.0f,           0.0f},
        {-sin(theta_y), 0.0f,   cos(theta_y),   0.0f},
        {0.0f,          0.0f,   0.0f,           1.0f},
    };
    model_matrix_ = model_matrix_x * model_matrix_y;
}
void Renderer::SetViewMatrix(const Vec3f& eye_pos){
    eye_pos_ = eye_pos;
    view_matrix_ = Matrix4f{
        {1.0f,          0.0f,           0.0f,       0.0f},
        {0.0f,          1.0f,           0.0f,       0.0f},
        {0.0f,          0.0f,           1.0f,       0.0f},
        {-eye_pos.x,    -eye_pos.y,     -eye_pos.z, 1.0f}
    };
}
void Renderer::SetProjectionMatrix(const float& eye_fov,const float& aspect_ratio,const float& zNear, const float& zFar){
    eye_fov_ = eye_fov;
    aspect_ratio_ = aspect_ratio;
    z_near_ = zNear;
    z_far_ = zFar;
    float theta = eye_fov * 3.1415926535898f / 360.0f;
    view_port_height_half_ = atan(theta) * zNear;
    view_port_width_half_ = view_port_height_half_ * aspect_ratio_;  
    float left = -view_port_width_half_;      
    float bottom = -view_port_height_half_;

    Matrix4f otho1 = {
        {1.0f,                                      0.0f,                                       0.0f,                   0.0f},
        {0.0f,                                      1.0f,                                       0.0f,                   0.0f},
        {0.0f,                                      0.0f,                                       1.0f,                   0.0f},
        {(left + view_port_width_half_) * -0.5f,    (view_port_height_half_ + bottom) * -0.5f,  (zNear + zFar) * -0.5f, 1.0f}
    };

    Matrix4f otho2 = {
        {2.0f/(view_port_width_half_-left), 0,                                      0,                   0},
        {0,                                 2.0f/(view_port_height_half_-bottom),   0,                   0},
        {0,                                 0,                                      2.0f/(zFar-zNear),   0},
        {0,                                 0,                                      0,                   1}
    };

    Matrix4f perspective = {
        {zNear,    0.0f,  0.0f,            0},
        {0.0f,     zNear, 0.0f,            0},
        {0.0f,     0.0f,  zNear+zFar,   1.0f},
        {0.0f,     0.0f,  -zNear*zFar,  0.0f}
    };

    projection_matrix_ = otho2 * otho1 * perspective;
}

bool Renderer::SetPixel(const unsigned int& x, const unsigned int& y, const Vec3f& color){
    int ind = (canvas_height_- y) * canvas_width_ + x;
    frame_buffer_[ind] = color;
    return true;   
}
bool Renderer::Draw2DLine(Vec2i p1, Vec2i p2, const Vec3f& color1, const Vec3f& color2){
    bool is_reverse = false;
    if(abs(p1.x - p2.x) < abs(p1.y - p2.y)){
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        is_reverse = true;
    }
    
    if(p1.x > p2.x){
        swap(p1, p2);
    }

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int x_end = p2.x;
    int y =  p1.y;
    int derror = abs(dy) * 2;
    int error = 0;

    for(int x = p1.x; x < x_end; x += 1){ 
        float t = 1.0f * x / dx;
        Vec3f color = color2 * t + color1 * (1.0f - t);
        if(is_reverse){
            SetPixel(y, x, color);        
        }
        else{
            SetPixel(x, y, color);
        }
        error += derror;
        if(error > dx){
            y += p2.y > p1.y ? 1 : -1;
            error -= dx * 2;
        }
    }

    return true;
}

bool Renderer::DrawLine(Vec3f p1, Vec3f p2, const Vec3f& color1, const Vec3f& color2){
    bool is_reverse = false;
    if(abs(p1.x - p2.x) < abs(p1.y - p2.y)){
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        is_reverse = true;
    }
    
    if(p1.x > p2.x){
        swap(p1, p2);
    }

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int x_end = p2.x;
    int y =  p1.y;
    int derror = abs(dy) * 2;
    int error = 0;
    float dz = (p2.z - p1.z) / dx;
    float z = p1.z;

    for(int x = p1.x; x < x_end; x++){ 
        float t = 1.0f * x / dx;
        Vec3f color = color2 * t + color1 * (1.0f - t);
        if(is_reverse){
            if(z < z_buffer_[x * canvas_width_+ y]){
                z_buffer_[x * canvas_width_+ y] = z;
                SetPixel(y, x, color);    
            }            
        }
        else{
            if(z < z_buffer_[y * canvas_width_+ x]){
                z_buffer_[y * canvas_width_+ x] = z;
                SetPixel(x, y, color);
            }
        }
        error += derror;
        z += dz;
        if(error > dx){
            y += p2.y > p1.y ? 1 : -1;
            error -= dx * 2;
        }
    }

    return true;
}
bool Renderer::RenderTriangles(const Triangle& triangle){
    Vec2f bbox[2];
    const array<Vec3f, 3>& vertex = triangle.vertices_camera;
    const array<Vec3f, 3>& normals = triangle.normals;
    const array<Vec2f, 3>& uv = triangle.texture_coords;
    FindBoundingBox(vertex, bbox);
    int max_x = static_cast<int>(bbox[1].x)+1;
    int max_y = static_cast<int>(bbox[1].y)+1;
    for(int y = bbox[0].y; y < max_y; y++){
        for(int x = bbox[0].x; x < max_x; x++){
            float weight = 1.0f;
            if(is_MSAA_open_){
                float offset[2] = {0.25, 0.75f};
                for(int i = 0; i < 2; i++){
                    for(int j = 0 ; j < 2; j++){
                        if(!IsInsideTriangle(vertex, Vec2f(x+offset[i], y+offset[j]))){
                            weight -= 0.25f;
                        }
                    }
                }
            }
            else{
                if(!IsInsideTriangle(vertex, Vec2f(x+0.5, y+0.5))){
                    weight = 0.0f;
                }
            }
            if(weight > 0.2f){
                Vec3f barycentric = BarycentricInterpolation(vertex, Vec2f(x+0.5f, y+0.5f));
                float pixel_z = barycentric.x * vertex[0].z + barycentric.y * vertex[1].z + barycentric.z * vertex[2].z;
                pixel_z = pixel_z * weight + (1.0f - weight) * z_far_;
                if(pixel_z > 0.0f && pixel_z < z_buffer_[y * canvas_width_+ x]){
                    z_buffer_[y*canvas_width_+x] = pixel_z;
                    Vec2f uv_interpolated(uv[0] * barycentric.x + uv[1] * barycentric.y + uv[2] * barycentric.z);
                    Vec3f pos(triangle.vertices_world[0] * barycentric.x + triangle.vertices_world[1] * barycentric.y + triangle.vertices_world[2] * barycentric.z);
                    Vec3f normal(normals[0] * barycentric.x + normals[1] * barycentric.y + normals[2] * barycentric.z);
                    Vec3f color(1.0f, 1.0f, 1.0f);
                    Vec3f texture = triangle.texture_ptr ?
                        triangle.texture_ptr->GetColor(uv_interpolated.u, uv_interpolated.v) : color;
                    Matrix2f st = {
                        uv[0] - uv_interpolated,
                        uv[1] - uv_interpolated
                    };
                    Vec3f vec1 = vertex[0] - pos;
                    Vec3f vec2 = vertex[1] - pos;
                    st = st.inversed();
                    Matrix3f TBN;
                    TBN.vec[0] = {
                        st.vec[0].x * vec1.x + st.vec[1].x * vec2.x,
                        st.vec[0].x * vec1.y + st.vec[1].x * vec2.y,
                        st.vec[0].x * vec1.z + st.vec[1].x * vec2.z
                    };
                    TBN.vec[1] = {
                        st.vec[0].y * vec1.x+ st.vec[1].y * vec2.x,
                        st.vec[0].y * vec1.y+ st.vec[1].y * vec2.y,
                        st.vec[0].y * vec1.z+ st.vec[1].y * vec2.z
                    };
                    TBN.vec[2] = normal;
                    FragmentShaderPayload payload(pos, color, texture, normal, TBN);
                    color = shader_ptr_->FragmentShader(payload);
                    SetPixel(x, y, color * weight);
                }
            }
        }
    }
    return true;
}

void Renderer::FindBoundingBox(const array<Vec3f,3>& vertex, Vec2f bbox[2]){
    bbox[0].x = vertex[2].x;
    bbox[0].y = vertex[2].y;
    bbox[1].x = vertex[2].x;
    bbox[1].y = vertex[2].y;

    for(int i = 0; i < 2; i++ ){
        if(vertex[i].x < bbox[0].x){
            bbox[0].x = vertex[i].x;
        }
        else if(vertex[i].x > bbox[1].x){
            bbox[1].x = vertex[i].x;
        }
        if(vertex[i].y < bbox[0].y){
            bbox[0].y = vertex[i].y;
        }
        else if(vertex[i].y > bbox[1].y){
            bbox[1].y = vertex[i].y;
        }
    }
    if(vertex[0].x < 0.0f){bbox[0].x = 0.0f;}
    if(vertex[0].y < 0.0f){bbox[0].y = 0.0f;}
    if(vertex[1].x > canvas_width_){bbox[1].x = canvas_width_;}
    if(vertex[1].y > canvas_height_){bbox[1].y = canvas_height_;}

}

bool Renderer::IsInsideTriangle(const array<Vec3f, 3>& vertex, const Vec2f& pixel){
    int count = 0;
    Vec2f vertex2[3] = {
        {vertex[0].x, vertex[0].y},
        {vertex[1].x, vertex[1].y},
        {vertex[2].x, vertex[2].y}
    };
    for(int i = 0; i < 3; i++){
        Vec2f v1 = vertex2[(i+1)%3] - vertex2[i];
        Vec2f v2 = pixel - vertex2[i];

        float dot = v1.cross(v2);
        if(dot > 0.0f) count++;
    }
    if( count == 0 || count == 3){
        return true;
    }
    return false;
}

Vec3f Renderer::BarycentricInterpolation(const array<Vec3f,3>& vertex,  const Vec2f& pixel){
    Vec2f v0(vertex[0].x, vertex[0].y);
    Vec2f v1(vertex[1].x, vertex[1].y);
    Vec2f v2(vertex[2].x, vertex[2].y);
    float area = 1.0f / (v0-v1).cross(v2 - v1);
    float alpha = (pixel-v1).cross(v2-v1) * area;
    float beta = (pixel-v2).cross(v0-v2) * area;
    float gamma = (pixel-v0).cross(v1-v0) * area;
    return Vec3f(alpha, beta, gamma); 
}

Vec3f Renderer::GetRayVector(const float& x, const float& y){
    return (view_port_cord_[0] * (x * 2.0f / canvas_width_ - 1.0f) * view_port_width_half_
         + view_port_cord_[1] * (y * 2.0f / canvas_height_ - 1.0f) * view_port_height_half_
         + view_port_cord_[2] * z_near_).normalized();
}

Vec3f Renderer::GetRayColor(const Ray& ray, const unsigned int step){
    HitPointInfo info;
    if(step == 0){
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
    if(hitable_list_.HitObject(ray, 0.0001f, 1e6, info)){
        Ray scattered(info.hit_point);
        Vec3f attenuation;
        info.material_ptr->Scatter(ray, info, attenuation, scattered);
        return GetRayColor(scattered, step-1).cwiseProduct(attenuation);
    }
    int t = ray.direction.y*0.5;
    return Vec3f(1.0f, 1.0f, 1.0f)*(0.5f - t) + Vec3f(0.5f, 0.7f, 1.0f)*(0.5f + t);
}

}