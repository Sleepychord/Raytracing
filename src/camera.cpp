#include "camera.h"
using namespace std;
std::istream& operator >> (std::istream& fin, Camera& c){
    string tmp;
    double mw, mh;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "lens") fin >> c.lens;
        else if (tmp == "h") fin >>c.h;
        else if (tmp == "w") fin >>c.w;
        else if (tmp == "center") fin >> c.center;
        else if (tmp == "width") fin >> mw;
        else if (tmp == "height") fin >> mh;
        else {
            cerr << "read camera error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    c.film = new Image(c.h, c.w);
    c.width = (( c.lens -c.center) * Vec3(0,0,-1)).unitize() * mw;
    c.height = (( c.lens -c.center) * c.width).unitize() * mh;
    return fin;
}
Vec3 Camera::getPoint(double x, double y){
    Vec3 ret = center + height * (x / h - 0.5)
                + width * (y / w - 0.5) ;
    return ret; 
}

Vec3 Camera::getSamplePoint(double x, double y, int su, long long seed){
    return center + height * ((x + hal(su, seed) - 0.5) / h - 0.5) + width * ((y + hal(su, seed) - 0.5) / w - 0.5);
}

 