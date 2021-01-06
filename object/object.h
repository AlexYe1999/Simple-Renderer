#ifndef OBJECT_H
#define OBJECT_H
#include"../geometry/geometry.h"
#include"../geometry/shape.h"
namespace YeahooQAQ{

class Object{
public:
    Object(const Vec3f& _position);
    virtual ~Object() = 0;

public:
    Vec3f position;   

};

class PointLight : public Object{
public:
    PointLight(const Vec3f& _position, const Vec3f& _intensity);
    ~PointLight();

public:
    Vec3f intensity;
};

}

#endif