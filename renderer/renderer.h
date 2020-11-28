#ifndef RENDERER_H_
#define RENDERER_H_
#include<string>
#include<opencv2/opencv.hpp>

namespace YeahooQAQ{

using namespace cv;
using namespace std;

class Renderer{
public:
    Renderer(unsigned int width, unsigned int height, Scalar& background_color);
    ~Renderer(){};

public:
    void ShowImage(string window_name);
    void SaveImage(string filename);
    void ClearImage(){
        canvas_ = Mat(canvas_height_, canvas_width_, CV_64FC4, background_color_);
    }

public:
    void StartClock(){
        if(!is_clock_running_){
            is_clock_running_ = true;
            start_time_ = getTickCount();            
        }
    }
    void StopClock(){
        if(is_clock_running_){
            is_clock_running_ = false;
            end_time_ = getTickCount();
            duration_ += end_time_ - start_time_;
        }
    }
    void ClearTimeCounter(){
        if(is_clock_running_){
            StopClock();
        }
        duration_ = 0;
    }
    void GetTimeCost(){
        if(is_clock_running_){
                StopClock();
                StartClock();
        }
        cout<<"time cost: "<<duration_ * time_per_tick_ *1000<<" ms\n";
    }

public:
    void DrawLine(Point2i p1, Point2i p2, Scalar& color);

private:
    const  double time_per_tick_;
    bool is_clock_running_;
    int64 start_time_;
    int64 end_time_;
    int64 duration_;

private: 
    const unsigned int canvas_width_;
    const unsigned int canvas_height_;
    Scalar background_color_;
    Mat canvas_;

};

}

#endif