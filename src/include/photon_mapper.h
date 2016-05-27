#pragma once
#include <fstream>
#include "scene.h"
#include "hit_map.h"
#include <algorithm>
#include <utility>
class PhotonMapper
{
public:
    PhotonMapper(std::string);
    Scene scene;
    HitMap hitmap;
    void solve();
    /* data */
    void buildHitMap();
    void calc(Ray& ray, int times, Color absorbing, int xx, int yy, Color weight);
    void photonTrace(const Ray& ray, int times, Color absorbing, Color phi, double r, int d3, long long seed);
};