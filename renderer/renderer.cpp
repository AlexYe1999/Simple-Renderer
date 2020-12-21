#include"renderer.h"
namespace YeahooQAQ{

Renderer::Renderer(const unsigned int& width, const unsigned int& height, const Vec3f& background_color)
    :
    time_per_tick_(1.0 / cv::getTickFrequency()),
    is_clock_running_(false),
    is_showing_rendering(false),
    is_MSAA_open_(false),
    start_time_(0.0),
    end_time_(0.0),
    duration_(0.0),
    canvas_width_(width),
    canvas_height_(height),
    background_color_(background_color),
    canvas_(),
    model_ptr_(nullptr),
    z_buffer_(nullptr),
    surface_size_(0),
    vertex_size_(0),
    normal_size_(0),
    texture_size_(0),
    surfaces_(), 
    vertices_(),
    normals_(), 
    textures_(),
    z_near_(0.0f),
    z_far_(0.0f),
    model_matrix_(),
    view_matrix_(),
    projection_matrix_(),
    lights_(),
    shader_()
{
    z_buffer_ = new float[width* height];
}

Renderer::~Renderer(){
    if(model_ptr_ != nullptr){
        delete model_ptr_;
        model_ptr_ = nullptr;
    }
    if(z_buffer_ = nullptr){
        delete[] z_buffer_;
        z_buffer_ = nullptr;
    }
};

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
    cv::Mat image;
    cv::flip(canvas_, image, 0);
    cv::namedWindow(window_name);
    imshow(window_name, image);
    cv::waitKey(delay_ms);
    StartClock();
}

void Renderer::SaveImage(const std::string& filename){
    cv::flip(canvas_, canvas_, 0);
    cv::namedWindow("OutputImage");
    cv::imshow("OutputImage", canvas_);
    cout<<"\nPush 's to save image or push 'q' to quit\n";
    int k = cv::waitKey(0);
    if(k == 's'){
        if(imwrite(filename, canvas_*255.0f)){
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

bool Renderer::LoadSets(const vector<Light>& lights){
    lights_ = lights;
}

bool Renderer::LoadModel(const string& filename, const string& texture_name){
    if(model_ptr_ == nullptr){
        model_ptr_ = new Model(filename, texture_name);
    }
    else{
        cerr <<"Model has been loaded\n";
        return false;
    }

    surface_size_ = model_ptr_->GetSurfeceSize();
    vertex_size_ = model_ptr_->GetVertexSize();
    normal_size_ = model_ptr_->GetNormalSize();
    texture_size_ = model_ptr_->GetTextureSize();

    for(unsigned int index = 0; index < surface_size_; index++){
        surfaces_.push_back(model_ptr_->GetSurfece(index));
    }
    for(unsigned int index = 0; index < texture_size_; index++){
        textures_.push_back(model_ptr_->GetTexture(index));
    }
    vertices_.resize(vertex_size_);
    normals_.resize(normal_size_);
    return true;
}

bool Renderer::MvpTransforme(){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }

    unsigned int z_buffer_size = canvas_width_ * canvas_height_;
    for(int i = 0; i <  z_buffer_size; i++){
        z_buffer_[i] = z_far_;
    };

    float f1 = (z_far_ - z_near_) * 0.5f;
    float f2 = (z_far_ + z_near_) * 0.5f;
    Matrix4f mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
    
    vector<Light> lights = lights_;
    for(Light& light : lights){
        Vec4f v4 = mvp_matrix * light.position.toVec4(1.0f);
        v4 /= v4.w;
        Vec3f v3 = {
            0.5f * canvas_width_ * (v4.x + 1.0f),
            0.5f * canvas_height_ * (v4.y + 1.0f),
            v4.z * f1 + f2
        };
        light.position = v3;
    }
    //shader_.LoadProperties(lights, Vec3f(canvas_width_*0.5f, canvas_height_*0.5f, 0.0f));
    shader_.LoadProperties(lights, Vec3f(canvas_width_*0.5f, canvas_height_*0.5f, 0.0f));
    
    for(unsigned int index = 0; index < vertex_size_; index++){
        Vec4f v4 = mvp_matrix * model_ptr_->GetVertex(index).toVec4(1.0f);
        v4 /= v4.w;
        Vec3f v3 = {
            0.5f * canvas_width_ * (v4.x + 1.0f),
            0.5f * canvas_height_ * (v4.y + 1.0f),
            v4.z * f1 + f2
        };
        vertices_[index] = v3;
    }

    for(unsigned int index = 0; index < normal_size_; index++){
        Vec3f vec = model_ptr_->GetNormal(index) * mvp_matrix.toMatrix3().inversed();
        normals_[index] = vec.normalized();
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

bool Renderer::RenderPointModel(){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"rendering point ...\n";

    for(int index = 0; index < surface_size_; index++){
        vector<Vec3i> indexes = surfaces_[index];
        Vec3f vertex[3]{
            vertices_[indexes[0].vertex],
            vertices_[indexes[1].vertex],
            vertices_[indexes[2].vertex]
        };
        Vec3f v0 = model_ptr_->getColor(textures_[indexes[0].uv].u, textures_[indexes[0].uv].v);  
        Vec3f v1 = model_ptr_->getColor(textures_[indexes[1].uv].u, textures_[indexes[1].uv].v);
        Vec3f v2 = model_ptr_->getColor(textures_[indexes[2].uv].u, textures_[indexes[2].uv].v);
        canvas_.at<cv::Scalar>(vertex[0].y, vertex[0].x) = cv::Scalar(v0.z / 255.0f, v0.y / 255.0f, v0.x / 255.0f);
        canvas_.at<cv::Scalar>(vertex[1].y, vertex[1].x) = cv::Scalar(v1.z / 255.0f, v1.y / 255.0f, v1.x / 255.0f);
        canvas_.at<cv::Scalar>(vertex[2].y, vertex[2].x) = cv::Scalar(v2.z / 255.0f, v2.y / 255.0f, v2.x / 255.0f);
        if(is_showing_rendering){
            cv::imshow("Rendering", canvas_);
            cv::waitKey(1);
        }
    }

}

bool Renderer::RenderWireEdge(const Vec3f& color){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"Rendering edge ...\n";

    for(int index = 0; index < surface_size_; index++){
        vector<Vec3i>& surface = surfaces_[index];
        Vec3f vertex[3]{
            vertices_[surfaces_[index][0].vertex],
            vertices_[surfaces_[index][1].vertex],
            vertices_[surfaces_[index][2].vertex]
        };
        for(int i = 0; i < 3; i++){
            Draw2DLine(Vec2i(vertex[i].x, vertex[i].y), Vec2i(vertex[(i+1)%3].x, vertex[(i+1)%3].y), color);
            if(is_showing_rendering){
                cv::imshow("Rendering", canvas_);
                cv::waitKey(1);
            }
        }
    }

    return true;
}

bool Renderer::RenderNormal(const Vec3f& color){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"Rendering normal ...\n";

    for(int index = 0; index < surface_size_; index++){
        vector<Vec3i>& surface = surfaces_[index];
        Vec3f start[3]{
            vertices_[surfaces_[index][0].vertex],
            vertices_[surfaces_[index][1].vertex],
            vertices_[surfaces_[index][2].vertex]
        };
        Vec3f normals[3]{
            normals_[surfaces_[index][0].normal],
            normals_[surfaces_[index][1].normal],
            normals_[surfaces_[index][2].normal]
        };
        Vec3f end[3]{
            start[0] + normals[0]*canvas_width_ * 0.01f,
            start[1] + normals[1]*canvas_height_ * 0.01f,
            start[2] + normals[2]*z_far_ * 0.01f
        };
        for(int i = 0; i < 3; i++){
            if(start[i].z - 0.01f < z_buffer_[static_cast<int>(start[i].y) * canvas_width_+ static_cast<int>(start[i].x)]){
                DrawLine(start[i], end[i], (normals[i]*-1.0f+Vec3f(1.0f,1.0f,1.0f))*0.5f);
            }
            if(is_showing_rendering){
                cv::imshow("Rendering", canvas_);
                cv::waitKey(1);
            }
        }
    }

}

bool Renderer::RenderModel(const ShaderType& shader_type){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"Rendering model ...\n";
    shader_.Setting(shader_type);
    for(int index = 0; index < surface_size_; index++){
        vector<Vec3i>& indexes = surfaces_[index];
        Vec3f vertex[3]{
            vertices_[indexes[0].vertex],
            vertices_[indexes[1].vertex],
            vertices_[indexes[2].vertex]
        };
        Vec3f normals[3]{
            normals_[indexes[0].normal],
            normals_[indexes[1].normal],
            normals_[indexes[2].normal]
        };
        Vec2f uv[3]{
            textures_[indexes[0].uv],         
            textures_[indexes[1].uv],    
            textures_[indexes[2].uv]
        };
        RenderTriangles(vertex, normals, uv);
        if(is_showing_rendering){
            cv::imshow("Rendering", canvas_);
            cv::waitKey(1);
        }
    }
    return true;
}

bool Renderer::Draw2DLine(Vec2i p1, Vec2i p2, const Vec3f& color){
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
        if(is_reverse){
            canvas_.at<cv::Scalar>(x, y) = cv::Scalar(color.z, color.y, color.x);            
        }
        else{
            canvas_.at<cv::Scalar>(y, x) = cv::Scalar(color.z, color.y, color.x);
        }
        error += derror;
        if(error > dx){
            y += p2.y > p1.y ? 1 : -1;
            error -= dx * 2;
        }
    }

    return true;
}

bool Renderer::DrawLine(Vec3f p1, Vec3f p2, const Vec3f& color){
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
        if(is_reverse){
            if(z < z_buffer_[x * canvas_width_+ y]){
                z_buffer_[x * canvas_width_+ y] = z;
                canvas_.at<cv::Scalar>(x, y) = cv::Scalar(color.z, color.y, color.x);          
            }            
        }
        else{
            if(z < z_buffer_[y * canvas_width_+ x]){
                z_buffer_[y * canvas_width_+ x] = z;
                canvas_.at<cv::Scalar>(y, x) = cv::Scalar(color.z, color.y, color.x);
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

bool Renderer::RenderTriangles(Vec3f* vertex, Vec3f* normals, Vec2f* uv){
    Vec2f bbox[2];
    FindBoundingBox(vertex, bbox);
    int max_x = static_cast<int>(bbox[1].x)+1;
    int max_y = static_cast<int>(bbox[1].y)+1;
    if(!is_MSAA_open_){
        for(int y = bbox[0].y; y < max_y; y++){
            for(int x = bbox[0].x; x < max_x; x++){
                if(IsInsideTriangle(vertex, Vec2f(x+0.5, y+0.5))){
                    Vec3f barycentric = BarycentricInterpolation(vertex, Vec2f(x+0.5, y+0.5));
                    float pixel_z = barycentric.x * vertex[0].z + barycentric.y * vertex[1].z + barycentric.z * vertex[2].z;
                    if(pixel_z < z_buffer_[y * canvas_width_+ x]){
                        z_buffer_[y*canvas_width_+x] = pixel_z;
                        Vec2f uv_interpolated(uv[0] * barycentric.x + uv[1] * barycentric.y + uv[2] * barycentric.z);
                        Vec3f color(1.0f, 1.0f, 1.0f);
                        Vec3f normal = normals[0] * barycentric.x + normals[1] * barycentric.y + normals[2] * barycentric.z;
                        Vec3f texture = model_ptr_->getColor(uv_interpolated.x, uv_interpolated.y);
                        FragmentShaderPayload payload(Vec3f(x, y, pixel_z), color, normal, texture);
                        color = shader_.FragmentShader(payload);
                        canvas_.at<cv::Scalar>(y, x) = cv::Scalar(color.z, color.y, color.x);            
                    }
                }
            }
        }
    }
    else{
        for(int y = bbox[0].y; y < max_y; y++){
            for(int x = bbox[0].x; x < max_x; x++){
                float pixel_z = 0.0f;
                unsigned int count = 0;
                float offset[2] = {0.3f, 0.7f};
                for(int i = 0; i < 2; i++){
                    for(int j = 0 ; j < 2; j++){
                        Vec2f pixel(x+offset[i], y+offset[j]);
                        if(IsInsideTriangle(vertex, pixel)){
                            count++;
                            Vec3f barycentric = BarycentricInterpolation(vertex, pixel);
                            pixel_z += barycentric.x * vertex[0].z + barycentric.y * vertex[1].z + barycentric.z * vertex[2].z;
                        }
                    }
                }
                pixel_z = count > 1 ? pixel_z / count : z_far_;           
                if(pixel_z < z_buffer_[y * canvas_width_+ x]){
                    z_buffer_[y*canvas_width_+x] = pixel_z;
                    Vec3f barycentric = BarycentricInterpolation(vertex, Vec2f(x+0.5, y+0.5));
                    Vec2f uv_interpolated(uv[0] * barycentric.x + uv[1] * barycentric.y + uv[2] * barycentric.z);
                    Vec3f color(1.0f, 1.0f, 1.0f);
                    Vec3f normal = normals[0] * barycentric.x + normals[1] * barycentric.y + normals[2] * barycentric.z;
                    Vec3f texture = model_ptr_->getColor(uv_interpolated.x, uv_interpolated.y);
                    FragmentShaderPayload payload(Vec3f(x, y, pixel_z), color, normal.normalized(), texture);
                    color = shader_.FragmentShader(payload);
                    canvas_.at<cv::Scalar>(y, x) = cv::Scalar(color.z, color.y, color.x);
                }
            }
        }
    }

    return true;
}

Vec3f Renderer::BarycentricInterpolation(const Vec3f vec[3],  const Vec2f& pixel){
    Vec2f v0(vec[0].x, vec[0].y);
    Vec2f v1(vec[1].x, vec[1].y);
    Vec2f v2(vec[2].x, vec[2].y);
    float area = 1.0f / (v0-v1).cross(v2 - v1);
    float alpha = (pixel-v1).cross(v2-v1) * area;
    float beta = (pixel-v2).cross(v0-v2) * area;
    float gamma = (pixel-v0).cross(v1-v0) * area;
    return Vec3f(alpha, beta, gamma); 
}

void Renderer::FindBoundingBox(const Vec3f vertex[3], Vec2f bbox[2]){
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

bool Renderer::IsInsideTriangle(const Vec3f vertex[3], const Vec2f& pixel){
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