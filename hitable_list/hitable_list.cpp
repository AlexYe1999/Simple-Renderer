#include"hitable_list.hpp"

namespace LemonCube{

bool HitableList::HitObject(const Ray& ray, const float& t_min, const float& t_max, HitPointInfo& info) const{
    bool hit_any = false;
    float closest_time = t_max;
    for(const shared_ptr<Hitable> obj_ptr : object_ptrs_){
        if(obj_ptr->HitObject(ray, t_min, closest_time, info)){
            hit_any = true;
            closest_time = info.time;
        }
    }

    return hit_any;
}


}