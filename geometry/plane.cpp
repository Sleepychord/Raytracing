#include "plane.h"
using namespace std;
Collider Plane::collide(Ray& ray){
    Collider ret;
    double dis_per_t = ray.dir / normal_vector;
    if(fabs(dis_per_t) < EPS){
        ret.collided_num = 0;
        return ret;
    }
    double distance = (base - ray.pos) / normal_vector;
    double t = distance / dis_per_t;
    if(t < EPS){
        ret.collided_num = 0;
        return ret;
    }else {
        ret.collided_num = 1;
        ret.t = t;
        //if(dis_per_t < 0)
        ret.normal_vector = normal_vector;
        //else ret.normal_vector = normal_vector * -1;
    }
    ret.m = material;
    return ret;
}
Color Plane::getTexture(Vec3& pos, int trinum ){
    if(!material->img.getM())
        return material->color;
    Vec3 xaxis = (normal_vector * Vec3(0, 1, 1)).unitize();
    Vec3 yaxis = (normal_vector * xaxis).unitize();
    return material->img.getColor((pos - base) / xaxis, (pos - base) / yaxis);
}
std::istream& operator >>(std::istream& fin, Plane& p){
    string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "material") {
            p.material = new Material();
            fin >> (*(p.material));
        }
        else if (tmp == "base") fin >> p.base;
        else if (tmp == "normal_vector") {
            fin >> p.normal_vector;
            p.normal_vector = p.normal_vector.unitize();
        }
        else {
            cerr << "read plane error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    return fin;
}