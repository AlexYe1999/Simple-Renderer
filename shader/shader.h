#ifndef SHADER_H_
#define SHADER_H_
#include"../geometry/geometry.h"
#include"../texture/texture.h"
#include"../object/object.h"
#include<vector>
#include<memory>
namespace LemonCube{
using namespace std;

struct FragmentShaderPayload{
    Vec3f position;
    Vec3f color;
    Vec3f texture;
    Vec3f normal;
    Matrix3f TBN;
    FragmentShaderPayload(
        const Vec3f& _position, const Vec3f& _color, 
        const Vec3f& _texture, const Vec3f& _normal, const Matrix3f& _TBN
    ) : position(_position), color(_color), texture(_texture), normal(_normal), TBN(_TBN){}
};
    
class IShader{
public:
    IShader();
    virtual ~IShader();

public:
    bool SetEyePosition(const Vec3f& eye_pos);
    bool SetLights(const vector<LightSource>& lights);

public:
    virtual Vec3f FragmentShader(const FragmentShaderPayload& payload);

protected:
    Vec3f eye_pos_;
    vector<LightSource> lights_;
};

class NormalShader : public IShader{
public:
    NormalShader();
    ~NormalShader();

public:
    Vec3f FragmentShader(const FragmentShaderPayload& payload);

};


class PhongShader : public IShader{
public:  
    PhongShader();
    ~PhongShader();

public:
    Vec3f FragmentShader(const FragmentShaderPayload& payload);

};

class TextureShader : public IShader{
public:
    TextureShader();
    ~TextureShader();
    
public:
    Vec3f FragmentShader(const FragmentShaderPayload& payload);

};

class BumpShader : public IShader{
public:
    BumpShader();
    ~BumpShader();

public:
    Vec3f FragmentShader(const FragmentShaderPayload& payload);
};

class DisplacementShader : public IShader{
public:
    DisplacementShader();
    ~DisplacementShader();

public:
    Vec3f FragmentShader(const FragmentShaderPayload& payload);
};

}
#endif