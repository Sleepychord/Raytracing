#pragma once
#include "object.h"
#include <vector>
#include <cstring>
#include <fstream>
struct Triangle
{
    int v0, v1, v2;
    Material * m; 
    Triangle():m(NULL){}   
};
class TriangleSet : public Object
{
public:
    std::vector<Vec3> vertices, normal_vector;
    std::vector<double> imgx;
    std::vector<double> imgy;
    std::vector<Triangle> triangles;
    virtual Collider collide(Ray&);
    virtual void build();
    virtual Color getTexture(Vec3& pos, int trinum = 0);
    friend std::istream& operator >>(std::istream& fin, TriangleSet&);
};