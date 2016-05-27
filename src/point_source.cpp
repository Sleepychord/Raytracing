#include "point_source.h"
using namespace std;
std::vector<Ray> PointSource::getLights(Vec3 aim){
    std::vector<Ray> ret;
    ret.push_back(Ray(aim, pos - aim));
    return ret;
}
Collider PointSource::collide(Ray & ray){
    Collider ret;
    if(fabs( ((ray.pos - pos) * ray.dir).mod2() < EPS )){
        ret.collided_num = 1;
        ret.t = (pos - ray.pos) / ray.dir;
        return ret;
    }
    else {
        ret.collided_num = 0;
        return ret;
    }
}

Ray PointSource::getPhoton(long long i){
    double p = 2. * PI * hal(0, i), t = 2. * acos(sqrt(1.-hal(1, i)));
        double st = sin(t);
    return Ray(pos, Vec3(cos(p) * st, cos(t), sin(p) * st));
}

 std::istream& operator >> (std::istream& fin, PointSource& p){
   string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "pos") fin >> p.pos;
        else if(tmp == "color") fin >> p.color;
        else {
            cerr << "read point_source error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    return fin;
}