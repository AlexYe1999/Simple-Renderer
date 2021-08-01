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

ShadowLight::ShadowLight(const Vec3f& _position, const Vec3f& _intensity, const Vec3f& _lookat, const int _buffer_size)
    : 
    LightSource(_position, _intensity),
    lookat_(_lookat),
    buffer_size_(_buffer_size),
    Shadow_view_(),
    depth_buffer_(new float[_buffer_size*_buffer_size]){

    Vec3f z = (_lookat - _position).normalized();
    Vec3f y = Vec3f(0.0f,1.0f,0.0f);
    Vec3f x = y.cross(z).normalized();
    y = z.cross(x).normalized();
    Matrix3f mat3(x, y, z);
    mat3 = mat3.inversed();
    Shadow_view_ = mat3.toMatrix4();

    Matrix4f mat4 = {
        Vec4f(1.0f, 0.0f, 0.0f, 0.0f),
        Vec4f(0.0f, 1.0f, 0.0f, 0.0f),
        Vec4f(0.0f, 0.0f, 1.0f, 0.0f),
        (_position * -1.0f).toVec4(1.0f) 
    };

    Shadow_view_ = Shadow_view_ * mat4;

    for(int i = 0;i < _buffer_size*_buffer_size; i++){
        depth_buffer_[i] = numeric_limits<float>::max();
    }

} 

ShadowLight::~ShadowLight(){}


}