#include"shader.h"

namespace YeahooQAQ{
    
IShader::IShader(){}
IShader::~IShader(){}
bool IShader::IShader::SetEyePosition(const Vec3f& eye_pos){}
bool IShader::SetLights(const vector<PointLight>& lights){}
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

PhongShader::PhongShader() 
    :
    IShader(),
    eye_pos_(0.0f, 0.0f, 0.0f),
    lights_()
{}
PhongShader::~PhongShader(){}
bool PhongShader::SetEyePosition(const Vec3f& eye_pos){
    eye_pos_ = eye_pos;
}
bool PhongShader::SetLights(const vector<PointLight>& lights){
    lights_ = lights;
}

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
    for(const PointLight& light : lights_){
        Vec3f light_intensity = light.intensity;
        Vec3f  pos2light = light.position - pos;
        Vec3f n = (pos2light.normalized() + pos2eye.normalized());
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
    }
    return dst_color;
}

TextureShader::TextureShader() 
    :
    IShader(),
    eye_pos_(0.0f, 0.0f, 0.0f),
    lights_()
{}
TextureShader::~TextureShader(){}
bool TextureShader::SetEyePosition(const Vec3f& eye_pos){
    eye_pos_ = eye_pos;
}
bool TextureShader::SetLights(const vector<PointLight>& lights){
    lights_ = lights;
}
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
    for(const PointLight& light : lights_){
        Vec3f light_intensity = light.intensity;
        Vec3f  pos2light = light.position - pos;
        Vec3f n = (pos2light.normalized() + pos2eye.normalized());
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
    }
    return dst_color;
}
/* Vec3f Shader::BumpFragmentShader(const FragmentShaderPayload& payload){

}
Vec3f Shader::DisplacementFragmentShader(const FragmentShaderPayload& payload){

} */

} 
