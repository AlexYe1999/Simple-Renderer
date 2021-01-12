#ifndef MATERIAL_H_
#define MATERIAL_H_
#include"../object/object.h"
namespace LemonCube{
class Ray;
struct HitPointInfo;

class Material{
public:
    Material(){};
    virtual ~Material() = 0;

public:
    virtual bool Scatter(
        const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
    ) const = 0;
};

class Lambertian : public Material{
public:
    Lambertian(const Vec3f& _color);
    virtual ~Lambertian();

public:
    virtual bool Scatter(
        const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
    ) const override;
public:
    Vec3f color;
};


}

#endif