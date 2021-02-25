#include"shader.h"
#include<memory>
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
Vec3f IShader::FragmentShader(const FragmentShaderPayload& payload){
    return payload.color;
}

NormalShader::NormalShader() : IShader(){}
NormalShader::~NormalShader(){}
Vec3f NormalShader::FragmentShader(const FragmentShaderPayload& payload){
    return ((payload.normal+Vec3f(1.0f,1.0f,1.0f))*0.5f).normalized();
}

PhongShader::PhongShader()  : IShader(){}
PhongShader::~PhongShader(){}
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
Vec3f TextureShader::FragmentShader(const FragmentShaderPayload& payload){
    const float p = 150;
    Vec3f dst_color(0.0f, 0.0f, 0.0f);
    Vec3f Ka(0.005f, 0.005f, 0.005f);
    Vec3f Kd = payload.texture
        ? payload.texture->GetColor(payload.tex_coord.u, payload.tex_coord.v) 
        : payload.color;
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
Vec3f BumpShader::FragmentShader(const FragmentShaderPayload& payload){
    const float p = 150;
    Vec3f dst_color(0.0f, 0.0f, 0.0f);
    Vec3f Ka(0.005f, 0.005f, 0.005f);
    Vec3f Kd = payload.texture
        ? payload.texture->GetColor(payload.tex_coord.u, payload.tex_coord.v) 
        : payload.color;
    Vec3f Ks(0.7f, 0.7f, 0.7f);
    Vec3f pos = payload.position;
    Vec3f normal = payload.normal; 
    Vec3f pos2eye = eye_pos_ - pos;

	float u = payload.tex_coord.u;
	float v = payload.tex_coord.v;
    float width = payload.texture->GetWidth();
    float height = payload.texture->GetHeight();



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

DisplacementShader::DisplacementShader() : IShader(){}
DisplacementShader::~DisplacementShader(){}
Vec3f DisplacementShader::FragmentShader(const FragmentShaderPayload& payload){

}


} 
