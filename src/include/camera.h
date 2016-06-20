#pragma once
#include "vec3.h"
#include "color.h"
#include "image.h"
#include "plane.h"
#include <vector>
class Camera
{
public:
    Camera():film(NULL){}
    ~Camera(){delete film;}
    Image* film;
    Vec3 center, height, width, lens;
    double h, w, focus_dis, r;
    friend std::istream& operator >> (std::istream& fin, Camera&);
    Vec3 getPoint(double x, double y);
    Vec3 getSamplePoint(double x, double y, int);
    Ray getDepthRay(double x, double y, int);
}; 