#ifndef SHAPE_H_
#define SHAPE_H_
#include"../texture/texture.h"
#include"geometry.h"
#include<array>
#include<memory>
namespace LemonCube{

template<typename T>
class Shape{
public:
    Shape(){};
    virtual ~Shape(){};
};

template<typename T>
class Point : public Shape<T>{
public:
    Point(Vec3<T>& _point, Vec3<T>& _color):point(_point),color(_color){};
    ~Point(){};

public:
    Vec3<T> point;
    Vec3<T> color;
};

template<typename T>
class Line : public Shape<T>{
public:
    Line(std::array<Vec3<T>, 2>& _vertices, std::array<Vec3<T>, 2>& _colors):vertices(_vertices),colors(_colors){};
    ~Line(){};

public:
    std::array<Vec3<T>, 2> vertices;
    std::array<Vec3<T>, 2> colors;
};

template<typename T>
class Triangle : public Shape<T>{
public:
    Triangle(shared_ptr<Texture> texture = nullptr)
        : 
        is_triangle_culled_(false), 
        vertices_world(),vertices_camera(),colors(),
        normals(),texture_coords(),texture_ptr(texture){};
    ~Triangle(){};

public:
    bool is_triangle_culled_;
    std::array<Vec3<T>, 3> vertices_world;
    std::array<Vec3<T>, 3> vertices_camera;
    std::array<Vec3<T>, 3> normals;
    std::array<Vec3<T>, 3> colors;
    std::array<Vec2<T>, 3> texture_coords;
    shared_ptr<Texture> texture_ptr;
};


}

#endif