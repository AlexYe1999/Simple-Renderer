#include"model.h"

namespace YeahooQAQ{
using namespace std;

Model::Model(string& filename, string texture_name)
    :
    surfaces_(), 
    vertices_(),
    normals_(), 
    textures_(),
    image_data_(),
    image_width_(0),
    image_height_(0){ 
    ifstream in;
    in.open(filename, ifstream::in);
    if(in.fail()){
        cout<<"Initialazation of model is failed\n";
        return;
    }
    
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
            vector<Vec3i> surfaces;
            Vec3i surface;
            iss >> trash;
            for(int i = 0; i < 3; i++){
                iss >> surface.vertex >> trash >> surface.uv >> trash >> surface.normal;
                surface.vertex--;
                surface.uv--;
                surface.normal--;
                surfaces.push_back(surface);
            }
            surfaces_.push_back(surfaces);
        }

    }
    cout <<"Model: \n" 
    <<"  vertex: "<< vertices_.size() << endl
    <<"  normal: "<< normals_.size() << endl
    <<" texture: "<< textures_.size() << endl
    <<" surface: "<<surfaces_.size()<<"\n\n";

    if(texture_name != ""){
        image_data_ = cv::imread(texture_name);
        if(!image_data_.empty()){
            image_width_ = image_data_.cols;
            image_height_ = image_data_.rows;
            cvtColor(image_data_, image_data_, CV_RGB2BGR); 
            cout<<"Texture is loaded\n";
        }
        else{
            cout<<"Texture is not loaded\n\n";
        }
    }
}

Model::~Model(){};

const Vec3f Model::getColor(float u, float v){
    if(image_data_.empty()){
        return Vec3f(rand() % 256, rand() % 256, rand() % 256);
    }
    else{
        int u_img = u * image_width_;
        int v_img = (1.0f - v) * image_height_;
        auto color = image_data_.at<cv::Vec3b>(v_img, u_img);
        return Vec3f(color[0], color[1], color[2]);
    }
}


}