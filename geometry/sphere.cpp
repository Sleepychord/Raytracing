#include "sphere.h"
using namespace std;
std::istream& operator >>(std::istream& fin, Sphere& s)
{
    string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "material") {
            s.material = new Material();
            fin >> (*(s.material));
        }
        else if (tmp == "o") fin >> s.o;
        else if (tmp == "r") fin >> s.r;
        else {
            cerr << "read sphere error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    return fin;
}
Collider Sphere::collide(Ray& ray)
{
    Collider ret;
    double t_pedal = (o - ray.pos) / ray.dir;
    double half_chord = r * r - (o - ray.pos).mod2() + t_pedal * t_pedal;
    if(half_chord < EPS){
        ret.is_collided = false;
        return ret;
    }else half_chord = sqrt(half_chord);
    double t1 = t_pedal - half_chord, t2 = t_pedal + half_chord;
    if(!(t1 > EPS) && !(t2 > EPS)){
        ret.is_collided = false;
        return ret;
    }
    double t;
    if(t1 > EPS) t = t1; else t = t2;
    if(t2 > EPS  && t2 < t) t = t2;
    ret.is_collided = true;
    ret.t = t;
    if((ray.pos - o).mod2() < r + EPS)// in the sphere 
        ret.normal_vector = (o - ray.pos - ray.dir * t ).unitize();
    else ret.normal_vector = (ray.pos + ray.dir * t - o).unitize();
    return ret;
} 
Color Sphere::getTexture(const Vec3& pos){
    return Color(1, 0, 0);//FIXME
}