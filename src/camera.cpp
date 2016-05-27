#include "camera.h"
using namespace std;
std::istream& operator >> (std::istream& fin, Camera& c){
    string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "lens") fin >> c.lens;
        else if (tmp == "h") fin >>c.h;
        else if (tmp == "w") fin >>c.w;
        else if (tmp == "center") fin >> c.center;
        else if (tmp == "width") fin >> c.width;
        else if (tmp == "height") fin >> c.height;
        else {
            cerr << "read camera error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    c.film = new Image(c.h, c.w);
    return fin;
}
Vec3 Camera::getPoint(double x, double y){
    Vec3 ret = center + height * (x / h - 0.5)
                + width * (y / w - 0.5) ;
    return ret; 
}

std::vector<Vec3> Camera::getSamplePoints(double x, double y, int num){
    vector<Vec3> ret;
    for(int i = 0;i < num;i++)
        ret.push_back(center + height * ((x + ran() - 0.5) / h - 0.5) + width * ((y + ran() - 0.5) / w - 0.5)) ;
    return ret;
}

 