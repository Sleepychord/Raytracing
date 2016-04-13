#pragma once
#include "const.h"
class Vec3{
public:
    int x, y, z;
    Vec3(int _x, int _y, int _z):x(_x), y(_y), z(_z){}
    ~Vec3(){}

    Vec3 operator +(const Vec3& b);
    Vec3 operator -(const Vec3& b);
    Vec3 operator *(const Vec3& b);//cross
    Vec3 operator /(const Vec3& b);//dot 
    Vec3 operator *(const double& k);
    Vec3 operator /(const double& k);
    double mod()const;
    Vec3 unitize()const;
}