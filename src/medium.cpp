#include "medium.h"
using namespace std;
std::istream& operator >>(std::istream& fin, Medium& s)
{
    string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        else if (tmp == "lambda") fin >> s.lambda;
        else if (tmp == "aabb") fin >> s.aabb.min[0] >> s.aabb.max[0] >> s.aabb.min[1] >> s.aabb.max[1]>> s.aabb.min[2] >> s.aabb.max[2];
        else {
            cerr << "read medium error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    return fin;
}
bool Medium::checkAABB(Ray ray, double& mint, double& maxt){
    double p;
    double ret = false;
    //slab of x axis
    double t1 = ray.cut(0, aabb.min[0]);
    if(t1 == INF){
        if(aabb.min[0] >= ray.pos.x || ray.pos.x >= aabb.max[0]) return ret;
    }else{
        double t2 = ray.cut(0, aabb.max[0]);
        if(t1 > t2) p = t1, t1 = t2, t2 = p;
        if(t1 > mint) mint = t1;
        if(t2 < maxt) maxt = t2;
    }
    //slab of y axis
    t1 = ray.cut(1, aabb.min[1]);
    if(t1 == INF){
        if(aabb.min[1] >= ray.pos.y || ray.pos.y >= aabb.max[1]) return ret;
    }else{
        double t2 = ray.cut(1, aabb.max[1]);
        if(t1 > t2) p = t1, t1 = t2, t2 = p;
        if(t1 > mint) mint = t1;
        if(t2 < maxt) maxt = t2;
    }
    //slab of z axis
    t1 = ray.cut(2, aabb.min[2]);
    if(t1 == INF){
        if(aabb.min[2] >= ray.pos.z || ray.pos.z >= aabb.max[2]) return ret;
    }else{
        double t2 = ray.cut(2, aabb.max[2]);
        if(t1 > t2) p = t1, t1 = t2, t2 = p;
        if(t1 > mint) mint = t1;
        if(t2 < maxt) maxt = t2;
    }
    if(mint - EPS > maxt) return ret;
    return true;
}
double Medium::collide(const Ray& ray, double maxt)
{
    double mint = 0;
    double ret = -1;
    if(!checkAABB(ray, mint, maxt)) return -1;
    double t = ran();
    if(t < 1 - exp(lambda * (mint - maxt)))
        ret =  mint -log(1 - t) / lambda;
    return ret;
} 

double Medium::getWeight(const Ray& ray, double maxt)
{
    double mint = 0;
    if(!checkAABB(ray, mint, maxt)) return 0;
    return 1 - exp(lambda * (mint - maxt));
} 

std::vector<double> Medium::getPos(const Ray& ray, double maxt, int num){
    double mint = 0;
    checkAABB(ray, mint, maxt);
    vector<double> ret;
    for(int i = 0;i < num;i++)
    {
        double t = ran() * (1 - exp(lambda * (mint - maxt)));
            ret.push_back(mint -log(1 - t) / lambda);
    }
    return ret;
}

