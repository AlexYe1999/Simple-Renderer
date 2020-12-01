#ifndef GEOMETRY_H__
#define GEOMETRY_H__
#include<iostream>

namespace YeahooQAQ{
using namespace std;


template<typename T>
struct Vec4{
    union{
        struct {T x, y, z, w;};
        struct {T vertex, uv, normal, none;};
    };
    Vec4() : x(0), y(0), z(0),w(0){}
    Vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w){}
    inline Vec4<T> operator +(const Vec4<T>& vec) const { return Vec4<T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
    inline Vec4<T> operator -(const Vec4<T>& vec) const {return Vec4<T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
    inline Vec4<T> operator *(const T n) const {return Vec4<T>(n * x, n * y, n * z, n * w); };
    inline T operator *(const Vec4<T>& vec) const {return T(x * vec.x + y * vec.y + z * vec.z, w * vec.w);}
    inline T& operator [](const int index) {if(index == 0) {return x;}else if(index == 1){ return y;}else if(index == 2){ return z;}else{return w;}}
    template<typename> friend ostream& operator <<(ostream& os, const Vec4<T>& vec4);
};


template<typename T>
struct Vec3{
    union{
        struct {T x, y, z;};
        struct {T vertex, uv, normal;};
    };
    Vec3() : x(0), y(0), z(0){}
    Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z){}
    inline Vec3<T> operator +(const Vec3<T>& vec) const { return Vec3<T>(x + vec.x, y + vec.y, z + vec.z);}
    inline Vec3<T> operator -(const Vec3<T>& vec) const {return Vec3<T>(x - vec.x, y - vec.y, z - vec.z);}
    inline Vec3<T> operator *(const T n) const {return Vec3<T>(n * x, n * y, n * z);};
    inline  Vec3<T> cross(const Vec3<T>& vec) const {return Vec3<T>(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);}
    inline T operator *(const Vec3<T>& vec) const {return T(x * vec.x + y * vec.y + z * vec.z);}
    inline T& operator [](const int index) {if(index == 0) {return x;}else if(index == 1){ return y;}else if(index == 2){ return z;}}
    inline Vec4<T> toVec4(const T w){ return Vec4<T>(x, y, z, w);}
    inline Vec4<T> toVec4(){ return Vec4<T>(x, y, z, T(0));}
    template<typename> friend ostream& operator <<(ostream& os, const Vec3<T>& vec3);
};

template<typename T>
struct Vec2{
    union{
        struct {T x, y;};
        struct {T u, v;};
    };
    Vec2() : x(0), y(0){}
    Vec2(T _x, T _y) : x(_x), y(_y){}
    inline Vec2<T> operator +(const Vec2<T>& vec) const { return Vec2<T>(x + vec.x, y + vec.y);}
    inline Vec2<T> operator -(const Vec2<T>& vec) const { return Vec2<T>(x - vec.x, y - vec.y);}
    inline Vec2<T> operator *(const T n) const { return Vec2<T>(n * x, n * y);}
    inline T operator *(const Vec2<T>& vec) const {return T(x*vec.x + y*vec.y);}
    inline T cross(const Vec2<T>& vec) const {return T(x * vec.y - y * vec.x);}
    inline T& operator [](const int index) {if(index == 0){ return x; }else if(index == 1){ return y;}}
    inline Vec3<T> toVec3(const T z){ return Vec3<T>(x, y, z);}
    inline Vec3<T> toVec3(){ return Vec3<T>(x, y, T(0));}
    template<typename> friend ostream& operator <<(ostream& os, const Vec2<T>& vec2);
};


typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vece3d;
typedef Vec4<int> Vec4i;
typedef Vec4<float> Vec4f;
typedef Vec4<double> Vece4d;


template <typename T> 
ostream& operator <<(ostream& os, const Vec2<T>& vec2){
    os << "( " << vec2.x << " , " << vec2.y << " )\n";
    return os;
}

template <typename T> 
ostream& operator <<(ostream& os, const  Vec3<T>& vec3){
    os << "( " << vec3.x << " , " << vec3.y << " , " << vec3.z << " )\n";
    return os;
}

template <typename T> 
ostream& operator <<(ostream& os, const Vec4<T>& vec4){
    os << "( " << vec4.x << " , " << vec4.y << " , " << vec4.z << ", " << vec4.w << ")\n";
    return os;
}

template<typename T>
struct Matrix4{
    Vec4<T> vec[4];
    Matrix4() : vec{Vec4<T>(), Vec4<T>(), Vec4<T>(), Vec4<T>()}{};
    Matrix4(Vec4<T> vec1, Vec4<T> vec2, Vec4<T> vec3, Vec4<T> vec4) : vec{vec1, vec2, vec3, vec4}{};
    inline Matrix4<T> operator +(const Matrix4<T>& m4) const{return Matrix4<T>(vec[0] + m4.vec[0], vec[1] + m4.vec[1], vec[2] + m4.vec[2], vec[3] + m4.vec[3]);}
    inline Matrix4<T> operator -(const Matrix4<T>& m4) const{return Matrix4<T>(vec[0] - m4.vec[0], vec[1] - m4.vec[1], vec[2] - m4.vec[2], vec[3] - m4.vec[3]);}
    inline Matrix4<T> operator *(const T n) const{return Matrix4<T>(n * vec[0], n * vec[1], n * vec[2], n * vec[3]);}
    inline Vec4<T>& operator [](const int index) {return vec[index];}
    template<typename> friend ostream& operator <<(ostream& os, const Matrix4<T>& m4);
    inline Matrix4<T> operator *(const Matrix4<T>& m4) const{
        Vec4<T> vec1 = {
            vec[0].x * m4.vec[0].x + vec[1].x * m4.vec[0].y + vec[2].x * m4.vec[0].z + vec[3].x * m4.vec[0].w, 
            vec[0].y * m4.vec[0].x + vec[1].y * m4.vec[0].y + vec[2].y * m4.vec[0].z + vec[3].y * m4.vec[0].w,
            vec[0].z * m4.vec[0].x + vec[1].z * m4.vec[0].y + vec[2].z * m4.vec[0].z + vec[3].z * m4.vec[0].w,
            vec[0].w* m4.vec[0].x + vec[1].w *m4.vec[0].y + vec[2].w *m4.vec[0].z + vec[3].w* m4.vec[0].w
        };
        Vec4<T> vec2 = {
            vec[0].x * m4.vec[1].x + vec[1].x * m4.vec[1].y + vec[2].x * m4.vec[1].z + vec[3].x * m4.vec[1].w,
            vec[0].y * m4.vec[1].x + vec[1].y * m4.vec[1].y + vec[2].y * m4.vec[1].z+ vec[3].y * m4.vec[1].w,
            vec[0].z * m4.vec[1].x + vec[1].z * m4.vec[1].y + vec[2].z * m4.vec[1].z + vec[3].z * m4.vec[1].w,
            vec[0].w* m4.vec[1].x + vec[1].w* m4.vec[1].y+ vec[2].w* m4.vec[1].z + vec[3].w *m4.vec[1].w
        };
        Vec4<T> vec3 = {
            vec[0].x * m4.vec[2].x + vec[1].x * m4.vec[2].y + vec[2].x * m4.vec[2].z + vec[3].x * m4.vec[2].w,
            vec[0].y * m4.vec[2].x + vec[1].y * m4.vec[2].y + vec[2].y * m4.vec[2].z + vec[3].y * m4.vec[2].w,
            vec[0].z * m4.vec[2].x + vec[1].z * m4.vec[2].y + vec[2].z * m4.vec[2].z + vec[3].z * m4.vec[2].w,
            vec[0].w* m4.vec[2].x + vec[1].w* m4.vec[2].y+ vec[2].w* m4.vec[2].z + vec[3].w* m4.vec[2].w
        };
        Vec4<T> vec4 = {
            vec[0].x * m4.vec[3].x + vec[1].x * m4.vec[3].y + vec[2].x * m4.vec[3].z + vec[3].x * m4.vec[3].w,
            vec[0].y * m4.vec[3].x + vec[1].y * m4.vec[3].y + vec[2].y * m4.vec[3].z + vec[3].y * m4.vec[3].w,
            vec[0].z * m4.vec[3].x + vec[1].z * m4.vec[3].y + vec[2].z * m4.vec[3].z + vec[3].z * m4.vec[3].w,
            vec[0].w* m4.vec[3].x + vec[1].w* m4.vec[3].y+ vec[2].w* m4.vec[3].z + vec[3].w* m4.vec[3].w
        };
        return Matrix4<T>(vec1, vec2, vec3, vec4);
    }    
};

template<typename T>
struct Matrix3{
    Vec3<T> vec[3];
    Matrix3() : vec{Vec3<T>(), Vec3<T>(), Vec3<T>()}{};
    Matrix3(Vec3<T> vec1, Vec3<T> vec2, Vec3<T> vec3) : vec{vec1, vec2, vec3}{};
    inline Matrix3<T> operator +(const Matrix3<T>& m3) const{return Matrix3<T>(vec[0] + m3.vec[0], vec[1] + m3.vec[1], vec[2] + m3.vec[2]);}
    inline Matrix3<T> operator -(const Matrix3<T>& m3) const{return Matrix3<T>(vec[0] - m3.vec[0], vec[1] - m3.vec[1], vec[2] - m3.vec[2]);}
    inline Matrix3<T> operator *(const T n) const{return Matrix3<T>(n * vec[0], n * vec[1], n * vec[2]);}
    inline Vec3<T>& operator [](const int index) {return vec[index];}
    template<typename> friend ostream& operator <<(ostream& os, const Matrix3<T>& m3);
    inline Matrix3<T> operator *(const Matrix3<T>& m3) const{
        Vec3<T> vec1 = {
            vec[0].x * m3.vec[0].x + vec[1].x * m3.vec[0].y + vec[2].x * m3.vec[0].z, 
            vec[0].y * m3.vec[0].x + vec[1].y * m3.vec[0].y + vec[2].y * m3.vec[0].z, 
            vec[0].z * m3.vec[0].x + vec[1].z * m3.vec[0].y + vec[2].z * m3.vec[0].z
        };
        Vec3<T> vec2 = {
            vec[0].x * m3.vec[1].x + vec[1].x * m3.vec[1].y + vec[2].x * m3.vec[1].z, 
            vec[0].y * m3.vec[1].x + vec[1].y * m3.vec[1].y + vec[2].y * m3.vec[1].z, 
            vec[0].z * m3.vec[1].x + vec[1].z * m3.vec[1].y + vec[2].z * m3.vec[1].z
        };
        Vec3<T> vec3 = {
            vec[0].x * m3.vec[2].x + vec[1].x * m3.vec[2].y + vec[2].x * m3.vec[2].z, 
            vec[0].y * m3.vec[2].x + vec[1].y * m3.vec[2].y + vec[2].y * m3.vec[2].z, 
            vec[0].z * m3.vec[2].x + vec[1].z * m3.vec[2].y + vec[2].z * m3.vec[2].z
        };
        return Matrix3<T>(vec1, vec2, vec3);
    }    
};

template<typename T>
struct Matrix2{
    Vec2<T> vec[2];
    Matrix2() : vec{Vec2<T>(), Vec2<T>()}{}
    Matrix2(Vec2<T> vec1, Vec2<T> vec2) : vec{vec1, vec2}{}
    inline Matrix2<T> operator +(const Matrix2<T>& m2) const{return Matrix2<T>(vec[0] + m2.vec[0], vec[1] + m2.vec[1]);}
    inline Matrix2<T> operator -(const Matrix2<T>& m2) const{return Matrix2<T>(vec[0] - m2.vec[0], vec[1] - m2.vec[1]);}
    inline Matrix2<T> operator *(const T n) const{return Matrix2<T>(n * vec[0], n * vec[1]);}
    inline Vec2<T>& operator [](const int index) {return vec[index];}
    template<typename> friend ostream& operator <<(ostream& os, const Matrix2<T>& m2);
    inline Matrix2<T> operator *(const Matrix2<T>& m2) const{
        Vec2<T> vec2 = {
            vec[0].x * m2.vec[0].x + vec[1].x * m2.vec[0].y,
            vec[0].y * m2.vec[0].x + vec[1].y * m2.vec[0].y
        };
        Vec2<T> vec3 = {    
            vec[0].x * m2.vec[1].x + vec[1].x * m2.vec[1].y,
            vec[0].y * m2.vec[1].x + vec[1].y * m2.vec[1].y
        };
        
        return Matrix2<T>(vec2, vec3);
    }    
};

typedef Matrix2<int> Matrix2i;
typedef Matrix2<float> Matrix2f;
typedef Matrix2<double> Matrix2d;
typedef Matrix3<int> Matrix3i;
typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;
typedef Matrix4<int> Matrix4i;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

template<typename T> 
ostream& operator <<(ostream& os, const Matrix2<T>& m2){
    os<<"{\n"; 
    os<<" "<< m2.vec[0].x << " , " << m2.vec[1].x <<" \n";
    os<<" "<< m2.vec[0].y << " , " << m2.vec[1].y <<" \n";
    os<<"}\n";
}

template<typename T> 
ostream& operator <<(ostream& os, const Matrix3<T>& m3){
    os<< "{\n";
    os<<" "<< m3.vec[0].x << " , " << m3.vec[1].x <<" , "<<m3.vec[2].x<<" \n";
    os<<" "<< m3.vec[0].y << " , " << m3.vec[1].y <<" , "<<m3.vec[2].y<<" \n";
    os<<" "<< m3.vec[0].z << " , " << m3.vec[1].z <<" , "<<m3.vec[2].z<<" \n";
    os<<"}\n";
}

template<typename T> 
ostream& operator <<(ostream& os, const Matrix4<T>& m4){
    os<< "{\n";
    os<<" "<< m4.vec[0].x << " , " << m4.vec[1].x <<" , "<< m4.vec[2].x <<" , " << m4.vec[3].x <<" \n";
    os<<" "<< m4.vec[0].y << " , " << m4.vec[1].y <<" , "<< m4.vec[2].y <<" , "<< m4.vec[3].y <<" \n";
    os<<" "<< m4.vec[0].z << " , " << m4.vec[1].z <<" , "<< m4.vec[2].z << " , "<< m4.vec[3].z<<" \n";
    os<<" "<< m4.vec[0].w << " , " <<m4.vec[1].w <<" , "<< m4.vec[2].w << " , "<< m4.vec[3].w<<" \n";
    os<<"}\n";
}

}

#endif