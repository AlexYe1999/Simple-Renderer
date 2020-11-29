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
    int GetVertexSize() const{return static_cast<int>(vertices_.size());};
    int GetSurfeceSize() const{return static_cast<int>(surfaces_.size());};
    Vec3f GetVertex(const int index) const{return vertices_[index];}    
    vector<int> GetSurfece(const int index) const{return surfaces_[index];};

private:
    vector<Vec3f> vertices_;
    vector<vector<int>> surfaces_;

};


}

#endif