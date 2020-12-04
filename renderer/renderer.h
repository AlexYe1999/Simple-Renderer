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
    void ShowImage(string window_name, const unsigned short delay_ms);
    void SaveImage(string filename);
    void ClearCanvas(){
        canvas_ = cv::Mat(canvas_height_, canvas_width_, CV_64FC4, background_color_);
    }

public:
    bool LoadModel(string filename);

public:
    void SetModelMatrix(const float x_axis, const float y_axis, const float z_axis);
    void SetViewMatrix(const Vec3f& eye_pos);
    void SetProjectionMatrix(const float eye_fov, 
                                                        const float aspect_ratio,
                                                        const float zNear, float zFar);

public:
    bool RenderPointModel(const  cv::Scalar& color,  const float max_size);
    bool RenderWireModel();
    bool RenderModel();

public:
    bool Draw2DLine(Vec2i p1, Vec2i p2, const  cv::Scalar& color);
    bool Draw2DTriangle(const Vec3f vertex[3], const cv::Scalar& color);

private:
    void FindBoundingBox(const Vec3f vertex[3], Vec2f bbox[2]);
    bool IsInsideTriangle(const Vec3f vertex[3], const Vec2f& pixel);
    Vec3f BarycentricInterpolation(const Vec3f vertex[3], const Vec2f& pixel);

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

private:
    float* z_buffer_;
    vector<vector<Vec3f>> surfaces_;
    vector<Vec3f> vertices_;
    vector<Vec3f> normals_;
    vector<cv::Scalar> colors_;    
    vector<Vec2f> textures_;


private:
    float zNear_;
    float zFar_;
    Matrix4f model_matrix_;
    Matrix4f view_matrix_;
    Matrix4f projection_matrix_;

};

}

#endif