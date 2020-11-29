#ifndef GEOMETRY_H__
#define GEOMETRY_H__
#include<iostream>

namespace YeahooQAQ{
using namespace std;

template<typename T> 
struct Vec2{
    union{
        struct {T x, y;};
        struct {T u, v;};
        T col[2];
    };
    Vec2() : x(0), y(0){}
    Vec2(T _x, T _y) : x(_x), y(_y){}
    inline Vec2<T> operator +(const Vec2<T>& vec) const { return Vec2<T>(x + vec.x, y + vec.y);}
    inline Vec2<T> operator -(const Vec2<T>& vec) const { return Vec2<T>(x - vec.x, y - vec.y);}
    inline Vec2<T> operator *(const float f) const { return Vec2<T>(x * f, y * f);}
    template<typename> friend ostream& operator <<(ostream& os, const Vec2<T>& vec2);
};

template<typename T>
struct Vec3{
    union{
        struct {T x, y, z;};
        struct {T vertex, uv, normal;};
        T col[3];
    };
    Vec3() : x(0), y(0), z(0){}
    Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z){}
    inline Vec3<T> operator +(const Vec3<T>& vec) const { return Vec3<T>(x + vec.x, y + vec.y, z + vec.z);}
    inline Vec3<T> operator -(const Vec3<T>& vec) const {return Vec3<T>(x - vec.x, y - vec.y, z - vec.z);}
    inline Vec3<T> operator *(const float& f) const {return Vec3<T>(f * x, f * y, f * z);}
    template<typename> friend ostream& operator <<(ostream& os, const Vec3<T>& vec3);
};

typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vece3d;

template <typename T> ostream& operator <<(ostream& os, Vec2<T> vec2){
    os << "( " << vec2.x << " , " << vec2.y << " )\n";
    return os;
}

template <typename T> ostream& operator <<(ostream& os, Vec3<T> vec3){
    os << "( " << vec3.x << " , " << vec3.y << " , " << vec3.z << " )\n";
    return os;
}


}

#endif