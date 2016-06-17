#include "raytracer.h"
using namespace std;
    
Raytracer::Raytracer(std::string filename)
{
    ifstream fin(filename.c_str());
    fin >> scene;
}

void Raytracer::solve(){
    for(int i = 0;i < scene.camera.film->getN();i++)
        for(int j = 0;j < scene.camera.film->getM();j++)
        {
            double dist = 0;
            if(j == 0) cerr<< "solve line "<<i<<endl;
            if(i == 600 && j == 301) 
                dist = 0;
            Ray sight = Ray(scene.camera.lens, scene.camera.lens - scene.camera.getPoint(i, j) );

            scene.camera.film->setColor(i,j,calc(sight, 0, dist, false));
        }
        
    vector<pair<int, int>> nodes;
    for(int i = 1;i < scene.camera.film->getN();i++)
        for(int j = 1;j < scene.camera.film->getM();j++)
        {
            Color diff = scene.camera.film->getColor(i,j) - scene.camera.film->getColor(i - 1, j);
            if(diff.r * diff.r + diff.g * diff.g + diff.b * diff.b > 0.15)
            {
                nodes.push_back(make_pair(i,j));
                nodes.push_back(make_pair(i - 1,j));
            }
            diff = scene.camera.film->getColor(i,j) - scene.camera.film->getColor(i, j - 1);
            if(diff.r * diff.r + diff.g * diff.g + diff.b * diff.b > 0.15)
            {
                nodes.push_back(make_pair(i,j));
                nodes.push_back(make_pair(i, j - 1));
            }
        }
    sort(nodes.begin(), nodes.end());
    for(int i = 0;i < nodes.size();i++)
        if(i == 0 || nodes[i] != nodes[i - 1]){
            const int SAMPLENUM = 4;
            Color final(0,0,0);
            for(int u = 0; u < SAMPLENUM; u++)
                for(int v = 0;v < SAMPLENUM; v++){
                    double x = nodes[i].first + (u - SAMPLENUM/2.0 + ran()) / SAMPLENUM;
                    double y = nodes[i].second + (v - SAMPLENUM/2.0 + ran()) / SAMPLENUM;
                    Ray sight = Ray(scene.camera.lens, scene.camera.lens - scene.camera.getPoint(x, y) );//FIXME
                    double dist = 0;
                    final = final + calc(sight, 0, dist, false);
                }
            scene.camera.film->setColor(nodes[i].first, nodes[i].second, final / (SAMPLENUM * SAMPLENUM));
        cerr << "optimized "<< i << " / "<< nodes.size()<<endl;
        }
    
    scene.camera.film-> save("./result/result.jpg");
}
Color Raytracer::calc(Ray& ray, int times, double& dist, bool in_refract){
    //find collider
    Collider c, d = scene.light_source -> collide(ray);
    Object * obj = scene.firstObject(ray, c);
    Color ans(0, 0, 0);
    if(d.collided_num && (!c.collided_num || d.t - EPS < c.t) ){
        ans = scene.light_source->color;
        return ans;
    }
    if(!c.collided_num) return ans;
    double collide_dist = 1e10;
    // if collider found  
    if(c.normal_vector / ray.dir > 0) c.normal_vector = c.normal_vector * -1;
    double index = 0;
    Vec3 pos = ray.pos + ray.dir * c.t;
    Ray reflect_ray(pos, ray.dir.reflect(c.normal_vector));
    if(c.m-> diffuse_percent > EPS){
        std::vector<Ray> lights = scene.light_source->getLights(pos);
        //phong
        for(int i = 0;i < lights.size();i++){
            if(!scene.isBlocked(lights[i]))        //shade
            index += abs(c.normal_vector.unitize() / lights[i].dir);
        }

        index = c.m->diffuse_percent * index / lights.size();//diffusion
        index += pow(lights[0].dir / reflect_ray.dir, 50) * c.m->high_light;//high_light
    }
    if(index > 1) index = 1;
    Color ret(index, index, index);
    ret = ret * scene.light_source->color * obj->getTexture(pos, c.collided_num - 1);
    // refraction
    double reflect_percent = c.m->reflect_percent;
    if(c.m->refract_percent > EPS && times < 10){
        Ray refract_ray(pos,Vec3(0,0,0));
        if(ray.dir.refract(c.normal_vector, in_refract?c.m->refract_index: 1/c.m->refract_index, refract_ray.dir)){
            Color remote_color = calc(refract_ray, times + 1, collide_dist, !in_refract);
            if(in_refract) collide_dist = 0;
            //cout<<collide_dist<<" "<<remote_color.r <<" "<<remote_color.g<<endl;
            remote_color = remote_color * (c.m->absorb_color * -collide_dist).Exp();
            ret += remote_color * c.m->refract_percent;
        }
        else reflect_percent += c.m->refract_percent;
    }
    // reflection
    if(reflect_percent > EPS && times < 10){
        ret += reflect_percent * calc(reflect_ray, times + 1, collide_dist, in_refract)* obj->getTexture(pos, c.collided_num - 1);
    }
    ret.Confine();
    dist = c.t;
    return ret ;
}
