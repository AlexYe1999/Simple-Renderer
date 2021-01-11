#ifndef HITABLE_H_
#define HITABLE_H_
#include"../object/object.h"
#include<vector>
#include<memory>

namespace LemonCube{

class HitableList{
public:
    HitableList() : object_ptrs_(){};
    ~HitableList(){};

public:
    void ClearObject(){object_ptrs_.clear();}
    void AddObjectPtr(const shared_ptr<Hitable>& obj_ptr){object_ptrs_.push_back(obj_ptr);}
    bool HitObject(const Ray& ray, const float& t_min, const float& t_max, HitPointInfo& info) const;
private:
    vector<shared_ptr<Hitable>> object_ptrs_;

};



}

#endif