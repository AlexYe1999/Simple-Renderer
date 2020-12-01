#ifndef RENDERER_H_
#define RENDERER_H_
#include<string>
#include<opencv2/opencv.hpp>
#include"../model/model.h"

namespace YeahooQAQ{
using namespace std;

class Renderer{
public:
    Renderer(unsigned int width, unsigned int height, cv::Scalar& background_color);
    ~Renderer();

public:
    void StartClock();
    void StopClock();
    void ClearTimeCounter();
    void GetTimeCost();
    
public:
    void ShowImage(string window_name);
    void SaveImage(string filename);
    void ClearImage(){
        canvas_ = cv::Mat(canvas_height_, canvas_width_, CV_64FC4, background_color_);
    }

public:
    bool LoadModel(string filename);
    bool ShowWireModel(const cv::Scalar& color, const float max_size);
    bool ShowFlatModel(const cv::Scalar& color, const float max_size);

public:
    bool Draw2DLine(Vec2i p1, Vec2i p2, const  cv::Scalar& color);
    bool Draw2DRectangle(const Vec3f vertex[3], const cv::Scalar& color);

private:
    void FindBoundingBox(const Vec3f vertex[3], Vec2f bbox[2]);
    bool IsInsideTriangle(const Vec3f vertex[3], const Vec2f& pixel);
    Vec3f BarycentricInterpolation(const Vec3f vertex[3]);

private:
    void SetModelMatrix();
    void SetViewMatrix();
    void SetProjectionMatrix();

private:
    const  double time_per_tick_;
    bool is_clock_running_;
    int64 start_time_;
    int64 end_time_;
    int64 duration_;

private: 
    const unsigned int canvas_width_;
    const unsigned int canvas_height_;
    cv::Scalar background_color_;
    cv::Mat canvas_;

private:
    Model* model_ptr_;
    float* z_buffer_;
};

}

#endif