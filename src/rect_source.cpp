#include "rect_source.h"
using namespace std;
//0 3
//1 2
std::vector<Ray> RectSource::getLights(Vec3 aim){
    std::vector<Ray> ret;
    static const int SAMPLE_NUM = 8;
    for(int i = 0;i < SAMPLE_NUM;i++)
        for(int j = 0;j < SAMPLE_NUM;j++)
        {
            double x = (i + ran())/SAMPLE_NUM, y = (j + ran())/SAMPLE_NUM;
            Vec3 pos = vertices[0] * x * y + vertices[1] * x *(1 - y)
                       + vertices[2] * (1 - x) * (1 - y) + vertices[3] * (1 - x) * y;
            ret.push_back(Ray(aim, pos - aim));           
        }
    return ret;
}
Collider RectSource::collide(Ray & ray){
    Collider ret = p.collide(ray);
    if(!ret.collided_num) return ret;
    Vec3 pos = ray.pos + ray.dir * ret.t;
    auto last = (vertices[3] - pos) * (vertices[0] - pos);
    for(int i = 0;i<3;i++){
        auto now = (vertices[i] - pos) * (vertices[i + 1] - pos);
        if(now.mod() < EPS) continue;
        if(last.mod() < EPS) {last = now; continue;}
        if( now / last < -EPS) {ret.collided_num = 0;return ret;}
    }

    return ret;
}

 std::istream& operator >> (std::istream& fin, RectSource& p){
   string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "vertices") {
            for(int i = 0;i < 4;i++)
                fin >> p.vertices[i];
        }
        else if(tmp == "color") fin >> p.color;
        else {
            cerr << "read point_source error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    p.p = Plane(p.vertices[0],(p.vertices[1] - p.vertices[0])*(p.vertices[2] - p.vertices[0]));
    return fin;
}