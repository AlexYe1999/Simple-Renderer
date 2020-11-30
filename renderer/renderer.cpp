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
    model_ptr_(nullptr)
{}

Renderer::~Renderer(){
    if(model_ptr_ != nullptr){
        delete model_ptr_;
        model_ptr_ = nullptr;
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

void Renderer::ShowImage(std::string window_name){
    StopClock();
    cv::namedWindow(window_name);
    imshow(window_name, canvas_);
    cv::waitKey(2000);
    StartClock();
}

void Renderer::SaveImage(std::string filename){
    cv::namedWindow("OutputImage");
    cv::imshow("OutputImage", canvas_);
    cout<<"Push s to save image\n";
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

    return true;
}

bool Renderer::ShowWireModel(const cv::Scalar& color, const float max_size){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"rendering model...\n";
    const int surfaces_num = model_ptr_->GetSurfeceSize();
    for(int index = 0; index < surfaces_num; index++){
        vector<int> indexes = model_ptr_->GetSurfece(index);
        for(int i = 0; i < 3; i++){
            Vec3f v1 = model_ptr_->GetVertex(indexes[i]);
            Vec3f v2 = model_ptr_->GetVertex(indexes[(i+1)%3]);
            int x1 = (v1.x * max_size + max_size) * canvas_width_ * 0.5f;
            int y1 = (-v1.y * max_size + max_size) * canvas_height_ * 0.5f;
            int x2 = (v2.x * max_size + max_size) * canvas_width_ * 0.5f;
            int y2 = (-v2.y * max_size + max_size) * canvas_height_ * 0.5f;
            Draw2DLine(Vec2i(x1, y1), Vec2i(x2, y2), color);
        }

    }

    return true;
}


bool Renderer::ShowFlatModel(const cv::Scalar& color, const float max_size){



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

bool Renderer::Draw2DRectangle(const Vec2f vertex[3], const cv::Scalar& color){

    Vec2f bbox[2]; 
    FindBoundingBox(vertex, bbox);
    int max_x = static_cast<int>(bbox[1].x) + 1;
    int max_y = static_cast<int>(bbox[1].y) + 1;
    for(int y = bbox[0].y; y < max_y; y++){
        for(int x = bbox[0].x; x < max_x; x++){
            if(IsInsideTriangle(vertex, Vec2f(x, y))){ 
                canvas_.at<cv::Scalar>(y, x) = color;       
            }
        }
    }

    return true;
}

Vec3f Renderer::BarycentricInterpolation(const Vec2f vec[3]){


}

void Renderer::FindBoundingBox(const Vec2f vertex[3], Vec2f bbox[2]){
    bbox[0].x = 0x7F800000;
    bbox[0].y = 0x7F800000;
    bbox[1].x = -1.0f;
    bbox[1].y = -1.0f;

    for(int i = 0; i < 3; i++ ){
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
    cout<<"Bounding box: ("<<bbox[0].x<<" , "<<bbox[0].y<<") ("<<bbox[1].x<< " , " <<bbox[1].y<<")\n";
}

bool Renderer::IsInsideTriangle(const Vec2f vertex[3], const Vec2f& pixel){
    int count = 0;
    for(int i = 0; i < 3; i++){
        Vec2f v1 = vertex[(i+1)%3] - vertex[i];
        Vec2f v2 = pixel - vertex[i];
        float dot = v1.cross(v2);
        if(dot > 0.0f) count++;
    }
    if(count == 3 || count == 0){
        return true;
    }
    return false;
}

}