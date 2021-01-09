#include"object.h"
namespace LemonCube{

Object::Object(const Vec3f& _position) : position(_position){}
Object::~Object(){};
float Object::HitObject(const Ray& ray) const{ return -1.0f;};

LightSource::LightSource(const Vec3f& _position, const Vec3f& _intensity)
    : 
    Object(_position),
    intensity(_intensity)
{}
LightSource::~LightSource(){}

Sphere::Sphere(const Vec3f& _position, const float _radius)
    : 
    Object(_position),
    radius(_radius)
{}
Sphere::~Sphere(){}

float Sphere::HitObject(const Ray& ray) const{
    Vec3f oc = ray.position - position;
    float a = ray.direction * ray.direction;
    float b = ray.direction * oc;
    float c = oc * oc - radius * radius;
    float delta = b * b - a * c;
    return delta > 0.0f ? (-b - sqrt(delta) ) / a : -1.0f;
} 


Ray::Ray(const Vec3f& _position, const Vec3f& _direction) 
    :
    Object(_position),
    direction(_direction)
{}

Ray::~Ray(){};


Vec3f Ray::at(const float t) const{
    return position + direction * t;
}

}