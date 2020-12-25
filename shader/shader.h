#ifndef SHADER_H_
#define SHADER_H_
#include"../geometry/geometry.h"
#include<vector>
namespace YeahooQAQ{
using namespace std;

enum class ShaderType{
    DEFAULT,
    NORMAL_SHADING,
    PHONG_SHADING,
    TEXTURE_SHADING,
    BUMPSHADING,
    DISPLACEMENT_SHADING
};

struct Light{
    Vec3f position;
    Vec3f intensity;
    Light(const Vec3f& _position, const Vec3f& _intensity) 
        : position(_position),intensity(_intensity){}
};

struct FragmentShaderPayload{
    Vec3f position;
    Vec3f color;
    Vec3f normal;
    Vec3f texture;
    FragmentShaderPayload(const Vec3f& _position, const Vec3f& _color, const Vec3f& _normal, const Vec3f& _texture)
        :   position(_position), color(_color), normal(_normal), texture(_texture){}
};

class Shader{
public:
    Shader();
    ~Shader();

public:
    void VertexShader();
public:
    bool LoadProperties(const vector<Light>& lights, const Vec3f& eye_pos);
    bool Setting(const ShaderType& shader_type){shader_type_ = shader_type;};
    Vec3f FragmentShader(const FragmentShaderPayload& payload);
private:
    Vec3f NormalFragmentShader(const FragmentShaderPayload& payload);
    Vec3f PhongFragmentShader(const FragmentShaderPayload& payload);
    Vec3f TextureFragmentShader(const FragmentShaderPayload& payload);
    Vec3f BumpFragmentShader(const FragmentShaderPayload& payload);
    Vec3f DisplacementFragmentShader(const FragmentShaderPayload& payload);

private:
    ShaderType shader_type_;
    Vec3f eye_pos_;
    vector<Light> lights_;
};
    

}
#endif