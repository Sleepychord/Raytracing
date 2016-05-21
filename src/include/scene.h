#pragma once
#include "camera.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle_set.h"
#include "point_source.h"
#include "rect_source.h"
#include "kdtree_set.h"
#include <string>
class  Scene
{
public:
     Scene():light_source(NULL){};
     ~Scene(){
        delete light_source;
        for(auto obj : objs)
            delete obj;
     }
    /* data */
    Source* light_source; 
    Camera camera;
    std::vector<Object *> objs;
    friend std::istream& operator >> (std::istream& fin, Scene& s);
    virtual Object* firstObject(Ray, Collider&);
    virtual bool isBlocked(Ray);
};