#pragma once
#include"../geometry/geometry.h"
namespace LemonCube{

template<typename T>
class Camera{
public:
    Camera(Vec3<T>& look_at, Vec3<T>& pos);

private:
    SetCameraPos(const Vec3<T>& pos);
    SetCameraLookAt(const Vec3<T>& look_at);

};

}