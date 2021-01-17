#ifndef RENDERER_H_
#define RENDERER_H_
#include"../model/model.h"
#include"../shader/shader.h"
#include"../geometry/shape.h"
#include"../texture/texture.h"
#include"../hitable_list/hitable_list.hpp"
#include<string>
#include<opencv2/opencv.hpp>

namespace LemonCube{
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
    void ClearCanvas();

public:
    bool Rendering();
    bool RayTracing(const unsigned int& bounce_times = 20);

public:
    void MSAA(const bool& is_open, const unsigned int sample_rate = 1){is_MSAA_open_ = is_open; sample_rate_ = sample_rate;}
    void ShowProcessing(bool is_open){is_showing_rendering = is_open;}
    bool RenderModelVerties(const bool& is_render_verties){is_render_verties_ = is_render_verties; return true;}
    bool RenderModelEdges(const bool& is_render_edges){ is_render_edges_ = is_render_edges;return true;}
    bool RenderModelNormals(const bool& is_render_normals){is_render_normals_ = is_render_normals;return true;}
    bool RenderModel(const bool& is_render_models){is_render_models_ = is_render_models;return true;}

public:
    bool SetShader(const shared_ptr<IShader>& shader);

public:
    bool LoadModel(const string& filename, const string& texture_name);
    bool LoadPoint(const Point& point);
    bool LoadPoint(const vector<Point>& points);
    bool LoadLine(const Line& line);
    bool LoadLine(const vector<Line>& lines);
    bool ClearLine();
    bool LoadTriangle(const Triangle& Triangle);    
    bool LoadTriangle(const vector<Triangle>& Triangles);
    bool LoadRectangle(const array<Vec3f,4>& vertices, const array<Vec3f,4>& normals);
    bool LoadRectangle(const vector<array<Vec3f,4>>& rectangles);
    bool LoadLightSource(const vector<LightSource>& lights);
    bool LoadObjectPtr(const vector<shared_ptr<Hitable>>& obj_ptrs);

public:
    bool MvpTransforme();
    void SetModelMatrix(const float& x_axis, const float& y_axis, const float& z_axis);
    void SetViewMatrix(const Vec3f& eye_pos);
    void SetProjectionMatrix(const float& eye_fov, const float& aspect_ratio, const float& zNear, const float& zFar);

public:
    bool SetPixel(const unsigned int& x, const unsigned int& y, const Vec3f& color);
    bool Draw2DLine(Vec2i p1, Vec2i p2, const  Vec3f& color1, const Vec3f& color2);
    bool DrawLine(Vec3f p1, Vec3f p2, const  Vec3f& color1, const Vec3f& color2);
    bool RenderTriangles(const Triangle& triangle);

private:
    void FindBoundingBox(const array<Vec3f, 3>& vertices, Vec2f bbox[2]);
    bool IsInsideTriangle(const array<Vec3f, 3>& vertices, const Vec2f& pixel);
    inline Vec3f BarycentricInterpolation(const array<Vec3f, 3>& vertex, const Vec2f& pixel);
    inline Vec3f GetRayVector(const float& x, const float& y);
    Vec3f GetRayColor(const Ray& ray, const unsigned int step = 10);

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
    Vec3f* frame_buffer_;
    float* z_buffer_;

private:
    bool is_render_verties_;
    bool is_render_edges_;
    bool is_render_normals_;
    bool is_render_models_;

private:
    float eye_fov_;
    float aspect_ratio_;
    float view_port_width_half_;
    float view_port_height_half_;
    float z_near_;
    float z_far_;
    Vec3f eye_pos_;
    Matrix4f model_matrix_;
    Matrix4f view_matrix_;
    Matrix4f projection_matrix_;

private:
    vector<Point> points_;
    vector<Line> lines_;
    vector<Triangle> triangles_;
    vector<LightSource> lights_;
    shared_ptr<IShader> shader_ptr_;

private:
    unsigned int sample_rate_;
    HitableList hitable_list_;
    Matrix3f view_port_cord_;

};

}

#endif