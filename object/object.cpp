#include"object.h"
namespace YeahooQAQ{

Object::Object(const Vec3f& _position) : position(_position){}
Object::~Object(){};

PointLight::PointLight(const Vec3f& _position, const Vec3f& _intensity) 
    :
    Object(_position),
    intensity(_intensity)
{}

PointLight::~PointLight(){}




}