#ifndef SHADER_H_
#define SHADER_H_
#include"../geometry/geometry.h"
#include"../object/object.h"
#include<vector>
namespace YeahooQAQ{
using namespace std;

struct FragmentShaderPayload{
    Vec3f position;
    Vec3f color;
    Vec3f normal;
    Vec3f texture;
    FragmentShaderPayload(const Vec3f& _position, const Vec3f& _color, const Vec3f& _normal, const Vec3f& _texture)
        :   position(_position), color(_color), normal(_normal), texture(_texture){}
};
    
class IShader{
public:
    IShader();
    virtual ~IShader();

public:
    bool SetEyePosition(const Vec3f& eye_pos);
    bool SetLights(const vector<PointLight>& lights);

public:
    virtual void VertexShader();
    virtual Vec3f FragmentShader(const FragmentShaderPayload& payload);

protected:
    Vec3f eye_pos_;
    vector<PointLight> lights_;
};

class NormalShader : public IShader{
public:
    NormalShader();
    ~NormalShader();

public:
    void VertexShader();
    Vec3f FragmentShader(const FragmentShaderPayload& payload);

};


class PhongShader : public IShader{
public:  
    PhongShader();
    ~PhongShader();

public:
    void VertexShader();
    Vec3f FragmentShader(const FragmentShaderPayload& payload);

};

class TextureShader : public IShader{
public:
    TextureShader();
    ~TextureShader();
    
public:
    void VertexShader();
    Vec3f FragmentShader(const FragmentShaderPayload& payload);

};

class BumpShader : public IShader{
public:
    BumpShader();
    ~BumpShader();

public:
    void VertexShader();
    Vec3f FragmentShader(const FragmentShaderPayload& payload);
};

class DisplacementShader : public IShader{
public:
    DisplacementShader();
    ~DisplacementShader();

public:
    void VertexShader();
    Vec3f FragmentShader(const FragmentShaderPayload& payload);
};

}
#endif