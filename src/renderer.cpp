#include"../include/renderer.h"
namespace YeahooQAQ{

Renderer::Renderer(unsigned int height, unsigned int width, Scalar background)
    :
    canvas_width_(width),
    canvas_height_(height),
    canvas_(width, height, CV_64FC4, background),
    time_per_tick_(1.0/getTickFrequency()),
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
    std::cout<<"Push s to save image";
    int k = waitKey(0);
    if(k == 's'){
        if(imwrite(filename, canvas_)){
            std::cout<<"Save the image\n";
        }
        else{
            std::cout<<"Save the image failed\n";
        }
    }
    else{
            std::cout<<"quit\n";
    }
}

}