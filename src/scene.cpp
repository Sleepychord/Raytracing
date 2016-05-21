#include "scene.h"
using namespace std;
std::istream& operator >> (std::istream& fin, Scene& s){
    string tmp;
    while(fin >> tmp){
        if(tmp == "Camera") fin >> s.camera;
        else if(tmp == "PointSource"){
            PointSource* ps = new PointSource();
            fin >> (*ps);
            s.light_source = ps;
        }else if(tmp == "RectSource"){
            RectSource* ps = new RectSource();
            fin >> (*ps);
            s.light_source = ps;            
        }
        else{
            if(tmp == "Sphere"){
                Sphere * new_obj = new Sphere();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);
            }else if( tmp == "Plane"){
                Plane * new_obj = new Plane();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);
            }else if( tmp == "TriangleSet"){
                TriangleSet * new_obj = new TriangleSet();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);
            }else if(tmp == "KdtreeSet"){
                TriangleSet * new_obj = new KdtreeSet();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);                
            }
            else{
                cerr << "read error in scene, tmp is "<<tmp<<endl;
                assert(0);
            }

        }
    }
    return fin;    
}

Object* Scene::firstObject(Ray ray, Collider& c){
    c.collided_num = 0;
    Object* ret = NULL;
    for(int i = 0;i < objs.size(); i++)
    {
        Collider tmp = objs[i]->collide(ray);
        if(tmp.collided_num && (!c.collided_num || tmp.t < c.t - EPS)) c = tmp, ret = objs[i];
    }
    return ret;
}
bool Scene::isBlocked(Ray ray){
    double maxt = light_source->collide(ray).t;
    for(int i = 0;i < objs.size(); i++)
    {
        Collider tmp = objs[i]->collide(ray);
        if(tmp.collided_num && tmp.t < maxt + EPS) return true;
    }
    return false;
}

