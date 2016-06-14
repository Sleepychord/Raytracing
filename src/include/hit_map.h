#include "kdtree_set.h"
#include <thread>
#include <mutex>
#include <vector>
struct HitPoint
{
    Vec3 pos;
    Vec3 normal_vector;
    Vec3 in_vector;
    int imgx, imgy;
    Color sum;
    Color weight;
    int n;
    HitPoint():n(0),sum(0,0,0){}
};

class HitMap{
public:
    std::mutex* mlock;
    std::vector<HitPoint> points;
    Node * root;//different kdtree share the same node class
    void build();
    void buildtree(Node * x, const std::vector<int>&);
    void update(Node * x, Vec3 pos, Color phi, Vec3 dir, double r);
};