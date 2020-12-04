#include"renderer.h"
namespace YeahooQAQ{

Renderer::Renderer(unsigned int width, unsigned int height, cv::Scalar& background_color)
    :
    time_per_tick_(1.0 / cv::getTickFrequency()),
    is_clock_running_(false),
    start_time_(0.0),
    end_time_(0.0),
    duration_(0.0),
    canvas_width_(width),
    canvas_height_(height),
    background_color_(background_color),
    canvas_(height, width, CV_64FC4, background_color),
    model_ptr_(nullptr),
    z_buffer_(nullptr),
    surfaces_(), 
    vertices_(),
    normals_(), 
    colors_(),
    textures_(),
    zNear_(0.0f),
    zFar_(0.0f),
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
    cv::namedWindow(window_name);
    imshow(window_name, canvas_);
    cv::waitKey(delay_ms);
    StartClock();
}

void Renderer::SaveImage(std::string filename){
    cv::namedWindow("OutputImage");
    cv::imshow("OutputImage", canvas_);
    cout<<"\nPush s to save image\n";
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

bool Renderer::LoadModel(string filename){
    if(model_ptr_ == nullptr){
        model_ptr_ = new Model(filename);
    }
    else{
        cerr <<"Model has been loaded\n";
        return false;
    }
    unsigned int surface_size = model_ptr_->GetSurfeceSize();
    surfaces_.resize(surface_size * 3);
    vertices_.resize(surface_size * 3);
    normals_.resize(surface_size * 3);
    colors_.resize(surface_size * 3);
    textures_.resize(surface_size * 3);

    return true;
}
void Renderer::SetModelMatrix(const float x_axis, const float y_axis, const float z_axis){
    float theta_x = x_axis * 3.1415926 / 180.0f;
    float theta_y = z_axis * 3.1415926 / 180.0f;
    Matrix4f model_matrix_x = {
        {1.0f, 0.0f,                    0.0f,                  0.0f},
        {0.0f, cos(theta_x),  sin(theta_x), 0.0f},
        {0.0f, -sin(theta_x), cos(theta_x), 0.0f},
        {0.0f, 0.0f,                    0.0f,                   1.0f}
    };
    Matrix4f model_matrix_y = {
        {cos(theta_y), 0.0f,sin(theta_y), 0.0f},
        {0.0f,                   1.0f, 0.0f,                    0.0f},
        {-sin(theta_y),0.0f, cos(theta_y),  0.0f},
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
    float theta = eye_fov * 3.1415926 / 360.0f;
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

bool Renderer::RenderPointModel(const  cv::Scalar& color,  const float max_size){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"rendering point ...\n";
    Vec3f fix(max_size, max_size, max_size);
    const int surfaces_num = model_ptr_->GetSurfeceSize();
    for(int index = 0; index < surfaces_num; index++){
        vector<Vec3f> indexes = model_ptr_->GetSurfece(index);
        Vec3f vertex[3];
        vertex[0] = (model_ptr_->GetVertex(indexes[0].vertex) + fix) * canvas_width_ * 0.5f;
        vertex[1]  = (model_ptr_->GetVertex(indexes[1].vertex) + fix) * canvas_width_ * 0.5f;
        vertex[2] =  (model_ptr_->GetVertex(indexes[2].vertex) + fix) * canvas_width_ * 0.5f;
        canvas_.at<cv::Scalar>(vertex[0].y, vertex[0].x) = cv::Scalar(1.0f, 0.0f, 0.0f);         
        canvas_.at<cv::Scalar>(vertex[1].y, vertex[1].x) = cv::Scalar(0.0f, 1.0f, 0.0f);         
        canvas_.at<cv::Scalar>(vertex[2].y, vertex[2].x) = cv::Scalar(0.0f, 0.0f, 1.0f);         
        cv::Mat flip;
        cv::flip(canvas_, flip, 0);
        cv::imshow("Processing", flip);
        cv::waitKey(1);
    }

}

bool Renderer::RenderWireModel(){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"rendering wire ...\n";
    float f1 = (zFar_ - zNear_) * 0.5f;
    float f2 = (zFar_ + zNear_) * 0.5f;
    Matrix4f mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
    const int surface_num = model_ptr_->GetSurfeceSize();
    for(int index = 0; index < surface_num; index++){
        vector<Vec3f> surface = model_ptr_->GetSurfece(index);
        surfaces_.push_back(surface);
        Vec4f v4[3] ={
            mvp_matrix * model_ptr_->GetVertex(surface[0].vertex).toVec4(1.0f),
            mvp_matrix * model_ptr_->GetVertex(surface[1].vertex).toVec4(1.0f),
            mvp_matrix * model_ptr_->GetVertex(surface[2].vertex).toVec4(1.0f)
        } ;
        Vec3f vertex[3];
        for(int i = 0; i < 3; i++){
            v4[i] /= v4[i].w;
            vertex[i] = {
                0.5f * canvas_width_ * (v4[i].x + 1.0f),
                0.5f * canvas_height_ * (v4[i].y + 1.0f),
                v4[i].z * f1 + f2
            };
            vertices_[surface[i].vertex] = vertex[i];
        }

        for(int i = 0; i < 3; i++){
            switch(i){
                case 0:{
                    colors_[surface[i].vertex] = cv::Scalar(1.0f, 0.0f, 0.0f);
                    break;
                }
                case 1:{
                    colors_[surface[i].vertex] = cv::Scalar(0.0f, 1.0f, 0.0f);
                    break;
                }
                case 2:{
                    colors_[surface[i].vertex] = cv::Scalar(0.0f, 0.0f, 1.0f);
                    break;
                }
                default:{
                    colors_[surface[i].vertex] = cv::Scalar(1.0f, 1.0f, 1.0f);
                    break;
                }
            }
            Draw2DLine(Vec2i(vertex[i].x, vertex[i].y), Vec2i(vertex[(i+1)%3].x, vertex[(i+1)%3].y), colors_[surface[i].vertex]);
        }
    }

    return true;
}

bool Renderer::RenderModel(){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"rendering model ...\n";

    float f1 = (zFar_ - zNear_) * 0.5f;
    float f2 = (zFar_ + zNear_) * 0.5f;
    Matrix4f mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
    const int surface_num = model_ptr_->GetSurfeceSize();
    for(int index = 0; index < surface_num; index++){
        vector<Vec3f> surface = model_ptr_->GetSurfece(index);
        surfaces_.push_back(surface);
        Vec4f v4[3] ={
            mvp_matrix * model_ptr_->GetVertex(surface[0].vertex).toVec4(1.0f),
            mvp_matrix * model_ptr_->GetVertex(surface[1].vertex).toVec4(1.0f),
            mvp_matrix * model_ptr_->GetVertex(surface[2].vertex).toVec4(1.0f)
        } ;
        Vec3f vertex[3];
        for(int i = 0; i < 3; i++){
            v4[i] /= v4[i].w;
            vertex[i] = {
                0.5f * canvas_width_ * (v4[i].x + 1.0f),
                0.5f * canvas_height_ * (v4[i].y + 1.0f),
                v4[i].z * f1 + f2
            };
            vertices_[surface[i].vertex] = vertex[i];
            switch(i){
                case 0:{
                    colors_[surface[i].vertex] = cv::Scalar(1.0f, 0.0f, 0.0f);
                    break;
                }
                case 1:{
                    colors_[surface[i].vertex] = cv::Scalar(0.0f, 1.0f, 0.0f);
                    break;
                }
                case 2:{
                    colors_[surface[i].vertex] = cv::Scalar(0.0f, 0.0f, 1.0f);
                    break;
                }
                default:{
                    colors_[surface[i].vertex] = cv::Scalar(1.0f, 1.0f, 1.0f);
                    break;
                }
            }
        }
        Draw2DTriangle(vertex , cv::Scalar(rand()%100 * 0.01, rand()%100 * 0.01, rand()%100 * 0.01));
    }

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

bool Renderer::Draw2DTriangle(const Vec3f vertex[3], const cv::Scalar& color){
    
    Vec2f bbox[2]; 
    FindBoundingBox(vertex, bbox);
    int max_x = static_cast<int>(bbox[1].x) + 1;
    int max_y = static_cast<int>(bbox[1].y) + 1;
    for(int y = bbox[0].y; y < max_y; y++){
        for(int x = bbox[0].x; x < max_x; x++){
            if(IsInsideTriangle(vertex, Vec2f(x+0.5, y+0.5))
                && x >= 0 && y >= 0
                && x<canvas_width_ && y<canvas_height_){
                BarycentricInterpolation(vertex, Vec2f(x+0.5, y+0.5));
                canvas_.at<cv::Scalar>(y, x) = color;
            }
        }
    }
    return true;
}

Vec3f Renderer::BarycentricInterpolation(const Vec3f vec[3],  const Vec2f& pixel){


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