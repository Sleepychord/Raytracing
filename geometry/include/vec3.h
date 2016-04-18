#pragma once
#include "const.h"
#include <cmath>
#include <iostream>
class Vec3{
public:
    double x, y, z;
    Vec3(){}
    Vec3(double _x, double _y, double _z):x(_x), y(_y), z(_z){}
    ~Vec3(){}

    Vec3 operator +(const Vec3& b);
    Vec3 operator -(const Vec3& b);
    Vec3 operator *(const Vec3& b);//cross
    double operator /(const Vec3& b);//dot 
    Vec3 operator *(double k);
    Vec3 operator /(double k);
    double mod();
    double mod2();
    Vec3 unitize();
    Vec3 reflect(Vec3& n);
    bool refract(Vec3& n, double refract_index, Vec3& output);
    Vec3 rotate(Vec3 axis, double alpha);
    friend std::istream& operator >>(std::istream& , Vec3& );

};