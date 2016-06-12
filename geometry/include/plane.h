#pragma once
#include "object.h"
class Plane : public Object
{
public:
    Plane(){}
    Plane(Vec3 _base, Vec3 _nv):base(_base), normal_vector(_nv){}
    ~Plane(){}

    Vec3 base;
    Vec3 normal_vector;
    virtual Collider collide(Ray&);
    virtual Color getTexture(Vec3& pos, int trinum = 0);
    friend std::istream& operator >>(std::istream& fin, Plane&);
};