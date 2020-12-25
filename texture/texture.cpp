#include "texture.h"

namespace YeahooQAQ{

Texture::Texture(const string& texture_name)
    :
    is_texture_valid_(false),
    image_width_(0),
    image_height_(0),
    image_data_(){
    if(texture_name != ""){
        image_data_ = cv::imread(texture_name);
        if(!image_data_.empty()){
            image_width_ = image_data_.cols;
            image_height_ = image_data_.rows;
            cvtColor(image_data_, image_data_, CV_RGB2BGR); 
            is_texture_valid_ = true;
            cout<<"Texture is loaded\n";
        }
        else{
            cout<<"Texture is not loaded\n\n";
        }
    }

}


Vec3f Texture::getColor(const float& u, const float& v) const{
    int u_img = u * image_width_;
    int v_img = (1.0f - v) * image_height_;
    auto color = image_data_.at<cv::Vec3b>(v_img, u_img);
    return Vec3f(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f);
}

}