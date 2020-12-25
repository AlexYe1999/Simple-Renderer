#ifndef SHAPE_H_
#define SHAPE_H_
#include"../texture/texture.h"
#include"geometry.h"
#include<array>
namespace YeahooQAQ{

class Shape{
public:
    Shape(){};
    virtual ~Shape() = 0;
};

class Point : public Shape{
public:
    Point(Vec3f _point, Vec3f _color):point(_point),color(_color){};
    ~Point(){};

public:
    Vec3f point;
    Vec3f color;
};

class Line : public Shape{
public:
    Line(std::array<Vec3f, 2> _vertices, std::array<Vec3f, 2> _colors):vertices(_vertices),colors(_colors){};
    ~Line(){};

public:
    std::array<Vec3f, 2> vertices;
    std::array<Vec3f, 2> colors;
};

class Triangle : public Shape{
public:
    Triangle(Texture* texture = nullptr):vertices_world(),vertices_camera(),colors(),normals_world(),normals_camera(),texture_coords(),texture_ptr(texture){};
    ~Triangle(){};

public:
    std::array<Vec3f, 3> vertices_world;
    std::array<Vec3f, 3> normals_world;
    std::array<Vec3f, 3> vertices_camera;
    std::array<Vec3f, 3> normals_camera;
    std::array<Vec3f, 3> colors;
    std::array<Vec2f, 3> texture_coords;
    Texture* texture_ptr;
};


}

#endif