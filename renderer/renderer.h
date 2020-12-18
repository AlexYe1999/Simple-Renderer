#ifndef RENDERER_H_
#define RENDERER_H_
#include"../model/model.h"
#include"../shader/shader.h"
#include<string>
#include<opencv2/opencv.hpp>

namespace YeahooQAQ{
using namespace std;

class Renderer{
public:
    Renderer(const unsigned int& width, const unsigned int& height, const Vec3f& background_color);
    ~Renderer();

public:
    void StartClock();
    void StopClock();
    void ClearTimeCounter();
    void GetTimeCost();
    
public:
    void ShowImage(string window_name, const unsigned short delay_ms);
    void SaveImage(const std::string& filename);
    void ShowProcessing(bool is_open){is_showing_rendering = is_open;}
    void MSAA(const bool& is_open){is_MSAA_open_ = is_open;}
    void ClearCanvas(){
        canvas_ = cv::Mat(canvas_height_, canvas_width_, CV_64FC4,
                                cv::Scalar(background_color_.z, background_color_.y, background_color_.x));
    }

public:
    bool LoadModel(const string& filename, const string& texture_name);
    bool LoadSets(const vector<Light>& lights);

public:
    bool MvpTransforme();
    void SetModelMatrix(const float& x_axis, const float& y_axis, const float& z_axis);
    void SetViewMatrix(const Vec3f& eye_pos);
    void SetProjectionMatrix(const float& eye_fov, 
                                                        const float& aspect_ratio,
                                                        const float& zNear, const float& zFar);

public:
    bool RenderPointModel();
    bool RenderWireEdge(const Vec3f& color);
    bool RenderNormal(const Vec3f& color);
    bool RenderModel(const ShaderType& shader_type);

public:
    bool DrawLine(Vec3f p1, Vec3f p2, const  Vec3f& color);
    bool Draw2DLine(Vec2i p1, Vec2i p2, const  Vec3f& color);
    bool RenderTriangles(Vec3f* vertex, Vec3f* normals, Vec2f* uv);

private:
    void FindBoundingBox(const Vec3f vertex[3], Vec2f bbox[2]);
    bool IsInsideTriangle(const Vec3f vertex[3], const Vec2f& pixel);
    Vec3f BarycentricInterpolation(const Vec3f vertex[3], const Vec2f& pixel);

private:
    const  double time_per_tick_;
    bool is_clock_running_;
    bool is_showing_rendering;
    bool is_MSAA_open_;
    int64 start_time_;
    int64 end_time_;
    int64 duration_;

private: 
    const unsigned int canvas_width_;
    const unsigned int canvas_height_;
    Vec3f background_color_;
    cv::Mat canvas_;

private:
    float* z_buffer_;
    unsigned int surface_size_;
    unsigned int vertex_size_;
    unsigned int normal_size_;
    unsigned int texture_size_;
    Model* model_ptr_;
    vector<vector<Vec3i>> surfaces_;
    vector<Vec3f> vertices_;
    vector<Vec3f> normals_;
    vector<Vec2f> textures_;

private:
    float z_near_;
    float z_far_;
    Matrix4f model_matrix_;
    Matrix4f view_matrix_;
    Matrix4f projection_matrix_;

private:
    vector<Light> lights_;
    Shader shader_;

};

}

#endif