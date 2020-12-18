#ifndef MODEL_H_
#define MODEL_H_

#include"geometry.h"
#include<vector>
#include<fstream>
#include<sstream>
#include<opencv2/opencv.hpp>

namespace YeahooQAQ{
using namespace std;

class Model{
public:
    Model(const string& file_name, const string& texture_name);
    ~Model();
public: 
    const int GetSurfeceSize() const{return static_cast<unsigned int>(surfaces_.size());}
    const int GetVertexSize() const{return static_cast<unsigned int>(vertices_.size());}
    const int GetNormalSize() const{return static_cast<unsigned int>(normals_.size());}
    const int GetTextureSize() const{return static_cast<unsigned int>(textures_.size());}
    const vector<Vec3i>& GetSurfece(const int index) const{return surfaces_[index];}
    const Vec3f& GetVertex(const int index) const{return vertices_[index];}    
    const Vec3f& GetNormal(const int index) const{return normals_[index];}    
    const Vec2f& GetTexture(const int index) const{return textures_[index];}
    const Vec3f getColor(const float& u, const float& v);
    
private:
    vector<vector<Vec3i>> surfaces_;
    vector<Vec3f> vertices_;
    vector<Vec3f> normals_;
    vector<Vec2f> textures_;

private:
    cv::Mat image_data_;
    int image_width_;
    int image_height_;
};


}

#endif