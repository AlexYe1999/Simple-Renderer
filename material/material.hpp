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
protected:
    inline Vec3f Reflect(const Vec3f& vec, const Vec3f& n) const{
        return vec - n * (vec*n) * 2.0f;
    }
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

class Metal : public Material {
public:
    Metal(const Vec3f& _color, const float& fuzz = 0.0f);
    virtual ~Metal();
public:
    virtual bool Scatter(
        const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
    ) const override;

public:
    float fuzz; 
    Vec3f color;
};

}

#endif