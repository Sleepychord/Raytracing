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

    Vec3 operator +(const Vec3& b)const;
    Vec3 operator -(const Vec3& b)const;
    Vec3 operator *(const Vec3& b)const;//cross
    double operator /(const Vec3& b)const;//dot 
    Vec3 operator *(double k)const;
    Vec3 operator /(double k)const;
    double mod()const;
    double mod2()const;
    Vec3 unitize()const;
    Vec3 reflect(Vec3& n)const;
    bool refract(Vec3& n, double refract_index, Vec3& output)const;
    Vec3 rotate(Vec3 axis, double alpha);
    friend std::istream& operator >>(std::istream& , Vec3& );

};