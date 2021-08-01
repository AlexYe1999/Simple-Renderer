#ifndef SHADER_H_
#define SHADER_H_
#include"../geometry/geometry.h"
#include"../texture/texture.h"
#include"../object/object.h"
#include<vector>
#include<memory>
namespace LemonCube{
using namespace std;

template <typename T>
struct FragmentShaderPayload{
    Vec3<T> position;
    Vec3<T> color;
    Vec3<T> texture;
    Vec3<T> normal;
    Matrix3<T> TBN;
    FragmentShaderPayload(
        const Vec3<T>& _position, const Vec3<T>& _color, 
        const Vec3<T>& _texture, const Vec3<T>& _normal, const Matrix3<T>& _TBN
    ) : position(_position), color(_color), texture(_texture), normal(_normal), TBN(_TBN){}
};
    
template <typename T>
class IShader{
public:
    IShader();
    virtual ~IShader();

public:
    bool SetEyePosition(const Vec3<T>& eye_pos);
    bool SetLights(const vector<shared_ptr<LightSource>>& lights);
    vector<shared_ptr<LightSource>> GetAllLight(){return lights_;}

public:
    virtual Vec3<T> FragmentShader(const FragmentShaderPayload<T>& payload);

protected:
    Vec3<T> eye_pos_;
    vector<shared_ptr<LightSource>> lights_;
};

template <typename T>
class NormalShader : public IShader<T>{
public:
    NormalShader();
    ~NormalShader();

public:
    Vec3<T> FragmentShader(const FragmentShaderPayload<T>& payload) override;

};

template <typename T>
class PhongShader : public IShader<T>{
public:  
    PhongShader();
    ~PhongShader();

public:
    Vec3<T> FragmentShader(const FragmentShaderPayload<T>& payload) override;

};

template <typename T>
class TextureShader : public IShader<T>{
public:
    TextureShader();
    ~TextureShader();
    
public:
    Vec3<T> FragmentShader(const FragmentShaderPayload<T>& payload) override;

};

template <typename T>
class BumpShader : public IShader<T>{
public:
    BumpShader();
    ~BumpShader();

public:
    Vec3<T> FragmentShader(const FragmentShaderPayload<T>& payload) override;
};

template <typename T>
class DisplacementShader : public IShader<T>{
public:
    DisplacementShader();
    ~DisplacementShader();

public:
    Vec3<T> FragmentShader(const FragmentShaderPayload<T>& payload) override;
};
template <typename T>
class ShadowShader : public IShader<T>{
public:
    ShadowShader();
    ~ShadowShader();
    
public:
    Vec3<T> FragmentShader(const FragmentShaderPayload<T>& payload) override;

};

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
bool IShader<T>::SetLights(const vector<shared_ptr<LightSource>>& lights){
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
    for(const shared_ptr<LightSource>& light : IShader<T>::lights_){
        Vec3<T> light_intensity = light->intensity;
        Vec3<T> pos2light = light->position - pos;
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
    for(const shared_ptr<LightSource>& light : IShader<T>::lights_){
        Vec3<T> light_intensity = light->intensity;
        Vec3<T>  pos2light = light->position - pos;
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

    for(const shared_ptr<LightSource>& light : IShader<T>::lights_){
        Vec3<T> light_intensity = light->intensity;
        Vec3<T>  pos2light = light->position - pos;
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


template<typename T>
ShadowShader<T>::ShadowShader() : IShader<T>(){}
template<typename T>
ShadowShader<T>::~ShadowShader(){}
template<typename T>
Vec3<T> ShadowShader<T>::FragmentShader(const FragmentShaderPayload<T>& payload){
    const float p = 150;
    Vec3<T> dst_color(0.0f, 0.0f, 0.0f);
    Vec3<T> Ka(0.005f, 0.005f, 0.005f);
    Vec3<T> Kd = payload.texture;
    Vec3<T> Ks(0.7f, 0.7f, 0.7f);
    Vec3<T> pos = payload.position;
    Vec3<T> normal = payload.normal; 
    Vec3<T> pos2eye = IShader<T>::eye_pos_ - pos;
    for(const shared_ptr<LightSource>& light : IShader<T>::lights_){
        Vec3<T> light_intensity = light->intensity;
        Vec3<T>  pos2light = light->position - pos;
        Vec3<T> n = (pos2light.normalized() + pos2eye.normalized());
        
        auto shadow_light = dynamic_cast<ShadowLight*>(light.get());
        if(shadow_light != nullptr){
            Vec3<T> shadow_view_pos = (shadow_light->Shadow_view_*pos.toVec4(1.0f)).toVec3();
            shadow_view_pos = shadow_view_pos*0.5f+Vec3f(0.5f, 0.5f, 0.0f);
            shadow_view_pos = shadow_view_pos*static_cast<int>(shadow_light->buffer_size_);
            
            int x = shadow_view_pos.x+0.5f;
            int y = shadow_view_pos.y+0.5f;
            if(x < 0 || y < 0 || x >= shadow_light->buffer_size_ || y >= shadow_light->buffer_size_){
                //cout<<"pos out of range of shadow map: "<<x<<" "<<y<<endl;
                continue;
            } 
            float* z_buffer_ = shadow_light->depth_buffer_.get();
            Vec3<T> direction = (shadow_light->position - shadow_light->lookat_).normalized();
            if(shadow_view_pos.z < z_buffer_[(shadow_light->buffer_size_-1-y)* shadow_light->buffer_size_ + x]+tan(acos(direction*normal))*2.0f/shadow_light->buffer_size_){
                Vec3<T> light_intensity = shadow_light->intensity;
                Vec3<T> n = (direction.normalized() + pos2eye.normalized());
                dst_color += Ka.cwiseProduct(light_intensity);
                dst_color += Kd.cwiseProduct(light_intensity) * max(0.0f, direction.normalized() * normal);
                dst_color += Ks.cwiseProduct(light_intensity) * pow(max(0.0f, normal * n.normalized()), p);
            }
        }
        else{
            dst_color += Ka.cwiseProduct(light_intensity);
            dst_color += (Kd.cwiseProduct(light_intensity)/(pos2light*pos2light)) * max(0.0f, pos2light.normalized() * normal);
            dst_color += (Ks.cwiseProduct(light_intensity)/(pos2light*pos2light)) * pow(max(0.0f, normal * n.normalized()), p);
        }
    }
    return dst_color;
}

}
#endif