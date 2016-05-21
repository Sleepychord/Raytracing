#pragma once
#include "vec3.h"
class Ray
{
public:
    Ray(Vec3 _pos, Vec3 _dir):pos(_pos), dir(_dir.unitize()){}
    Vec3 pos, dir;
    double cut(int d, double split){
        double dis_per_t;
        if(d == 0) 
            dis_per_t = dir / Vec3(1, 0, 0); 
        else if (d == 1) 
            dis_per_t = dir / Vec3(0, 1, 0); 
        else  dis_per_t = dir / Vec3(0, 0, 1); 
        if(fabs(dis_per_t) < EPS) return INF;    
        if(d == 0) 
            return (split - pos.x) / dis_per_t; 
        else if (d == 1) 
            return (split - pos.y) / dis_per_t;
        else return (split - pos.z) / dis_per_t;
    }
};