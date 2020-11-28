#include"renderer.h"
namespace YeahooQAQ{

Renderer::Renderer(unsigned int width, unsigned int height, Scalar& background_color)
    :
    canvas_width_(width),
    canvas_height_(height),
    background_color_(background_color),
    canvas_(height, width, CV_64FC4, background_color),
    time_per_tick_(1.0/getTickFrequency()),
    is_clock_running_(false),
    start_time_(0.0),
    end_time_(0.0),
    duration_(0.0)
{}

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

void Renderer::DrawLine(Point2i p1, Point2i p2, Scalar& color){
    bool reverse = false;
    if(p1.x > p2.x){
        swap(p1, p2);
    }
    if(abs(p1.x - p2.x) < abs(p1.y - p2.y)){
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        reverse = true;
    }

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int x2 = p2.x;
    int y =  p1.y;
    int derror = abs(dy) * 2;
    int error = 0;

    for(int x1 = p1.x; x1 < x2; x1 += 1){ 
        if(reverse){
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

}


}