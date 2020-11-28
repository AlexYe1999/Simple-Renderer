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
    void StartClock();
    void StopClock();
    void ClearTimeCounter();
    void GetTimeCost();
    
public:
    void ShowImage(string window_name);
    void SaveImage(string filename);
    void ClearImage(){
        canvas_ = Mat(canvas_height_, canvas_width_, CV_64FC4, background_color_);
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