#include"shader.h"

namespace YeahooQAQ{
    
Shader::Shader()
    :
    lights_{}
{}

Shader::~Shader(){}

Vec3f Shader::LoadProperties(const vector<Light>& lights){        
    lights_ = lights;
}

Vec3f Shader::NormalFragmentShader(const FragmentShaderPayload& payload){
    return (payload.normal+Vec3f(1.0f,1.0f,1.0f))*0.5f;
} 
Vec3f Shader::PhongFragmentShader(const FragmentShaderPayload& payload){
    Vec3f Ka(0.0000007f, 0.000007f, 0.0000007f);
    Vec3f Kd = payload.color;
    Vec3f Ks(0.7f, 0.7f, 0.7f);
    const float p = 150;
    Vec3f pos = payload.position;
    Vec3f eye_pos(0.0,0.0, 0.0f);
    Vec3f normal = payload.normal; 
    Vec3f pos2eye = pos * -1.0f;
    Vec3f dst_color(0.0f, 0.0f, 0.0f);

    for(const Light& light : lights_){
        Vec3f light_intensity = light.intensity;        
        Vec3f light_pos = light.position;
        Vec3f  pos2light = light_pos - pos;
        Vec3f n = (pos2light.normalized() + pos2eye.normalized()).normalized();
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal.normalized());
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal.normalized() * n), p);
    }
    return dst_color;
}
Vec3f Shader::TextureFragmentShader(const FragmentShaderPayload& payload){
    Vec3f Ka(0.00001f, 0.00001f, 0.00001f);
    Vec3f Kd = payload.texture;
    Vec3f Ks(0.7f, 0.7f, 0.7f);
    const float p = 80;
    Vec3f pos = payload.position;
    Vec3f normal = payload.normal; 
    Vec3f pos2eye = Vec3f(500,500,0) - pos;
    Vec3f dst_color(0.25f, 0.25f, 0.25f);

    for(const Light& light : lights_){
        Vec3f light_intensity = light.intensity;        
        Vec3f light_pos = light.position;
        Vec3f  pos2light = light_pos - pos;
        Vec3f n = (pos2light.normalized() + pos2eye.normalized()).normalized();
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal.normalized());
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal.normalized() * n), p);
    }
    return dst_color;
}
Vec3f Shader::BumpFragmentShader(const FragmentShaderPayload& payload){

}
Vec3f Shader::DisplacementFragmentShader(const FragmentShaderPayload& payload){

}

} 
