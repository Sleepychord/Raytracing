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
            if(i == 0 && j == 721)
                cerr<<"2"<<endl;
            Ray sight = Ray(scene.camera.lens, scene.camera.lens - scene.camera.getPoint(i, j) );

            scene.camera.film->setColor(i,j,calc(sight, 0, dist, false));
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
    double index = 0;
    Vec3 pos = ray.pos + ray.dir * c.t;
    Ray reflect_ray(pos, ray.dir.reflect(c.normal_vector));
    if(obj->material-> diffuse_percent > EPS){
        std::vector<Ray> lights = scene.light_source->getLights(pos);
        //phong
        for(int i = 0;i < lights.size();i++){
            if(!scene.isBlocked(lights[i]))        //shade
            index += abs(c.normal_vector.unitize() / lights[i].dir);
        }
        index = obj->material->diffuse_percent * index / lights.size();//diffusion
        index += pow(lights[0].dir / reflect_ray.dir, 50) * obj->material->high_light;//high_light
    }
    if(index > 1) index = 1;
    Color ret(index, index, index);
    ret = ret * scene.light_source->color * obj->getTexture(pos);
    // refraction
    double reflect_percent = obj->material->reflect_percent;
    if(obj->material->refract_percent > EPS && times < 10){
        Ray refract_ray(pos,Vec3(0,0,0));
        if(ray.dir.refract(c.normal_vector, in_refract?obj->material->refract_index: 1/obj->material->refract_index, refract_ray.dir)){
            Color remote_color = calc(refract_ray, times + 1, collide_dist, !in_refract);
            if(in_refract) collide_dist = 0;
            //cout<<collide_dist<<" "<<remote_color.r <<" "<<remote_color.g<<endl;
            remote_color = remote_color * (obj->material->absorb_color * -collide_dist).Exp();
            ret += remote_color * obj->material->refract_percent;
        }
        else reflect_percent += obj->material->refract_percent;
    }
    // reflection
    if(reflect_percent > EPS && times < 10){
        ret += reflect_percent * calc(reflect_ray, times + 1, collide_dist, in_refract)* obj->getTexture(pos);
    }

    dist = c.t;
    return ret ;
}
