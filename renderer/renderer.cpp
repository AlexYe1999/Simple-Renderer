#include"renderer.h"
namespace YeahooQAQ{

Renderer::Renderer(unsigned int width, unsigned int height, Scalar& background_color)
    :
    time_per_tick_(1.0/getTickFrequency()),
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
        start_time_ = getTickCount();            
    }
}

void Renderer::StopClock(){
    if(is_clock_running_){
        is_clock_running_ = false;
        end_time_ = getTickCount();
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
    cout<<"time cost: "<<duration_ * time_per_tick_ *1000<<" ms\n";
}

void Renderer::ShowImage(std::string window_name){
    StopClock();
    namedWindow(window_name);
    imshow(window_name, canvas_);
    waitKey(2000);
    StartClock();
}

void Renderer::SaveImage(std::string filename){
    namedWindow("OutputImage");
    imshow("OutputImage", canvas_);
    cout<<"Push s to save image\n";
    int k = waitKey(0);
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

bool Renderer::Show2DModel(const Scalar& color){
    if(model_ptr_ == nullptr){
        cerr << "Model heven't been loaded\n";
        return false;
    }
    cout<<"rendering model\n";
    const int surfaces_num = model_ptr_->GetSurfeceSize();
    for(int index = 0; index < surfaces_num; index++){
        vector<int> indexes = model_ptr_->GetSurfece(index);
        const int index_num = indexes.size();
        for(int i = 0; i < index_num; i++){
            Vec3f v1 = model_ptr_->GetVertex(indexes[i]);
            Vec3f v2 = model_ptr_->GetVertex(indexes[(i+1)%index_num]);
            int x1 = (v1.x * 0.9f + 1.0f) * canvas_width_ * 0.5f;
            int y1 = (-v1.y * 0.9f + 1.0f) * canvas_height_ * 0.5f;
            int x2 = (v2.x  * 0.9f + 1.0f) * canvas_width_ * 0.5f;
            int y2 = (-v2.y  * 0.9f + 1.0f) * canvas_height_ * 0.5f;
            DrawLine(Point2i(x1, y1), Point2i(x2, y2), color);
        }

    }

    return true;
}


bool Renderer::DrawLine(Point2i p1, Point2i p2, const Scalar& color){
    bool is_reverse = false;
    if(p1.x > p2.x){
        swap(p1, p2);
    }
    if(abs(p1.x - p2.x) < abs(p1.y - p2.y)){
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        is_reverse = true;
    }

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int x2 = p2.x;
    int y =  p1.y;
    int derror = abs(dy) * 2;
    int error = 0;

    for(int x1 = p1.x; x1 < x2; x1 += 1){ 
        if(is_reverse){
            canvas_.at<Scalar>(x1, y) = color;            
        }
        else{
            canvas_.at<Scalar>(y, x1) = color;            
        }
        error += derror;
        if(error > dx){
            y += p2.y > p1.y ? 1 : -1;
            error -= dx*2;
        }
    }

    return true;
}


}