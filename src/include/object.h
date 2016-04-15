#pragma once
#include "vec3.h"
#include "const.h"
#include "color.h"
#include "image.h"
#include "ray.h"
#include "material.h"
struct Collider
{
    bool is_collided;
    double t;
    Vec3 normal_vector;
};

class  Object
{
public:
     Object();
     Object(Material* _m):material(_m){}
    ~ Object();

    Material* material;
    virtual Collider collide(const Ray&) = 0; 
    virtual Color getTexture(const Vec3& pos) = 0;
};