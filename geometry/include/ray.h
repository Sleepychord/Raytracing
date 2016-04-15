#pragma once
#include "vec3.h"
class Ray
{
public:
    Ray(Vec3 _pos, Vec3 _dir):pos(_pos), dir(_dir.unitize()){}
    Vec3 pos, dir;
};