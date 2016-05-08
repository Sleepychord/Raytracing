#pragma once
#include "vec3.h"
#include "const.h"
#include "color.h"
#include "image.h"
#include "ray.h"
#include "material.h"
struct Collider
{
    int collided_num;
    double t;
    Vec3 normal_vector;
};

class  Object
{
public:
     Object():material(NULL){};
     Object(Material* _m):material(_m){}
    virtual ~Object(){delete material;}
    Material* material;
    virtual Collider collide( Ray&) = 0; 
    virtual Color getTexture(Vec3& pos) = 0;
};