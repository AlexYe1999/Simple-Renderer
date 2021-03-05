#ifdef SHADER_CPP
#include"shader.hpp"
#include<memory>
namespace LemonCube{

template<typename T>
IShader<T>::IShader(){}
template<typename T>
IShader<T>::~IShader(){}
template<typename T>
bool IShader<T>::SetEyePosition(const Vec3<T>& eye_pos){
    eye_pos_ = eye_pos;
    return true;
}
template<typename T>
bool IShader<T>::SetLights(const vector<LightSource>& lights){
    lights_ = lights;
    return true;
}
template<typename T>
Vec3<T> IShader<T>::FragmentShader(const FragmentShaderPayload<T>& payload){
    return payload.color;
}

template<typename T>
NormalShader<T>::NormalShader() : IShader<T>(){}
template<typename T>
NormalShader<T>::~NormalShader(){}
template<typename T>
Vec3<T> NormalShader<T>::FragmentShader(const FragmentShaderPayload<T>& payload){
    return ((payload.normal+Vec3f(1.0f,1.0f,1.0f))*0.5f).normalized();
}

template<typename T>
PhongShader<T>::PhongShader() : IShader<T>(){}
template<typename T>
PhongShader<T>::~PhongShader(){}
template<typename T>
Vec3<T> PhongShader<T>::FragmentShader(const FragmentShaderPayload<T>& payload){
    const float p = 150;
    Vec3<T> dst_color(0.0f, 0.0f, 0.0f);
    Vec3<T> Ka(0.005f, 0.005f, 0.005f);
    Vec3<T> Kd = payload.color;
    Vec3<T> Ks(0.7f, 0.7f, 0.7f);
    Vec3<T> pos = payload.position;
    Vec3<T> normal = payload.normal; 
    Vec3<T> pos2eye = IShader<T>::eye_pos_ - pos;
    for(const LightSource& light : IShader<T>::lights_){
        Vec3<T> light_intensity = light.intensity;
        Vec3<T>  pos2light = light.position - pos;
        Vec3<T> n = (pos2light.normalized() + pos2eye.normalized());
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
    }
    return dst_color;
}

template<typename T>
TextureShader<T>::TextureShader() : IShader<T>(){}
template<typename T>
TextureShader<T>::~TextureShader(){}
template<typename T>
Vec3<T> TextureShader<T>::FragmentShader(const FragmentShaderPayload<T>& payload){
    const float p = 150;
    Vec3<T> dst_color(0.0f, 0.0f, 0.0f);
    Vec3<T> Ka(0.005f, 0.005f, 0.005f);
    Vec3<T> Kd = payload.texture;
    Vec3<T> Ks(0.7f, 0.7f, 0.7f);
    Vec3<T> pos = payload.position;
    Vec3<T> normal = payload.normal; 
    Vec3<T> pos2eye = IShader<T>::eye_pos_ - pos;
    for(const LightSource& light : IShader<T>::lights_){
        Vec3<T> light_intensity = light.intensity;
        Vec3<T>  pos2light = light.position - pos;
        Vec3<T> n = (pos2light.normalized() + pos2eye.normalized());
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
    }
    return dst_color;
}

template<typename T>
BumpShader<T>::BumpShader() : IShader<T>(){}
template<typename T>
BumpShader<T>::~BumpShader(){}
template<typename T>
Vec3<T> BumpShader<T>::FragmentShader(const FragmentShaderPayload<T>& payload){
    const float p = 150;
    Vec3<T> dst_color(0.0f, 0.0f, 0.0f);
    Vec3<T> Ka(0.005f, 0.005f, 0.005f);
    Vec3<T> Kd = payload.color;
    Vec3<T> Ks(0.7f, 0.7f, 0.7f);
    Vec3<T> pos = payload.position;
    Vec3<T> normal = payload.normal; 
    Vec3<T> pos2eye = IShader<T>::eye_pos_ - pos;

    normal = payload.TBN * payload.texture;

    for(const LightSource& light : IShader<T>::lights_){
        Vec3<T> light_intensity = light.intensity;
        Vec3<T>  pos2light = light.position - pos;
        Vec3<T> n = (pos2light.normalized() + pos2eye.normalized());
        dst_color += Ka.cwiseProduct(light_intensity);
        dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
        dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
    }
    return dst_color;
}

template<typename T>
DisplacementShader<T>::DisplacementShader() : IShader<T>(){}
template<typename T>
DisplacementShader<T>::~DisplacementShader(){}
template<typename T>
Vec3<T> DisplacementShader<T>::FragmentShader(const FragmentShaderPayload<T>& payload){

}


} 

#endif