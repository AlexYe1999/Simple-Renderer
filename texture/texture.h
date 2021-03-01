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
    Vec3f GetColor(const float& u, const float& v) const;
    int GetWidth() const{ return image_width_; }
    int GetHeight() const{ return image_height_; }   
    bool IsValid() const{ return is_texture_valid_; }
private:
    bool is_texture_valid_; 
    int image_width_;
    int image_height_;
    cv::Mat image_data_;

};


}

#endif