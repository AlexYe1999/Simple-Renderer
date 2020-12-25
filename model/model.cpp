#include"model.h"

namespace YeahooQAQ{
using namespace std;

Model::Model(const string& filename)
    :
    surfaces_(), 
    vertices_(),
    normals_(), 
    textures_(){ 
    ifstream in;
    in.open(filename, ifstream::in);
    
    string line;
    char trash;
    while(!in.eof()){
        getline(in, line);
        istringstream iss(line.c_str());
        if(!line.compare(0, 2, "v ")){
            Vec3f v;
            iss >> trash;
            for(int i = 0; i < 3; i++){ iss >> v[i];}
            vertices_.push_back(v);
        }
        else if(!line.compare(0, 3, "vn ")){
            Vec3f vn;
            iss >> trash >> trash;
            for(int i = 0; i < 3; i++){ iss >> vn[i];}
            normals_.push_back(vn);
        }
        else if(!line.compare(0, 3, "vt ")){
            Vec2f vt;
            iss >> trash >> trash;
            for(int i = 0; i < 2; i++){iss >> vt[i];}
            textures_.push_back(vt);
        }
        else if(!line.compare(0, 2, "f ")){
            array<Vec3i, 3> surfaces;
            Vec3i surface;
            iss >> trash;
            for(int i = 0; i < 3; i++){
                iss >> surface.vertex >> trash >> surface.uv >> trash >> surface.normal;
                surface.vertex--;
                surface.uv--;
                surface.normal--;
                surfaces[i] = surface;
            }
            surfaces_.push_back(surfaces);
        }

    }
    cout <<"Model: \n" 
    <<"  vertex: "<< vertices_.size() << endl
    <<"  normal: "<< normals_.size() << endl
    <<" texture: "<< textures_.size() << endl
    <<" surface: "<<surfaces_.size()<<"\n\n";

}

Model::~Model(){};


}