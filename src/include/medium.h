#pragma once
#include "object.h"
#include "kdtree_set.h"
#include <cstring>
class Medium
{
public:
    double lambda;
    AABB aabb;
    Medium(){}
    ~Medium(){}

    double collide(const Ray&, double);
    double getWeight(const Ray&, double);
    std::vector<double> getPos(const Ray&, double, int);
    bool checkAABB(Ray, double & ,double &);
    friend std::istream& operator >>(std::istream& fin, Medium&);
};