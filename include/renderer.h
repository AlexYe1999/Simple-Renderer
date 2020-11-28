#ifndef RENDERER_H_
#define RENDERER_H_
#include<string>
#include<opencv2/opencv.hpp>

namespace YeahooQAQ{

using namespace cv;
using std::string;

class Renderer{
public:
    Renderer(unsigned int width, unsigned int height, Scalar scalar);
    ~Renderer();

public:
    void ShowImage(string window_name);
    void SaveImage(string filename);

public:
    void StartClock(){
        start_time_ = getTickCount();
    }
    void StopClock(){
        end_time_ = getTickCount();
        duration_ += end_time_ - start_time_;
    }
    void ClearTimeCounter(){
        duration_ = 0;
    }
    double GetTimeCost() const{
        return duration_ * time_per_tick_;
    }

private:
    const  double time_per_tick_;
    int64 start_time_;
    int64 end_time_;
    int64 duration_;

private: 
    const unsigned int canvas_width_;
    const unsigned int canvas_height_;
    Mat canvas_;

};

}

#endif