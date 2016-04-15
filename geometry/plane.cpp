#include "plane.h"
using namespace std;
Collider Plane::collide(Ray& ray){
    Collider ret;
    double dis_per_t = ray.dir / normal_vector;
    if(fabs(dis_per_t) < EPS){
        ret.is_collided = false;
        return ret;
    }
    double distance = (base - ray.pos) / normal_vector;
    double t = distance / dis_per_t;
    if(t < EPS){
        ret.is_collided = false;
        return ret;
    }else {
        ret.is_collided = true;
        ret.t = t;
        if(dis_per_t < 0)
            ret.normal_vector = normal_vector;
        else ret.normal_vector = normal_vector * -1;
    }
    return ret;
}
Color Plane::getTexture(const Vec3& pos){
    return Color(1,1,1);//FIXME
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
        else if (tmp == "normal_vector") fin >> p.normal_vector;
        else {
            cerr << "read plane error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    return fin;
}