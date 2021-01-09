#include"shader.h"

namespace LemonCube{
    
IShader::IShader(){}
IShader::~IShader(){}
bool IShader::SetEyePosition(const Vec3f& eye_pos){
    eye_pos_ = eye_pos;
    return true;
}
bool IShader::SetLights(const vector<LightSource>& lights){
    lights_ = lights;
    return true;
}
void IShader::VertexShader(){}
Vec3f IShader::FragmentShader(const FragmentShaderPayload& payload){
    return Vec3f(0.5f, 0.5f, 0.5f);
}

NormalShader::NormalShader() : IShader(){}
NormalShader::~NormalShader(){}
void NormalShader::VertexShader(){}
Vec3f NormalShader::FragmentShader(const FragmentShaderPayload& payload){
    return ((payload.normal+Vec3f(1.0f,1.0f,1.0f))*0.5f).normalized();
}

PhongShader::PhongShader()  : IShader(){}
PhongShader::~PhongShader(){}
void PhongShader::VertexShader(){}
Vec3f PhongShader::FragmentShader(const FragmentShaderPayload& payload){
    const float p = 150;
    Vec3f dst_color(0.0f, 0.0f, 0.0f);
    Vec3f Ka(0.005f, 0.005f, 0.005f);
    Vec3f Kd = payload.color;
    Vec3f Ks(0.7f, 0.7f, 0.7f);
    Vec3f pos = payload.position;
    Vec3f normal = payload.normal; 
    Vec3f pos2eye = eye_pos_ - pos;
    for(const LightSource& light : lights_){
        Vec3f light_intensity = light.intensity;
        Vec3f  pos2light = light.position - pos;
        Vec3f n = (pos2light.normalized() + pos2eye.normalized());
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
    }
    return dst_color;
}

TextureShader::TextureShader() : IShader(){}
TextureShader::~TextureShader(){}
void TextureShader::VertexShader(){}
Vec3f TextureShader::FragmentShader(const FragmentShaderPayload& payload){
    const float p = 150;
    Vec3f dst_color(0.0f, 0.0f, 0.0f);
    Vec3f Ka(0.005f, 0.005f, 0.005f);
    Vec3f Kd = payload.texture;
    Vec3f Ks(0.7f, 0.7f, 0.7f);
    Vec3f pos = payload.position;
    Vec3f normal = payload.normal; 
    Vec3f pos2eye = eye_pos_ - pos;
    for(const LightSource& light : lights_){
        Vec3f light_intensity = light.intensity;
        Vec3f  pos2light = light.position - pos;
        Vec3f n = (pos2light.normalized() + pos2eye.normalized());
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
    }
    return dst_color;
}

BumpShader::BumpShader() : IShader(){}
BumpShader::~BumpShader(){}
void BumpShader::VertexShader(){};
Vec3f BumpShader::FragmentShader(const FragmentShaderPayload& payload){

}

DisplacementShader::DisplacementShader() : IShader(){}
DisplacementShader::~DisplacementShader(){}
void DisplacementShader::VertexShader(){};
Vec3f DisplacementShader::FragmentShader(const FragmentShaderPayload& payload){

}


} 
