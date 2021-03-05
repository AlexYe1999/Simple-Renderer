#pragma once
#include"../model/model.h"
#include"../shader/shader.hpp"
#include"../geometry/shape.h"
#include"../texture/texture.h"
#include"../hitable_list/hitable_list.hpp"
#include<string>
#include<opencv2/opencv.hpp>

namespace LemonCube{
using namespace std;
template<typename T>
class Renderer{
public:
    Renderer(const unsigned int& width, const unsigned int& height, const Vec3<T>& background_color);
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
    bool SetShader(const shared_ptr<IShader<T>>& shader);

public:
    bool LoadModel(const string& filename, const string& texture_name);
    bool LoadPoint(const Point<T>& point);
    bool LoadPoint(const vector<Point<T>>& points);
    bool LoadLine(const Line<T>& line);
    bool LoadLine(const vector<Line<T>>& lines);
    bool ClearLine();
    bool LoadTriangle(const Triangle<T>& Triangle);    
    bool LoadTriangle(const vector<Triangle<T>>& Triangles);
    bool LoadRectangle(const array<Vec3<T>,4>& vertices, const array<Vec3<T>,4>& normals);
    bool LoadRectangle(const vector<array<Vec3<T>,4>>& rectangles);
    bool LoadLightSource(const vector<LightSource>& lights);
    bool LoadObjectPtr(const vector<shared_ptr<Hitable>>& obj_ptrs);

public:
    bool VertexShader();
    void SetModelMatrix(const T& x_axis, const T& y_axis, const T& z_axis);
    void SetViewMatrix(const Vec3<T>& eye_pos);
    void SetProjectionMatrix(const T& eye_fov, const T& aspect_ratio, const T& zNear, const T& zFar);

public:
    bool SetPixel(const unsigned int& x, const unsigned int& y, const Vec3<T>& color);
    bool Draw2DLine(Vec2i p1, Vec2i p2, const  Vec3<T>& color1, const Vec3<T>& color2);
    bool DrawLine(Vec3<T> p1, Vec3<T> p2, const  Vec3<T>& color1, const Vec3<T>& color2);
    bool RenderTriangles(const Triangle<T>& triangle);

private:
    void FindBoundingBox(const array<Vec3<T>, 3>& vertices, Vec2<T> bbox[2]);
    bool IsInsideTriangle(const array<Vec3<T>, 3>& vertices, const Vec2<T>& pixel);
    inline Vec3<T> BarycentricInterpolation(const array<Vec3<T>, 3>& vertex, const Vec2<T>& pixel);
    inline Vec3<T> GetRayVector(const T& x, const T& y);
    Vec3<T> GetRayColor(const Ray& ray, const unsigned int step = 10);

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
    Vec3<T> background_color_;
    Vec3<T>* frame_buffer_;
    T* z_buffer_;

private:
    bool is_render_verties_;
    bool is_render_edges_;
    bool is_render_normals_;
    bool is_render_models_;

private:
    T eye_fov_;
    T aspect_ratio_;
    T view_port_width_half_;
    T view_port_height_half_;
    T z_near_;
    T z_far_;
    Vec3<T> eye_pos_;
    Matrix4<T> model_matrix_;
    Matrix4<T> view_matrix_;
    Matrix4<T> projection_matrix_;

private:
    vector<Point<T>> points_;
    vector<Line<T>> lines_;
    vector<Triangle<T>> triangles_;
    vector<LightSource> lights_;
    shared_ptr<IShader<T>> shader_ptr_;

private:
    unsigned int sample_rate_;
    HitableList hitable_list_;
    Matrix3<T> view_port_cord_;

};

template<typename T>
Renderer<T>::Renderer(const unsigned int& width, const unsigned int& height, const Vec3<T>& background_color)
    :
    time_per_tick_(1.0 / cv::getTickFrequency()),
    is_clock_running_(false),
    is_showing_rendering(false),
    is_MSAA_open_(false),
    start_time_(0.0), end_time_(0.0), duration_(0.0),
    canvas_width_(width), canvas_height_(height),
    background_color_(background_color),
    frame_buffer_(nullptr),
    z_buffer_(nullptr),
    eye_pos_{0.0f,0.0f,0.0f},
    points_(),lines_(),triangles_(),
    is_render_verties_(false),
    is_render_edges_(false),
    is_render_normals_(false),
    is_render_models_(false),
    eye_fov_(0.0f),aspect_ratio_(canvas_width_ / canvas_height_),
    view_port_width_half_(0.0f), view_port_height_half_(0.0f),
    z_near_(-1.0f),z_far_(500.0f),
    model_matrix_(),view_matrix_(),projection_matrix_(),
    lights_(),
    shader_ptr_(make_shared<IShader<float>>()),
    sample_rate_(1),
    hitable_list_(),
    view_port_cord_(){
    frame_buffer_ = new Vec3<T>[width * height];
    z_buffer_ = new float[width * height];
}

template<typename T>
Renderer<T>::~Renderer(){
    if(frame_buffer_ != nullptr){
        delete[] frame_buffer_;
        frame_buffer_ = nullptr;
    }
    if(z_buffer_ != nullptr){
        delete[] z_buffer_;
        z_buffer_ = nullptr;
    }
}

template<typename T>
void Renderer<T>::StartClock(){
    if(!is_clock_running_){
        is_clock_running_ = true;
        start_time_ = cv::getTickCount();            
    }
}
template<typename T>
void Renderer<T>::StopClock(){
    if(is_clock_running_){
        is_clock_running_ = false;
        end_time_ = cv::getTickCount();
        duration_ += end_time_ - start_time_;
    }
}
template<typename T>
void Renderer<T>::ClearTimeCounter(){
    if(is_clock_running_){
        StopClock();
    }
    duration_ = 0;
}
template<typename T>
void Renderer<T>::GetTimeCost(){
    if(is_clock_running_){
            StopClock();
            StartClock();
    }
    cout<<"time cost: --"<<duration_ * time_per_tick_ *1000 <<" ms  --" << duration_ * time_per_tick_<<" s\n";
}

template<typename T>
void Renderer<T>::ShowImage(std::string window_name, const unsigned short delay_ms){
    StopClock();
    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    cv::namedWindow(window_name);
    imshow(window_name, image);
    cv::waitKey(delay_ms);
    StartClock();
}

template<typename T>
void Renderer<T>::SaveImage(const std::string& filename){
    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
    //cv::cvtColor(image, image, CV_RGB2BGR);
    cv::imshow("Rendering", image);
    cv::waitKey(3);      
    cout<<"\nPush 's to save image or push 'q' to quit\n";
    char k = cin.get();
    if(k == 's'){
        if(imwrite(filename, image*255.0f)){
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

template<typename T>
void Renderer<T>::ClearCanvas(){
    unsigned int z_buffer_size = canvas_width_ * canvas_height_;    
    for(int i = 0;i < z_buffer_size; i++){
        frame_buffer_[i] = background_color_;
        z_buffer_[i] = z_far_;
    }

}

template<typename T>
bool Renderer<T>::Rendering(){

    unsigned int lines_num = lines_.size();
    for(unsigned int index = 0; index < lines_num; index++){
        Line<T> line = lines_[index];
        DrawLine(line.vertices[0], line.vertices[1], line.colors[0], line.colors[1]);
    }

    unsigned int triangles_num = triangles_.size();
    for(unsigned int index = 0; index < triangles_num; index++){
        Triangle<T>& triangle = triangles_[index];
        Vec3<T> vertex[3]{
            triangle.vertices_camera[0],
            triangle.vertices_camera[1],
            triangle.vertices_camera[2]
        };
        if(is_render_models_ && !triangle.is_triangle_culled_){
            RenderTriangles(triangle);
        }
        if(is_render_verties_){
            Vec3<T> v0(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
            Vec3<T> v1(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
            Vec3<T> v2(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
            if(triangle.texture_ptr != nullptr){
                v0 = triangle.texture_ptr->GetColor(triangle.texture_coords[0].u, triangle.texture_coords[0].v);  
                v1 = triangle.texture_ptr->GetColor(triangle.texture_coords[1].u, triangle.texture_coords[1].v);
                v2 = triangle.texture_ptr->GetColor(triangle.texture_coords[2].u, triangle.texture_coords[2].v);
            }

            SetPixel(vertex[0].x, vertex[0].y, v0);
            SetPixel(vertex[1].x, vertex[1].y, v1);
            SetPixel(vertex[2].x, vertex[2].y, v2);
        }
        if(is_render_edges_){
            for(int i = 0; i < 3; i++){
                Vec3<T> v0(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
                Vec3<T> v1(rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f, rand() % 256 * 1.0f / 255.0f);
                if(triangle.texture_ptr != nullptr){
                    v0 = triangle.texture_ptr->GetColor(triangle.texture_coords[0].u, triangle.texture_coords[0].v);  
                    v1 = triangle.texture_ptr->GetColor(triangle.texture_coords[1].u, triangle.texture_coords[1].v);
                }
                Draw2DLine(Vec2i(vertex[i].x, vertex[i].y), Vec2i(vertex[(i+1)%3].x, vertex[(i+1)%3].y), v0, v1);
                if(is_showing_rendering){
                    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
                    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);            
                    cv::imshow("Rendering", image);
                    cv::waitKey(1);
                }
            }
        }
        if(is_render_normals_){
            Matrix4<T> mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
            Vec3<T> end[3]{
                vertex[0] + triangle.normals[0]*canvas_width_ * mvp_matrix.toMatrix3().inversed() * 0.02f,
                vertex[1] + triangle.normals[1]*canvas_height_ * mvp_matrix.toMatrix3().inversed() * 0.02f,
                vertex[2] + triangle.normals[2]*mvp_matrix.toMatrix3().inversed() * z_far_ * 0.02f
            };
            for(int i = 0; i < 3; i++){
                if(vertex[i].z - 0.01f < z_buffer_[static_cast<int>(vertex[i].y) * canvas_width_+ static_cast<int>(vertex[i].x)]){
                    DrawLine(vertex[i], end[i], ((triangle.normals[i]* mvp_matrix.toMatrix3().inversed()+Vec3<T>(1.0f,1.0f,1.0f))*0.5f).normalized(), Vec3<T>(0.0f,0.0f,0.0f));
                }
                if(is_showing_rendering){
                    cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
                    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);            
                    cv::imshow("Rendering", image);
                    cv::waitKey(1);
                }
            }
        }
        if(is_showing_rendering){
            cv::Mat image(canvas_height_, canvas_width_, CV_32FC3, frame_buffer_);
            cv::cvtColor(image, image, cv::COLOR_RGB2BGR);            
            cv::imshow("Rendering", image);
            cv::waitKey(1);
        }
    }

    return true;
}

template<typename T>
bool Renderer<T>::RayTracing(const unsigned int& bounce_times){
    
    Ray ray(eye_pos_, eye_pos_.normalized()*-1.0f);
    for(int  y = 0; y < canvas_height_; y++){
        for(int x = 0; x < canvas_width_; x++){
            if(!is_MSAA_open_){
                ray.direction = GetRayVector(x, y);
                SetPixel(x, y+1, GetRayColor(ray, bounce_times));
            }
            else{
                Vec3<T> pixel_color(0.0f,0.0f,0.0f);                
                for(unsigned int i = 0; i < sample_rate_; i++){
                    ray.direction = GetRayVector(x+(rand()%100)*0.01f, y+(rand()%100)*0.01f);
                    pixel_color += GetRayColor(ray, bounce_times);
                }             
                SetPixel(x, y+1, pixel_color / sample_rate_);                
            }
        }
        cout<<"Rendering : "<< y * 100.0f / canvas_height_ <<"% "<< endl ;
    } 

    return true;
}
template<typename T>
bool Renderer<T>::SetShader(const shared_ptr<IShader<T>>& shader){
    if(shader == nullptr){
        return false;        
    }
    else{
        shader_ptr_ = shader;
    }
    return true;
}
template<typename T>
bool Renderer<T>::LoadModel(const string& filename, const string& texture_name){

    shared_ptr<Model> model_ptr = make_shared<Model>(filename);
    int surface_size = model_ptr->GetSurfeceSize();
    Triangle<T> triangle(make_shared<Texture>(texture_name));
    for(unsigned int index = 0; index < surface_size; index++){
        array<Vec3i, 3> indexes = model_ptr->GetSurfece(index);//v uv nor
        for(int i = 0; i < 3; i++){
            triangle.vertices_world[i] = model_ptr->GetVertex(indexes[i].vertex);
            triangle.normals[i] = model_ptr->GetNormal(indexes[i].normal);            
            triangle.texture_coords[i] = model_ptr->GetTexture(indexes[i].uv);
        }
        triangles_.push_back(triangle);
    }   
    return true;
}

template<typename T>
bool Renderer<T>::LoadPoint(const Point<T>& point){
    points_.push_back(point);
    return true;
}

template<typename T>
bool Renderer<T>::LoadPoint(const vector<Point<T>>& points){
    for(Point<T> point : points){
        points_.push_back(point);
    }
    return true;
}
template<typename T>
bool Renderer<T>::LoadLine(const Line<T>& line){
    lines_.push_back(line);
    return true;
}
template<typename T>
bool Renderer<T>::LoadLine(const vector<Line<T>>& lines){
    for(Line<T> line : lines){
        lines_.push_back(line);
    }
    return true;
}

template<typename T>
bool Renderer<T>::ClearLine(){
    lines_.clear();
    return true;
}

template<typename T>
bool Renderer<T>::LoadTriangle(const Triangle<T>& triangle){
    triangles_.push_back(triangle);
    return true;
}    

template<typename T>
bool Renderer<T>::LoadTriangle(const vector<Triangle<T>>& triangles){
    for(const Triangle<T>& triangle : triangles){
        triangles_.push_back(triangle);
    }
    return true;
}
template<typename T>
bool Renderer<T>::LoadRectangle(const array<Vec3<T>,4>& vertices, const array<Vec3<T>,4>& normals){
    Triangle<T> triangle;
    for(int i = 0; i < 3; i += 2){
        for(int j = 0; j < 3; j++){
            triangle.vertices_world[j] = vertices[(i+j)%4];
            triangle.normals[j] = normals[(i+j)%4];
        }        
        triangles_.push_back(triangle);
    }
    return true;
}
template<typename T>
bool Renderer<T>::LoadRectangle(const vector<array<Vec3<T>,4>>& rectangles){
    return true;
}

template<typename T>
bool Renderer<T>::LoadLightSource(const vector<LightSource>& lights){
    shader_ptr_->SetLights(lights);
    return true;
}
template<typename T>
bool Renderer<T>::LoadObjectPtr(const vector<shared_ptr<Hitable>>& obj_ptrs){
    for(const shared_ptr<Hitable>& obj_ptr : obj_ptrs){
        hitable_list_.AddObjectPtr(obj_ptr);
    }
    return true;
}
template<typename T>
bool Renderer<T>::VertexShader(){
    view_port_cord_ = model_matrix_.toMatrix3();
    eye_pos_ = eye_pos_ * view_port_cord_;
    view_port_cord_ = view_port_cord_.transposed();
    shader_ptr_->SetEyePosition(eye_pos_);

    float f1 = (z_far_ - z_near_) * 0.5f;
    float f2 = (z_far_ + z_near_) * 0.5f;
    Matrix4<T> mvp_matrix = projection_matrix_ * view_matrix_ * model_matrix_;
    
    unsigned int triangle_num = triangles_.size();
    for(unsigned int index = 0; index < triangle_num; index++){
        Triangle<T>& triangle = triangles_[index];
        for(int i = 0; i < 3; i++){
            Vec4<T> v4 = mvp_matrix * triangle.vertices_world[i].toVec4(1.0f);
            v4 /= v4.w;
            Vec3<T> v3 = {
                0.5f * canvas_width_ * (v4.x + 1.0f),
                0.5f * canvas_height_ * (v4.y + 1.0f),
                v4.z * f1 + f2
            };
            triangle.vertices_camera[i] = v3;
        }
        Vec3<T> v1 = triangle.vertices_camera[1] - triangle.vertices_camera[0];
        Vec3<T> v2 = triangle.vertices_camera[2] - triangle.vertices_camera[1];
        triangle.is_triangle_culled_ = v1.cross(v2).z > 0.0f ? true : false;
    }

    return true;
}

template<typename T>
void Renderer<T>::SetModelMatrix(const T& x_axis, const T& y_axis, const T& z_axis){
    float theta_x = x_axis * 3.1415926535898f / 180.0f;
    float theta_y = y_axis * 3.1415926535898f / 180.0f;
    Matrix4f model_matrix_x = {
        {1.0f, 0.0f,            0.0f,           0.0f},
        {0.0f, cos(theta_x),    -sin(theta_x),  0.0f},
        {0.0f, sin(theta_x),    cos(theta_x),   0.0f},
        {0.0f, 0.0f,            0.0f,           1.0f}
    };
    Matrix4f model_matrix_y = {
        {cos(theta_y),  0.0f,   sin(theta_y),   0.0f},
        {0.0f,          1.0f,   0.0f,           0.0f},
        {-sin(theta_y), 0.0f,   cos(theta_y),   0.0f},
        {0.0f,          0.0f,   0.0f,           1.0f},
    };
    model_matrix_ = model_matrix_x * model_matrix_y;
}
template<typename T>
void Renderer<T>::SetViewMatrix(const Vec3<T>& eye_pos){
    eye_pos_ = eye_pos;
    view_matrix_ = Matrix4f{
        {1.0f,          0.0f,           0.0f,       0.0f},
        {0.0f,          1.0f,           0.0f,       0.0f},
        {0.0f,          0.0f,           1.0f,       0.0f},
        {-eye_pos.x,    -eye_pos.y,     -eye_pos.z, 1.0f}
    };
}
template<typename T>
void Renderer<T>::SetProjectionMatrix(const T& eye_fov,const T& aspect_ratio,const T& zNear, const T& zFar){
    eye_fov_ = eye_fov;
    aspect_ratio_ = aspect_ratio;
    z_near_ = zNear;
    z_far_ = zFar;
    T theta = eye_fov * 3.1415926535898f / 360.0f;
    view_port_height_half_ = atan(theta) * zNear;
    view_port_width_half_ = view_port_height_half_ * aspect_ratio_;  
    T left = -view_port_width_half_;      
    T bottom = -view_port_height_half_;

    Matrix4<T> otho1 = {
        {1.0f,                                      0.0f,                                       0.0f,                   0.0f},
        {0.0f,                                      1.0f,                                       0.0f,                   0.0f},
        {0.0f,                                      0.0f,                                       1.0f,                   0.0f},
        {(left + view_port_width_half_) * -0.5f,    (view_port_height_half_ + bottom) * -0.5f,  (zNear + zFar) * -0.5f, 1.0f}
    };

    Matrix4<T> otho2 = {
        {2.0f/(view_port_width_half_-left), 0,                                      0,                   0},
        {0,                                 2.0f/(view_port_height_half_-bottom),   0,                   0},
        {0,                                 0,                                      2.0f/(zFar-zNear),   0},
        {0,                                 0,                                      0,                   1}
    };

    Matrix4<T> perspective = {
        {zNear,    0.0f,  0.0f,            0},
        {0.0f,     zNear, 0.0f,            0},
        {0.0f,     0.0f,  zNear+zFar,   1.0f},
        {0.0f,     0.0f,  -zNear*zFar,  0.0f}
    };

    projection_matrix_ = otho2 * otho1 * perspective;
}

template<typename T>
bool Renderer<T>::SetPixel(const unsigned int& x, const unsigned int& y, const Vec3<T>& color){
    int ind = (canvas_height_- y) * canvas_width_ + x;
    frame_buffer_[ind] = color;
    return true;   
}
template<typename T>
bool Renderer<T>::Draw2DLine(Vec2i p1, Vec2i p2, const Vec3<T>& color1, const Vec3<T>& color2){
    bool is_reverse = false;
    if(abs(p1.x - p2.x) < abs(p1.y - p2.y)){
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        is_reverse = true;
    }
    
    if(p1.x > p2.x){
        swap(p1, p2);
    }

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int x_end = p2.x;
    int y =  p1.y;
    int derror = abs(dy) * 2;
    int error = 0;

    for(int x = p1.x; x < x_end; x += 1){ 
        T t = 1.0f * x / dx;
        Vec3<T> color = color2 * t + color1 * (1.0f - t);
        if(is_reverse){
            SetPixel(y, x, color);        
        }
        else{
            SetPixel(x, y, color);
        }
        error += derror;
        if(error > dx){
            y += p2.y > p1.y ? 1 : -1;
            error -= dx * 2;
        }
    }

    return true;
}

template<typename T>
bool Renderer<T>::DrawLine(Vec3<T> p1, Vec3<T> p2, const Vec3<T>& color1, const Vec3<T>& color2){
    bool is_reverse = false;
    if(abs(p1.x - p2.x) < abs(p1.y - p2.y)){
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
        is_reverse = true;
    }
    
    if(p1.x > p2.x){
        swap(p1, p2);
    }

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int x_end = p2.x;
    int y =  p1.y;
    int derror = abs(dy) * 2;
    int error = 0;
    float dz = (p2.z - p1.z) / dx;
    float z = p1.z;

    for(int x = p1.x; x < x_end; x++){ 
        float t = 1.0f * x / dx;
        Vec3<T> color = color2 * t + color1 * (1.0f - t);
        if(is_reverse){
            if(z < z_buffer_[x * canvas_width_+ y]){
                z_buffer_[x * canvas_width_+ y] = z;
                SetPixel(y, x, color);    
            }            
        }
        else{
            if(z < z_buffer_[y * canvas_width_+ x]){
                z_buffer_[y * canvas_width_+ x] = z;
                SetPixel(x, y, color);
            }
        }
        error += derror;
        z += dz;
        if(error > dx){
            y += p2.y > p1.y ? 1 : -1;
            error -= dx * 2;
        }
    }

    return true;
}
template<typename T>
bool Renderer<T>::RenderTriangles(const Triangle<T>& triangle){
    Vec2f bbox[2];
    const array<Vec3<T>, 3>& vertex = triangle.vertices_camera;
    const array<Vec3<T>, 3>& normals = triangle.normals;
    const array<Vec2f, 3>& uv = triangle.texture_coords;
    FindBoundingBox(vertex, bbox);
    int max_x = static_cast<int>(bbox[1].x)+1;
    int max_y = static_cast<int>(bbox[1].y)+1;
    for(int y = bbox[0].y; y < max_y; y++){
        for(int x = bbox[0].x; x < max_x; x++){
            float weight = 1.0f;
            if(is_MSAA_open_){
                float offset[2] = {0.25, 0.75f};
                for(int i = 0; i < 2; i++){
                    for(int j = 0 ; j < 2; j++){
                        if(!IsInsideTriangle(vertex, Vec2f(x+offset[i], y+offset[j]))){
                            weight -= 0.25f;
                        }
                    }
                }
            }
            else{
                if(!IsInsideTriangle(vertex, Vec2f(x+0.5, y+0.5))){
                    weight = 0.0f;
                }
            }
            if(weight > 0.2f){
                Vec3<T> barycentric = BarycentricInterpolation(vertex, Vec2f(x+0.5f, y+0.5f));
                float pixel_z = barycentric.x * vertex[0].z + barycentric.y * vertex[1].z + barycentric.z * vertex[2].z;
                pixel_z = pixel_z * weight + (1.0f - weight) * z_far_;
                if(pixel_z > 0.0f && pixel_z < z_buffer_[y * canvas_width_+ x]){
                    z_buffer_[y*canvas_width_+x] = pixel_z;
                    Vec2f uv_interpolated(uv[0] * barycentric.x + uv[1] * barycentric.y + uv[2] * barycentric.z);
                    Vec3<T> pos(triangle.vertices_world[0] * barycentric.x + triangle.vertices_world[1] * barycentric.y + triangle.vertices_world[2] * barycentric.z);
                    Vec3<T> normal(normals[0] * barycentric.x + normals[1] * barycentric.y + normals[2] * barycentric.z);
                    Vec3<T> color(1.0f, 1.0f, 1.0f);
                    Vec3<T> texture = triangle.texture_ptr ?
                        triangle.texture_ptr->GetColor(uv_interpolated.u, uv_interpolated.v) : color;
                    Matrix2f st = {
                        uv[0] - uv_interpolated,
                        uv[1] - uv_interpolated
                    };
                    Vec3<T> vec1 = vertex[0] - pos;
                    Vec3<T> vec2 = vertex[1] - pos;
                    st = st.inversed();
                    Matrix3f TBN;
                    TBN.vec[0] = {
                        st.vec[0].x * vec1.x + st.vec[1].x * vec2.x,
                        st.vec[0].x * vec1.y + st.vec[1].x * vec2.y,
                        st.vec[0].x * vec1.z + st.vec[1].x * vec2.z
                    };
                    TBN.vec[1] = {
                        st.vec[0].y * vec1.x+ st.vec[1].y * vec2.x,
                        st.vec[0].y * vec1.y+ st.vec[1].y * vec2.y,
                        st.vec[0].y * vec1.z+ st.vec[1].y * vec2.z
                    };
                    TBN.vec[2] = normal;
                    FragmentShaderPayload<float> payload(pos, color, texture, normal, TBN);
                    color = shader_ptr_->FragmentShader(payload);
                    SetPixel(x, y, color * weight);
                }
            }
        }
    }
    return true;
}
template<typename T>
void Renderer<T>::FindBoundingBox(const array<Vec3<T>,3>& vertex, Vec2<T> bbox[2]){
    bbox[0].x = vertex[2].x;
    bbox[0].y = vertex[2].y;
    bbox[1].x = vertex[2].x;
    bbox[1].y = vertex[2].y;

    for(int i = 0; i < 2; i++ ){
        if(vertex[i].x < bbox[0].x){
            bbox[0].x = vertex[i].x;
        }
        else if(vertex[i].x > bbox[1].x){
            bbox[1].x = vertex[i].x;
        }
        if(vertex[i].y < bbox[0].y){
            bbox[0].y = vertex[i].y;
        }
        else if(vertex[i].y > bbox[1].y){
            bbox[1].y = vertex[i].y;
        }
    }
    if(vertex[0].x < 0.0f){bbox[0].x = 0.0f;}
    if(vertex[0].y < 0.0f){bbox[0].y = 0.0f;}
    if(vertex[1].x > canvas_width_){bbox[1].x = canvas_width_;}
    if(vertex[1].y > canvas_height_){bbox[1].y = canvas_height_;}

}
template<typename T>
bool Renderer<T>::IsInsideTriangle(const array<Vec3<T>, 3>& vertex, const Vec2<T>& pixel){
    int count = 0;
    Vec2f vertex2[3] = {
        {vertex[0].x, vertex[0].y},
        {vertex[1].x, vertex[1].y},
        {vertex[2].x, vertex[2].y}
    };
    for(int i = 0; i < 3; i++){
        Vec2f v1 = vertex2[(i+1)%3] - vertex2[i];
        Vec2f v2 = pixel - vertex2[i];

        float dot = v1.cross(v2);
        if(dot > 0.0f) count++;
    }
    if( count == 0 || count == 3){
        return true;
    }
    return false;
}
template<typename T>
Vec3<T> Renderer<T>::BarycentricInterpolation(const array<Vec3<T>,3>& vertex,  const Vec2<T>& pixel){
    Vec2f v0(vertex[0].x, vertex[0].y);
    Vec2f v1(vertex[1].x, vertex[1].y);
    Vec2f v2(vertex[2].x, vertex[2].y);
    float area = 1.0f / (v0-v1).cross(v2 - v1);
    float alpha = (pixel-v1).cross(v2-v1) * area;
    float beta = (pixel-v2).cross(v0-v2) * area;
    float gamma = (pixel-v0).cross(v1-v0) * area;
    return Vec3<T>(alpha, beta, gamma); 
}
template<typename T>
Vec3<T> Renderer<T>::GetRayVector(const T& x, const T& y){
    return (view_port_cord_[0] * (x * 2.0f / canvas_width_ - 1.0f) * view_port_width_half_
         + view_port_cord_[1] * (y * 2.0f / canvas_height_ - 1.0f) * view_port_height_half_
         + view_port_cord_[2] * z_near_).normalized();
}
template<typename T>
Vec3<T> Renderer<T>::GetRayColor(const Ray& ray, const unsigned int step){
    HitPointInfo info;
    if(step == 0){
        return Vec3<T>(0.0f, 0.0f, 0.0f);
    }
    if(hitable_list_.HitObject(ray, 0.0001f, 1e6, info)){
        Ray scattered(info.hit_point);
        Vec3<T> attenuation;
        info.material_ptr->Scatter(ray, info, attenuation, scattered);
        return GetRayColor(scattered, step-1).cwiseProduct(attenuation);
    }
    int t = ray.direction.y*0.5;
    return Vec3<T>(1.0f, 1.0f, 1.0f)*(0.5f - t) + Vec3<T>(0.5f, 0.7f, 1.0f)*(0.5f + t);
}

}
