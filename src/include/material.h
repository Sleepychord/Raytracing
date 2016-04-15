#pragma once
#include "vec3.h"
#include "image.h"
#include "color.h"
#include <cstring>
class  Material
{
public:
    Material();
    ~ Material();
    double reflect_percent, refract_percent, diffuse_percent, refract_index;
    Color absorb_color;
    Image img;
    friend std::istream& operator >> (std::istream& fin, Material&);
};