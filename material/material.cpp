#include"material.hpp"

namespace LemonCube{

Material::~Material(){}
Lambertian::Lambertian(const Vec3f& _color) : color(_color){};
Lambertian::~Lambertian(){};
bool Lambertian::Scatter(const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered) const { 
    //Vec3f rand_vector = Vec3<float>::RandomInSphere();
    Vec3f rand_vector = Vec3<float>::RandomInSphere().normalized();
    if(rand_vector * info.normal < 0.0f){
        rand_vector *= -0.1f;
    }
    attenuation = color * (rand_vector * info.normal);
    scattered.position = info.hit_point;
    scattered.direction = rand_vector;
    return true;
}


Metal::Metal(const Vec3f& _color, const float& _fuzz) : color(_color), fuzz(_fuzz){};
Metal::~Metal(){};
bool Metal::Scatter(const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered) const { 
    Vec3f reflect = Reflect(ray.direction, info.normal) + Vec3<float>::RandomInSphere().normalized() * fuzz;
    attenuation = color;
    scattered.position = info.hit_point;
    scattered.direction = reflect;
    return (scattered.direction * info.normal) > 0.0f;
}





}