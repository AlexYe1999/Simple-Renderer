#ifndef TEXTURE_H_
#define TEXTURE_H_
#include"../geometry/geometry.h"
#include<opencv2/opencv.hpp>
#include<string>
namespace LemonCube{
using namespace std;

class Texture{
public:
    Texture(const string& texture_name);
    ~Texture(){};

public:
    Vec3f getColor(const float& u, const float& v) const;
    bool IsValid() const{return is_texture_valid_;}
private:
    bool is_texture_valid_; 
    int image_width_;
    int image_height_;
    cv::Mat image_data_;

};


}

#endif