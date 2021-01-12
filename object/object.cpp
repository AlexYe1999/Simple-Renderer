#include"object.h"
namespace LemonCube{

Object::Object(const Vec3f& _position) : position(_position){}
Object::~Object(){}

Ray::Ray(const Vec3f& _position, const Vec3f& _direction) 
    :
    Object(_position),
    direction(_direction)
{}

Ray::~Ray(){};

Vec3f Ray::at(const float t) const{
    return position + direction * t;
}

Hitable::Hitable(const Vec3f& _position) : Object(_position){}
Hitable::~Hitable(){}

Sphere::Sphere(const Vec3f& _position, const float& _radius,const shared_ptr<Material>& _material_ptr)
    : 
    Hitable(_position),
    radius(_radius),
    material_ptr(_material_ptr)
{}
Sphere::~Sphere(){}

bool Sphere::HitObject(const Ray& ray, const float& t_min, const float& t_max, HitPointInfo& info) const{
    Vec3f oc = ray.position - position;
    float a = ray.direction * ray.direction;
    float b = ray.direction * oc;
    float c = oc * oc - radius * radius;
    float delta = b * b - a * c;
    if(delta < 0.0f) return false;
    float t = (-b - sqrt(delta) ) / a;
    if(t > t_max || t < t_min){
        t = (-b + sqrt(delta) ) / a;
        if(t > t_max || t < t_min){
            return false;
        }
    }
    info.time = t;
    info.material_ptr = material_ptr;
    info.hit_point = ray.at(t);
    info.SetReflectNormal(ray, (info.hit_point - position) / radius);
    return true;
} 

LightSource::LightSource(const Vec3f& _position, const Vec3f& _intensity)
    : 
    Object(_position),
    intensity(_intensity)
{}
LightSource::~LightSource(){}


}