#ifndef SHADER_H_
#define SHADER_H_
#include"../model/geometry.h"
#include<vector>
namespace YeahooQAQ{

struct Light{
    Vec3f position;
    Vec3f intensity;
    Light(const Vec3f& _position, const Vec3f& _intensity) 
        : position(_position),intensity(_intensity){}
};

struct FragmentShaderPayload{
    Vec3f color;
    Vec3f normal;
    FragmentShaderPayload(const Vec3f& _color, const Vec3f& _normal)
        :   color(_color),normal(_normal){}
};

class Shader{
public:
    Shader();
    ~Shader();

public:
    void VertexShader();
public:
    Vec3f LoadProperties(const vector<Light>& lights);
    Vec3f NormalFragmentShader(const FragmentShaderPayload& payload);
    Vec3f PhongFragmentShader(const FragmentShaderPayload& payload);
    Vec3f TextureFragmentShader(const FragmentShaderPayload& payload);
    Vec3f BumpFragmentShader(const FragmentShaderPayload& payload);
    Vec3f DisplacementFragmentShader(const FragmentShaderPayload& payload);

private:
    vector<Light> lights_;
};
    

}
#endif