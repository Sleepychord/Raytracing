#include "photon_mapper.h"
using namespace std;
const double beer_const = 0.1;
PhotonMapper::PhotonMapper(std::string filename)
{
    ifstream fin(filename.c_str());
    fin >> scene;
}
void PhotonMapper::buildHitMap(){
    for(int i = 0;i < scene.camera.film->getN();i++)
        for(int j = 0;j < scene.camera.film->getM();j++)
        {
            double dist = 0;
            if(j == 0) cerr<< "solve line "<<i<<endl;
            if(i == 600 && j == 301) 
                dist = 0;
            Ray sight = Ray(scene.camera.lens, scene.camera.lens - scene.camera.getPoint(i, j) );
            calc(sight, 0, Color(0, 0, 0), i, j, Color(1, 1, 1));
        }
    hitmap.build();
}

void PhotonMapper::calc(Ray& ray, int times, Color absorbing, int xx, int yy, Color weight){
    //find collider
    Collider c, d = scene.light_source -> collide(ray);
    Object * obj = scene.firstObject(ray, c);
    if(d.collided_num && (!c.collided_num || d.t - EPS < c.t) ){
        scene.camera.film->setColor(xx, yy, scene.camera.film->getColor(xx, yy) + scene.light_source->color * weight);
        return ;
    }
    if(!c.collided_num) return ;
    // if collider found  
    Vec3 pos = ray.pos + ray.dir * c.t;
    bool from_out = (c.normal_vector / ray.dir < 0);
    Vec3 normal_vector = from_out? c.normal_vector: c.normal_vector * -1;//real normal_vector
    weight = weight * (absorbing * (-beer_const) * (ray.pos - pos).mod()).Exp();
    Ray reflect_ray(pos, ray.dir.reflect(normal_vector));
    if(obj->material-> diffuse_percent > EPS){
        HitPoint e;
        e.weight = weight * obj->material->diffuse_percent * obj->getTexture(pos);
        e.pos = pos;
        e.normal_vector = normal_vector;
        e.in_vector = ray.dir;
        e.imgx = xx;
        e.imgy = yy;
        hitmap.points.push_back(e);
    }
    // refraction
    double reflect_percent = obj->material->reflect_percent;
    if(obj->material->refract_percent > EPS && times < 10){
        Ray refract_ray(pos,Vec3(0,0,0));
        double next_ri = from_out? 1.0/obj->material->refract_index: obj->material->refract_index;
        if(ray.dir.refract(normal_vector, next_ri, refract_ray.dir)){
            calc(refract_ray, times + 1, from_out?obj->material->absorb_color: Color(0, 0, 0), xx, yy, weight * obj->material->refract_percent);
            //FIXME
        }
        else reflect_percent += obj->material->refract_percent;
    }
    // reflection
    if(reflect_percent > EPS && times < 10){
        calc(reflect_ray, times + 1, absorbing, xx, yy, weight * reflect_percent * obj->getTexture(pos));
    }
}

void PhotonMapper::solve(){
    vector< vector<Color> > colormap(scene.camera.film->getN(), std::vector<Color>(scene.camera.film->getM(), Color(0, 0, 0)));
    vector< vector<Color> > directmap(scene.camera.film->getN(), std::vector<Color>(scene.camera.film->getM(), Color(0, 0, 0)));
    buildHitMap();
    for(int i = 0;i < scene.camera.film->getN();i++)
        for(int j = 0;j < scene.camera.film->getM();j++)
            directmap[i][j] = scene.camera.film->getColor(i, j);
    cerr<< "finish buildHitMap"<<endl;
    const int rounds = 2, photon_num = 100000;
    double r = 1;
    for(int rd = 0; rd < rounds; rd++){
        cout << "new round "<<rd <<endl;
        for(int i = 0;i < photon_num;i++){
            int seed = rd * photon_num + i;
            photonTrace(scene.light_source->getPhoton(seed), 0, Color(0,0,0), scene.light_source->color, r, 4, seed);
            //cout << "emit a photon"<<endl;
        }
        for(int i = 0;i < scene.camera.film->getN();i++)
            for(int j = 0;j < scene.camera.film->getM();j++){
                colormap[i][j] = colormap[i][j] * rd / (rd + 1);
            }
        double source_energy = 1000;
        for(auto & h: hitmap.points){
            colormap[h.imgx][h.imgy] += h.sum  * h.weight * source_energy / (r * r * photon_num * (rd + 1));
            //cerr << h.sum.r <<" "<<h.sum.g << " "<<h.sum.b <<endl;
            h.sum = 0;
            h.n = 0;
        }
        r = r * sqrt((rd + 0.7) / (rd + 1));
        //save img
        for(int i = 0;i < scene.camera.film->getN();i++)
            for(int j = 0;j < scene.camera.film->getM();j++){
                Color tmp = colormap[i][j] + directmap[i][j];
                tmp.Confine();
                scene.camera.film->setColor(i, j, tmp);
            }
        scene.camera.film->save("./result/result.jpg");
    }
}

void PhotonMapper::photonTrace(const Ray& ray, int times, Color absorbing, Color phi, double r, int d3, int seed){
    Collider c;
    Object * obj = scene.firstObject(ray, c);
    if(!c.collided_num) return;
    Vec3 pos = ray.pos + ray.dir * c.t;
    bool from_out = (c.normal_vector / ray.dir < 0);
    Vec3 normal_vector = from_out? c.normal_vector: c.normal_vector * -1;//real normal_vector 
    phi = phi * (absorbing * (-beer_const) * (ray.pos - pos).mod()).Exp();// beer
    double p = hal(d3, seed);
    if(p < obj->material->diffuse_percent){
        hitmap.update(hitmap.root, pos, phi, ray.dir, r);
        if(times < 10){
            phi = phi * obj->getTexture(pos);
            if(phi.r > phi.g && phi.r > phi.b){double t = 1.0 / phi.r; phi = phi * t;}
            else if(phi.g > phi.r && phi.g > phi.b){double t = 1.0 / phi.g; phi = phi * t;}
            else {double t = 1.0 / phi.b; phi = phi * t;}
            //-----------------
            double r1 = 2. * PI * hal(d3 + 1, seed), r2 = hal(d3 + 2, seed);
            double r2s = sqrt(r2);
            Vec3 w = normal_vector, u = ((fabs(w.x) > .1 ? Vec3(0,1,0) : Vec3(1,0,0)) * w).unitize();
            Vec3 v = w * u, d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1-r2)).unitize();
            photonTrace(Ray(pos, d), times + 1, absorbing, phi, r, d3 + 3, seed);
        }
    }else if (p < obj->material->diffuse_percent + obj->material->refract_percent){
        if(times < 10){
            Ray refract_ray(pos,Vec3(0,0,0));
            double next_ri = from_out? 1.0/obj->material->refract_index: obj->material->refract_index;
            if(ray.dir.refract(normal_vector, next_ri, refract_ray.dir))
                photonTrace(refract_ray, times + 1, from_out?obj->material->absorb_color: Color(0, 0, 0), phi, r, d3 + 3, seed);
            else
            {    
                Ray reflect_ray(pos, ray.dir.reflect(normal_vector));
                phi = phi * obj->getTexture(pos);
                if(phi.r > phi.g && phi.r > phi.b){double t = 1.0 / phi.r; phi = phi * t;}
                else if(phi.g > phi.r && phi.g > phi.b){double t = 1.0 / phi.g; phi = phi * t;}
                else {double t = 1.0 / phi.b; phi = phi * t;}

                photonTrace(reflect_ray, times + 1, absorbing, phi, r, d3 + 3, seed);
            }
        }else if (p < obj->material->diffuse_percent + obj->material->refract_percent + obj->material->reflect_percent && times < 10){

                Ray reflect_ray(pos, ray.dir.reflect(normal_vector));
                phi = phi * obj->getTexture(pos);
                if(phi.r > phi.g && phi.r > phi.b){double t = 1.0 / phi.r; phi = phi * t;}
                else if(phi.g > phi.r && phi.g > phi.b){double t = 1.0 / phi.g; phi = phi * t;}
                else {double t = 1.0 / phi.b; phi = phi * t;}

                photonTrace(reflect_ray, times + 1, absorbing, phi, r, d3 + 3, seed);
        }
    }
}