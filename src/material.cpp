#include "material.h"
using namespace std;

Material::Material():reflect_percent(0), refract_percent(0), diffuse_percent(1), refract_index(0), absorb_color(Color(0,0,0)), img(0,0),high_light(0){}

std::istream& operator >> (std::istream& fin, Material& m){
    string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "reflect_percent") fin >> m.reflect_percent;
        else if (tmp == "refract_percent") fin >> m.refract_percent;
        else if (tmp == "diffuse_percent") fin >> m.diffuse_percent;
        else if (tmp == "refract_index") fin >> m.refract_index;
        else if (tmp == "absorb_color") fin >> m.absorb_color;
        else if (tmp == "high_light") fin >> m.high_light;
        else if (tmp == "img") {
            string filename;
            fin >> filename;
            m.img.read(filename);
            fin >> m.img.h >> m.img.w;
        }
        else {
            cerr<< "read material error, string is "<<tmp<<endl;
            assert(0);
        }
    }
    return fin;
}
