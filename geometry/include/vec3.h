#pragma once
#include "const.h"
#include <cmath>
#include <iostream>
class Vec3{
public:
    int x, y, z;
    Vec3(){}
    Vec3(int _x, int _y, int _z):x(_x), y(_y), z(_z){}
    ~Vec3(){}

    Vec3 operator +(const Vec3& b);
    Vec3 operator -(const Vec3& b);
    Vec3 operator *(const Vec3& b);//cross
    Vec3 operator /(const Vec3& b);//dot 
    Vec3 operator *(double k);
    Vec3 operator /(double k);
    double mod();
    Vec3 unitize();
    Vec3 reflect(const Vec3& n);
    Vec3 rotate(Vec3 axis, double alpha);
    friend std::istream& operator >>(std::istream& , Vec3& );

};