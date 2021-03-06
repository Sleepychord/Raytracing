#pragma once
#include "object.h"
#include <cstring>
class Sphere : public Object
{
public:
    Vec3 o;
    double r;
    Sphere(Vec3 _o, double _r):o(_o), r(_r), Object(NULL){}
    Sphere(){}
    ~Sphere(){}

    virtual Collider collide(Ray&);
    virtual Color getTexture(Vec3& pos, int trinum = 0);
    friend std::istream& operator >>(std::istream& fin, Sphere&);
};