#ifndef MATERIAL_H_
#define MATERIAL_H_
#include"../object/object.h"
namespace LemonCube{
class Ray;
struct HitPointInfo;

class Material{
public:
    Material(const Vec3f& _color) : color(_color){};
    virtual ~Material() = 0;

public:
    virtual bool Scatter(
        const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
    ) const = 0;
protected:
    inline Vec3f Reflect(const Vec3f& vec, const Vec3f& n) const{
        return vec - n * (vec*n) * 2.0f;
    }
    inline Vec3f Refract(const Vec3f& vec, const Vec3f& n, const float& thetai_over_thetao) const{
        Vec3f vec_horizontal = (vec + n * -1.0f * (n * vec)) * thetai_over_thetao;
        Vec3f vec_vertical = n * -sqrt(1.0f - vec_horizontal*vec_horizontal);
        return vec_vertical + vec_horizontal;
    }

protected:
    Vec3f color;
};

class Lambertian : public Material{
public:
    Lambertian(const Vec3f& _color);
    virtual ~Lambertian();

public:
    virtual bool Scatter(
        const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
    ) const override;
};

class Metal : public Material {
public:
    Metal(const Vec3f& _color, const float& _fuzz = 0.1f);
    virtual ~Metal();
public:
    virtual bool Scatter(
        const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
    ) const override;

protected:
    float fuzz; 
};


class Dielectric : public Metal {
public:
    Dielectric(const Vec3f& _color, const float& _refraction_index = 1.5f, const float& _fuzz = 0.0f);
    virtual ~Dielectric();
public:
    virtual bool Scatter(
        const Ray& ray, const HitPointInfo& info, Vec3f& attenuation, Ray& scattered
    ) const override;

public:
    float refraction_index; 
};


}

#endif