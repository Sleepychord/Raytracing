#pragma once
#include "source.h"
class PointSource : public Source
{
public:
    PointSource(){}
    ~PointSource(){}

    /* data */
    Vec3 pos;
    virtual std::vector<Ray> getLights(Vec3 aim);
    virtual Collider collide(Ray &);
    virtual Ray getPhoton(long long roundnum);
    friend std::istream& operator >> (std::istream& fin, PointSource& p);
};