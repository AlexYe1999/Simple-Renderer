#ifndef OBJECT_H
#define OBJECT_H
#include"../geometry/geometry.h"
#include"../geometry/shape.h"
#include"../material/material.hpp"
#include<memory>
namespace LemonCube{
class Material;
class Object{
public:
    Object(const Vec3f& _position);
    virtual ~Object() = 0;

public:
    Vec3f position;
};

class Ray : public Object{
public:
    Ray(const Vec3f& _position) : Object(_position){};
    Ray(const Vec3f& _position, const Vec3f& _direction);
    ~Ray();

public:
    Vec3f at(const float t) const;

public:
    Vec3f direction;

};

struct HitPointInfo{
    float time; 
    shared_ptr<Material> material_ptr;
    Vec3f hit_point;
    Vec3f normal;
    inline void SetReflectNormal(const Ray& _ray, const Vec3f& _normal){
        normal = _normal * _ray.direction < 0.0f ? _normal : _normal * -1.0f;
    }
};
class Hitable : public Object{
public:
    Hitable(const Vec3f& _position);
    virtual ~Hitable() = 0;
public:
    virtual bool HitObject(const Ray& ray, const float& t_min, const float& t_max, HitPointInfo& info) const = 0;

};

class Sphere : public Hitable{
public:
    Sphere(const Vec3f& _position, const float& _radius,const shared_ptr<Material>& _material_ptr);
    ~Sphere();

public:
    bool HitObject(const Ray& ray, const float& t_min, const float& t_max, HitPointInfo& info) const override;
private:
    float radius;
    shared_ptr<Material> material_ptr;
};

class LightSource : public Object{
public:
    LightSource(const Vec3f& _position, const Vec3f& _intensity);
    ~LightSource();

public:
    Vec3f intensity;
};





}

#endif