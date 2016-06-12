#pragma once
#include "triangle_set.h"
#include <cmath>
struct Node
{
    Node * l, *r;
    std::vector<int> data;
    double split;
    int dimension;
    Node():l(NULL),r(NULL){}
    ~Node(){
        delete l;
        delete r;
    }
};
struct AABB
{
    double max[3],min[3];//AABB
};
class KdtreeSet : public TriangleSet
{
    std::vector<std::pair<double, int> > points;
    std::vector<int> used;
    std::vector<Vec3> normal_v;
public:
    Node * root;
    AABB box;
    virtual Collider collide(Ray&);
    virtual void build();
    void buildtree(Node* x,const std::vector<int>&, AABB b);
    Collider search(Node * x, Ray& r, double t1, double t2);
};
