#include"renderer.h"
namespace YeahooQAQ{

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
    z_near_(0.0f),
    z_far_(0.0f),
    model_matrix_(),
    view_matrix_(),
    projection_matrix_(),
    point_lights_(),
    textures_ptrs_(),
    shader_ptr_(nullptr)
{
    frame_buffer_ = new Vec3f[width * height];
    z_buffer_ = new float[width * height];
    shader_ptr_ = new IShader;
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
    unsigned int texture_num = textures_ptrs_.size();
    for(int i = 0; i < texture_num; i++){
        if(textures_ptrs_[i] != nullptr){
            delete textures_ptrs_[i];
        }
    }
    if(shader_ptr_ != nullptr){
        delete shader_ptr_;
        shader_ptr_ = nullptr;
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
    cout<<"time cost: "<<duration_ * time_per_tick_ *1000 <<" ms\n";
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
    cout<<"\nPush 's to save image or push 'q' to quit\n";
    char k = cin.get();
    if(k == 's'){
        cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);            
        cv::imshow("Rendering", image);
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
    for(int i = 0;i < canvas_width_ * canvas_height_; i++){
        frame_buffer_[i] = background_color_;
    }
}

bool Renderer::Rendering(){
    cout<<"Rendering model ...\n";
    
    unsigned int lines_num = lines_.size();
    for(unsigned int index = 0; index < lines_num;index++){
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
                v0 = triangle.texture_ptr->getColor(triangle.texture_coords[0].u, triangle.texture_coords[0].v);  
                v1 = triangle.texture_ptr->getColor(triangle.texture_coords[1].u, triangle.texture_coords[1].v);
                v2 = triangle.texture_ptr->getColor(triangle.texture_coords[2].u, triangle.texture_coords[2].v);
            }

            SetPixel(Vec2i(vertex[0].x, vertex[0].y), v0);
            SetPixel(Vec2i(vertex[1].x, vertex[1].y), v1);
            SetPixel(Vec2i(vertex[2].x, vertex[2].y), v2);
        }
        if(is_render_edges_){
            for(int i = 0; i < 3; i++){
                Vec3f v0(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
                Vec3f v1(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
                if(triangle.texture_ptr != nullptr){
                    v0 = triangle.texture_ptr->getColor(triangle.texture_coords[0].u, triangle.texture_coords[0].v);  
                    v1 = triangle.texture_ptr->getColor(triangle.texture_coords[1].u, triangle.texture_coords[1].v);
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

bool Renderer::SetShader(IShader*& shader){
    if(shader == nullptr) return false;
    if(shader_ptr_ != nullptr){
        delete shader_ptr_;
        shader_ptr_ = shader;
    }
    else{
        shader_ptr_ = shader;
    }
    return true;
}

bool Renderer::LoadModel(const string& filename, const string& texture_name = ""){

    Model* model_ptr = new Model(filename);
    Texture* texture_ptr = new Texture(texture_name);
    if(texture_ptr->IsValid()){
        textures_ptrs_.push_back(texture_ptr);
    }
    else{
        delete texture_ptr;
        texture_ptr = nullptr;
    }

    int surface_size = model_ptr->GetSurfeceSize();
    Triangle triangle(texture_ptr);
    for(unsigned int index = 0; index < surface_size; index++){
        array<Vec3i, 3> indexes = model_ptr->GetSurfece(index);//v uv nor
        for(int i = 0; i < 3; i++){
            triangle.vertices_world[i] = model_ptr->GetVertex(indexes[i].vertex);
            triangle.normals[i] = model_ptr->GetNormal(indexes[i].normal);            
            triangle.texture_coords[i] = model_ptr->GetTexture(indexes[i].uv);
        }
        triangles_.push_back(triangle);
    }

    if(model_ptr != nullptr){
        delete model_ptr;
        model_ptr = nullptr;
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

bool Renderer::LoadPointLights(const vector<PointLight>& lights){
    shader_ptr_->SetLights(lights);
}

bool Renderer::MvpTransforme(){
    unsigned int z_buffer_size = canvas_width_ * canvas_height_;
    for(int i = 0; i <  z_buffer_size; i++){
        z_buffer_[i] = z_far_;
    };
    eye_pos_ = eye_pos_ * model_matrix_.toMatrix3();
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
        {1.0f, 0.0f,                    0.0f,                  0.0f},
        {0.0f, cos(theta_x),  -sin(theta_x), 0.0f},
        {0.0f, sin(theta_x), cos(theta_x), 0.0f},
        {0.0f, 0.0f,                    0.0f,                   1.0f}
    };
    Matrix4f model_matrix_y = {
        {cos(theta_y), 0.0f, sin(theta_y), 0.0f},
        {0.0f,                   1.0f, 0.0f,                   0.0f},
        {-sin(theta_y),0.0f, cos(theta_y), 0.0f},
        {0.0f,                   0.0f, 0.0f,                   1.0f},
    };
    model_matrix_ = model_matrix_x * model_matrix_y;
}
void Renderer::SetViewMatrix(const Vec3f& eye_pos){
    eye_pos_ = eye_pos;
    view_matrix_ = Matrix4f{
        {1.0f,                  0.0f,               0.0f,               0.0f},
        {0.0f,                  1.0f,               0.0f,               0.0f},
        {0.0f,                  0.0f,               1.0f,               0.0f},
        {-eye_pos.x, -eye_pos.y, -eye_pos.z, 1.0f}
    };
}
void Renderer::SetProjectionMatrix(const float& eye_fov, const float& aspect_ratio, const float& zNear, const float& zFar){
    z_near_ = zNear;
    z_far_ = zFar;
    float theta = eye_fov * 3.1415926535898f / 360.0f;
    float top = atan(theta) * zNear;
    float bottom =  -top;
    float right = top * aspect_ratio;
    float left = -right;
    Matrix4f otho1 = {
        {1.0f,                               0.0f,                                      0.0f,                                  0.0f},
        {0.0f,                               1.0f,                                      0.0f,                                  0.0f},
        {0.0f,                               0.0f,                                      1.0f,                                  0.0f},
        {(left + right) * -0.5f, (top + bottom) * -0.5f ,(zNear + zFar) * -0.5f ,1.0f}
    };

    Matrix4f otho2 = {
        {2.0f/(right-left), 0,                                   0,                                 0},
        {0,                             2.0f/(top-bottom), 0,                                 0},
        {0,                             0,                                   2.0f/(zFar-zNear), 0},
        {0,                             0,                                   0,                                 1}
    };

    Matrix4f perspective = {
        {zNear, 0.0f,     0.0f,                        0},
        {0.0f,     zNear, 0.0f,                        0},
        {0.0f,     0.0f,      zNear+zFar,  1.0f},
        {0.0f,     0.0f,      -zNear*zFar, 0.0f}
    };

    projection_matrix_ = otho2 * otho1 * perspective;
}

bool Renderer::SetPixel(const Vec2i& pos, const Vec3f& color){
    int ind = (canvas_height_-pos.y)*canvas_width_ + pos.x;
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
            SetPixel(Vec2i(y, x), color);        
        }
        else{
            SetPixel(Vec2i(x, y), color);
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
                SetPixel(Vec2i(y, x), color);    
            }            
        }
        else{
            if(z < z_buffer_[y * canvas_width_+ x]){
                z_buffer_[y * canvas_width_+ x] = z;
                SetPixel(Vec2i(x, y), color);
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
    if(!is_MSAA_open_){
        for(int y = bbox[0].y; y < max_y; y++){
            for(int x = bbox[0].x; x < max_x; x++){
                if(IsInsideTriangle(vertex, Vec2f(x+0.5, y+0.5))){
                    Vec3f barycentric = BarycentricInterpolation(vertex, Vec2f(x+0.5f, y+0.5f));
                    float pixel_z = barycentric.x * vertex[0].z + barycentric.y * vertex[1].z + barycentric.z * vertex[2].z;
                    if(pixel_z > 0.0f && pixel_z < z_buffer_[y * canvas_width_+ x]){
                        z_buffer_[y*canvas_width_+x] = pixel_z;
                        Vec2f uv_interpolated(uv[0] * barycentric.x + uv[1] * barycentric.y + uv[2] * barycentric.z);
                        Vec3f pos(triangle.vertices_world[0] * barycentric.x + triangle.vertices_world[1] * barycentric.y + triangle.vertices_world[2] * barycentric.z);
                        Vec3f normal(normals[0] * barycentric.x + normals[1] * barycentric.y + normals[2] * barycentric.z);
                        Vec3f texture(triangle.texture_ptr->getColor(uv_interpolated.x, uv_interpolated.y));
                        Vec3f color(1.0f, 1.0f, 1.0f);
                        FragmentShaderPayload payload(pos, color, normal, texture);
                        color = shader_ptr_->FragmentShader(payload);
                        SetPixel(Vec2i(x, y), color);
                    }
                }
            }
        }
    }
    else{
        for(int y = bbox[0].y; y < max_y; y++){
            for(int x = bbox[0].x; x < max_x; x++){
                float weight = 0.0f;
                float offset[2] = {0.25, 0.75f};
                for(int i = 0; i < 2; i++){
                    for(int j = 0 ; j < 2; j++){
                        Vec2f pixel(x+offset[i], y+offset[j]);
                        if(IsInsideTriangle(vertex, pixel)){
                            weight += 0.25f;
                        }
                    }
                }
                if(weight < 0.2f) continue;
                Vec3f barycentric = BarycentricInterpolation(vertex, Vec2f(x+0.5, y+0.5));
                float pixel_z = barycentric.x * vertex[0].z + barycentric.y * vertex[1].z + barycentric.z * vertex[2].z;
                if(pixel_z < z_buffer_[y * canvas_width_+ x]){
                    if(pixel_z > 0.9f) z_buffer_[y*canvas_width_+x] = pixel_z;
                    Vec2f uv_interpolated(uv[0] * barycentric.x + uv[1] * barycentric.y + uv[2] * barycentric.z);
                    Vec3f normal = normals[0] * barycentric.x + normals[1] * barycentric.y + normals[2] * barycentric.z;
                    Vec3f texture = triangle.texture_ptr->getColor(uv_interpolated.x, uv_interpolated.y);
                    Vec3f color(1.0f, 1.0f, 1.0f);
                    FragmentShaderPayload payload(Vec3f(x, y, pixel_z), color, normal.normalized(), texture);
                    color = shader_ptr_->FragmentShader(payload) * weight;
                    SetPixel(Vec2i(x, y), color);
                }
            }
        }
    }
    return true;
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



}