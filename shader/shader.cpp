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

}
Vec3f Shader::TextureFragmentShader(const FragmentShaderPayload& payload){

}
Vec3f Shader::BumpFragmentShader(const FragmentShaderPayload& payload){

}
Vec3f Shader::DisplacementFragmentShader(const FragmentShaderPayload& payload){

}

} 
