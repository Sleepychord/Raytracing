#pragma once
#include "object.h"
#include <vector>
#include <string>
class Source
{
public:
    Color color;
    /* data */
    virtual std::vector<Ray> getLights(Vec3 aim) = 0;// attention: aim => source
    virtual Collider collide( Ray &) = 0;
    virtual Ray getPhoton(int roundnum) = 0;
    virtual ~Source(){}
};