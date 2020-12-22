#include"shader.h"

namespace YeahooQAQ{
    
Shader::Shader()
    :
    shader_type_(ShaderType::DEFAULT),
    eye_pos_(0.0f, 0.0f, 0.0f),
    lights_{}
{}

Shader::~Shader(){}

bool Shader::LoadProperties(const vector<Light>& lights, const Vec3f& eye_pos){        
    lights_ = lights;
    eye_pos_ = eye_pos;
}

Vec3f Shader::FragmentShader(const FragmentShaderPayload& payload){
    Vec3f color;
    switch (shader_type_){
        case ShaderType::NORMAL_SHADING:{
            color = NormalFragmentShader(payload);
            break;
        }
        case ShaderType::PHONG_SHADING:{
            color = PhongFragmentShader(payload);
            break;
        }
        case ShaderType::TEXTURE_SHADING:{
            color = TextureFragmentShader(payload);
            break;
        }
        case ShaderType::BUMPSHADING:{
            color = NormalFragmentShader(payload);
            break;
        }
        case ShaderType::DISPLACEMENT_SHADING:{
            color = NormalFragmentShader(payload);
            break;
        }
        default:{
            color = Vec3f(0.1f, 0.1f, 0.1f);
        }
    }
    return color;
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
    Vec3f normal = payload.normal; 
    Vec3f pos2eye = eye_pos_ - pos;
    Vec3f dst_color(0.1f, 0.1f, 0.1f);

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
    const float p = 150;
    Vec3f pos = payload.position;
    Vec3f normal = payload.normal; 
    Vec3f pos2eye = eye_pos_ - pos;
    Vec3f dst_color(0.2f, 0.2f, 0.2f);

    for(const Light& light : lights_){
        Vec3f light_intensity = light.intensity;        
        Vec3f light_pos = light.position;
        Vec3f  pos2light = light_pos - pos;
        Vec3f n = (pos2light.normalized() + pos2eye.normalized()).normalized();
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n), p);
    }
    return dst_color;
}
Vec3f Shader::BumpFragmentShader(const FragmentShaderPayload& payload){

}
Vec3f Shader::DisplacementFragmentShader(const FragmentShaderPayload& payload){

}

} 
