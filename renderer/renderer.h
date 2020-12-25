#ifndef RENDERER_H_
#define RENDERER_H_
#include"../model/model.h"
#include"../shader/shader.h"
#include"../geometry/shape.h"
#include"../texture/texture.h"
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
    void ClearCanvas();

public:
    bool Rendering(const ShaderType& shader_type);

public:
    void MSAA(const bool& is_open){is_MSAA_open_ = is_open;}
    void ShowProcessing(bool is_open){is_showing_rendering = is_open;}
    bool RenderModelVerties(const bool& is_render_verties){is_render_verties_ = is_render_verties;}
    bool RenderModelEdges(const bool& is_render_edges){ is_render_edges_ = is_render_edges;}
    bool RenderModelNormals(const bool& is_render_normals){is_render_normals_ = is_render_normals;}
    bool RenderModel(const bool& is_render_models){is_render_models_ = is_render_models;}

public:
    bool LoadModel(const string& filename, const string& texture_name);
    bool LoadPoint(const Point& point);
    bool LoadPoint(const vector<Point>& points);
    bool LoadLine(const Line& line);
    bool LoadLine(const vector<Line>& lines);
    bool LoadPointLights(const vector<Light>& lights);

public:
    bool MvpTransforme();
    void SetModelMatrix(const float& x_axis, const float& y_axis, const float& z_axis);
    void SetViewMatrix(const Vec3f& eye_pos);
    void SetProjectionMatrix(const float& eye_fov, 
                                                        const float& aspect_ratio,
                                                        const float& zNear, const float& zFar);

public:
    bool SetPixel(const Vec2i& pos, const Vec3f& color);
    bool Draw2DLine(Vec2i p1, Vec2i p2, const  Vec3f& color);
    bool DrawLine(Vec3f p1, Vec3f p2, const  Vec3f& color);
    bool RenderTriangles(Vec3f* vertex, Vec3f* normals, Vec2f* uv, Texture* texture_ptr);

private:
    Vec3f BarycentricInterpolation(const Vec3f vertex[3], const Vec2f& pixel);
    void FindBoundingBox(const Vec3f vertex[3], Vec2f bbox[2]);
    bool IsInsideTriangle(const Vec3f vertex[3], const Vec2f& pixel);

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
    float z_near_;
    float z_far_;
    Matrix4f model_matrix_;
    Matrix4f view_matrix_;
    Matrix4f projection_matrix_;

private:
    vector<Point> points_;
    vector<Line> lines_;
    vector<Triangle> triangles_;
    vector<Light> point_lights_;
    vector<Texture*> textures_ptrs_;
    Shader shader_;

};

}

#endif