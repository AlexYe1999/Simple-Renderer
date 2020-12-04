#ifndef MODEL_H_
#define MODEL_H_

#include<vector>
#include<fstream>
#include<sstream>
#include"geometry.h"

namespace YeahooQAQ{
using namespace std;

class Model{
public:
    Model(string& filename);
    ~Model();
    
public: 
    const int GetVertexSize() const{return static_cast<int>(vertices_.size());};
    const int GetSurfeceSize() const{return static_cast<int>(surfaces_.size());};
    const Vec3f& GetVertex(const int index) const{return vertices_[index];}    
    const Vec3f& GetNormal(const int index) const{return normals_[index];}    
    const Vec2f& GetTexture(const int index) const{return textures_[index];}    
    const vector<Vec3i>& GetSurfece(const int index) const{return surfaces_[index];};

private:
    vector<vector<Vec3i>> surfaces_;
    vector<Vec3f> vertices_;
    vector<Vec3f> normals_;
    vector<Vec2f> textures_;
};


}

#endif