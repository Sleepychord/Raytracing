#pragma once
#include "source.h"
#include "plane.h"
class RectSource : public Source
{
public:
    RectSource(){}
    ~RectSource(){}

    /* data */
    Plane p;
    Vec3 vertices[4];
    virtual std::vector<Ray> getLights(Vec3 aim);
    virtual Collider collide(Ray &);
    virtual Ray getPhoton(int roundnum);
    friend std::istream& operator >> (std::istream& fin, RectSource& p);
};