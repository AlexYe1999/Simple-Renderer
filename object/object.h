#ifndef OBJECT_H
#define OBJECT_H
#include"../geometry/geometry.h"
#include"../geometry/shape.h"
namespace LemonCube{

class Ray;

class Object{
public:
    Object(const Vec3f& _position);
    virtual ~Object() = 0;

public:
    virtual float HitObject(const Ray& Ray) const;
public:
    Vec3f position;   

};

class LightSource : public Object{
public:
    LightSource(const Vec3f& _position, const Vec3f& _intensity);
    ~LightSource();

public:
    Vec3f intensity;
};

class Sphere : public Object{
public:
    Sphere(const Vec3f& _position, const float _radius);
    ~Sphere();

public:
    float HitObject(const Ray& ray) const;
public:
    float radius;
};


class Ray : public Object{
public:
    Ray(const Vec3f& _position, const Vec3f& _direction);
    ~Ray();

public:
    Vec3f at(const float t) const;

public:
    Vec3f direction;

};


}

#endif