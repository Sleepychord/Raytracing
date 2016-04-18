#pragma once
#include <fstream>
#include "scene.h"
class Raytracer
{
public:
    Raytracer(std::string);
    Scene scene;
    Color calc(Ray&, int, double&, bool);
    void solve();
    /* data */
};