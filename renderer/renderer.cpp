#include"renderer.h"
namespace YeahooQAQ{

Renderer::Renderer(unsigned int width, unsigned int height, cv::Scalar& background_color)
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
    canvas_(height, width, CV_64FC4, background_color),
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
    projection_matrix_()
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

void Renderer::SaveImage(std::string filename){
    cv::flip(canvas_, canvas_, 0);
    cv::namedWindow("OutputImage");
    cv::imshow("OutputImage", canvas_);
    cout<<"\nPush 's to save image or push 'q' to \n";
    int k = cv::waitKey(0);
    if(k == 's'){
        if(imwrite(filename, canvas_)){
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
    return true;
}

bool Renderer::LoadTransformedVertex(){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    vertices_.clear();
    normals_.clear();
    float f1 = (z_far_ - z_near_) * 0.5f;
    float f2 = (z_far_ + z_near_) * 0.5f;
    Matrix4f mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
    for(unsigned int index = 0; index < vertex_size_; index++){
        Vec4f v4 = mvp_matrix * model_ptr_->GetVertex(index).toVec4(1.0f);
        v4 /= v4.w;
        Vec3f v3 = {
            0.5f * canvas_width_ * (v4.x + 1.0f),
            0.5f * canvas_height_ * (v4.y + 1.0f),
            v4.z * f1 + f2
        };
        vertices_.push_back(v3);
    }
    for(unsigned int index = 0; index < normal_size_; index++){
        normals_.push_back(model_ptr_->GetNormal(index));
    }

    return true;
}

void Renderer::SetModelMatrix(const float x_axis, const float y_axis, const float z_axis){
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
void Renderer::SetProjectionMatrix(const float eye_fov, const float aspect_ratio, const float zNear, const float zFar){
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

bool Renderer::RenderWireModel(cv::Scalar color){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"Rendering wire ...\n";

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

bool Renderer::RenderModel(){

    unsigned int z_buffer_size = canvas_width_ * canvas_height_;
    for(int i = 0; i <  z_buffer_size; i++){
        z_buffer_[i] = z_far_;
    };
    for(int index = 0; index < surface_size_; index++){
        vector<Vec3i>& indexes = surfaces_[index];
        Vec3f vertex[3]{
            vertices_[indexes[0].vertex],
            vertices_[indexes[1].vertex],
            vertices_[indexes[2].vertex],
        };
        Vec2f uv[3]{
            textures_[indexes[0].uv],         
            textures_[indexes[1].uv],    
            textures_[indexes[2].uv]
        };
        RenderTriangles(vertex, uv);
        if(is_showing_rendering){
            cv::imshow("Rendering", canvas_);
            cv::waitKey(1);
        }
    }
    return true;
}

bool Renderer::Draw2DLine(Vec2i p1, Vec2i p2, const cv::Scalar& color){
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
            canvas_.at<cv::Scalar>(x, y) = color;            
        }
        else{
            canvas_.at<cv::Scalar>(y, x) = color;            
        }
        error += derror;
        if(error > dx){
            y += p2.y > p1.y ? 1 : -1;
            error -= dx * 2;
        }
    }

    return true;
}

bool Renderer::RenderTriangles(Vec3f* vertex, Vec2f* uv){
    Vec2f bbox[2];
    FindBoundingBox(vertex, bbox);
    int max_x = static_cast<int>(bbox[1].x) + 1;
    int max_y = static_cast<int>(bbox[1].y) + 1;
    for(int y = bbox[0].y; y < max_y; y++){
        for(int x = bbox[0].x; x < max_x; x++){
            if(IsInsideTriangle(vertex, Vec2f(x+0.5, y+0.5))){
                Vec3f barycentric = BarycentricInterpolation(vertex, Vec2f(x+0.5, y+0.5));
                float pixel_z = barycentric.x * vertex[0].z + barycentric.y * vertex[1].z + barycentric.z * vertex[2].z;
                if(pixel_z <= z_buffer_[y * canvas_width_+ x]){
                    z_buffer_[y*canvas_width_+x] = pixel_z;
                    Vec2f uv_interpolated(uv[0] * barycentric.x + uv[1] * barycentric.y + uv[2] * barycentric.z);
                    Vec3f color = model_ptr_->getColor(uv_interpolated.x, uv_interpolated.y);
                    canvas_.at<cv::Scalar>(y, x) = cv::Scalar(color.z / 255.0f, color.y / 255.0f, color.x / 255.0f);
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
    float alpha = (pixel-v1).cross(v2-v1) / (v2 - v1).cross(v0-v1);
    float beta = (pixel-v2).cross(v0-v2) / (v0-v2).cross(v1-v2);
    float gamma = (pixel-v0).cross(v1-v0) / (v2 - v0).cross(v1-v0);
    return Vec3f(abs(alpha), abs(beta), abs(gamma)); 
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