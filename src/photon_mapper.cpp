#include "photon_mapper.h"
using namespace std;
const double beer_const = 0.7;
PhotonMapper::PhotonMapper(std::string filename)
{
    ifstream fin(filename.c_str());
    fin >> scene;
}
void PhotonMapper::buildHitMap(){
    srand(time(0));
    const int SAMPLENUM = 4;
    vector<thread> thpool(SAMPLENUM);
    vector<long long> seed;
    for(int i = 0;i < SAMPLENUM;i++)
        seed.push_back(10 + i * 2);
    auto func = [SAMPLENUM, &seed](int x, PhotonMapper* ppm){
        for(int i = 0;i < ppm->scene.camera.film->getN();i++)
            for(int j = 0;j < ppm->scene.camera.film->getM();j++)
            {
                seed[x] += SAMPLENUM * 2;
                double dist = 0;
                if(x == 0 && j == 0) cerr<< "solve line "<<i<<endl;

                //Vec3 p = ppm->scene.camera.getSamplePoint(i, j, x, seed[x]);
                //Ray sight = Ray(ppm->scene.camera.lens, ppm->scene.camera.lens - p );
                Ray sight = ppm->scene.camera.getDepthRay(i, j, x, seed[x]);
                Color cc = Color(1.0 / SAMPLENUM, 1.0 / SAMPLENUM, 1.0 / SAMPLENUM);
                ppm->calc(sight, 0, Color(0, 0, 0), i, j, cc);
            }
    };
    for(int i = 0;i < SAMPLENUM;i++)
        thpool[i] = thread(func, i, this);
    for(int i = 0;i < SAMPLENUM;i++)
        thpool[i].join();
    hitmap.build();
}

void PhotonMapper::calc(Ray& ray, int times, Color absorbing, int xx, int yy, Color weight){
    //find collider
    Collider c, d = scene.light_source -> collide(ray);
    Object * obj = scene.firstObject(ray, c);
    if(d.collided_num && (!c.collided_num || d.t - EPS < c.t) ){
        hitmap.mlock->lock();
        scene.camera.film->setColor(xx, yy, scene.camera.film->getColor(xx, yy) + scene.light_source->color * weight);
        hitmap.mlock->unlock();
        return ;
    }
    if(!c.collided_num) return ;
    // if collider found  
    if(times == 0 && scene.medium){
        double wmedium = scene.medium -> getWeight(ray, c.t);
        weight = weight * (1 - wmedium);
        if(wmedium > EPS){// hit the medium
            const int MEDIUMNUM = 50;
            vector<double> mpos = scene.medium -> getPos(ray, c.t, MEDIUMNUM);
            for(int k = 0;k < MEDIUMNUM;k++){
                HitPoint e;
                e.weight = Color(wmedium / MEDIUMNUM, wmedium / MEDIUMNUM, wmedium / MEDIUMNUM);
                e.pos = ray.pos + ray.dir * mpos[k];
                e.normal_vector = Vec3(0,0,0);
                e.in_vector = ray.dir;
                e.imgx = xx;
                e.imgy = yy;
                hitmap.mlock->lock();
                hitmap.points.push_back(e);
                hitmap.mlock->unlock();
            }
        }
    }
    Vec3 pos = ray.pos + ray.dir * c.t;
    bool from_out = (c.normal_vector / ray.dir < 0);
    Vec3 normal_vector = from_out? c.normal_vector: c.normal_vector * -1;//real normal_vector
    weight = weight * (absorbing * (-beer_const) * (ray.pos - pos).mod()).Exp();
    Ray reflect_ray(pos, ray.dir.reflect(normal_vector));
    if(c.m-> diffuse_percent > EPS){
        HitPoint e;
        e.weight = weight * c.m->diffuse_percent * obj->getTexture(pos, c.collided_num - 1);
        e.pos = pos;
        e.normal_vector = normal_vector;
        e.in_vector = ray.dir;
        e.imgx = xx;
        e.imgy = yy;
        hitmap.mlock->lock();
        hitmap.points.push_back(e);
        hitmap.mlock->unlock();
    }
    // refraction
    double reflect_percent = c.m->reflect_percent;
    if(c.m->refract_percent > EPS && times < 6){
        Ray refract_ray(pos,Vec3(0,0,0));
        double next_ri = from_out? 1.0/c.m->refract_index: c.m->refract_index;
        if(ray.dir.refract(normal_vector, next_ri, refract_ray.dir)){
            calc(refract_ray, times + 1, from_out?c.m->absorb_color: Color(0, 0, 0), xx, yy, weight * c.m->refract_percent);
            //FIXME
        }
        else reflect_percent += c.m->refract_percent;
    }
    // reflection
    if(reflect_percent > EPS && times < 10){
        calc(reflect_ray, times + 1, absorbing, xx, yy, weight * reflect_percent * obj->getTexture(pos, c.collided_num - 1));
    }
}

void PhotonMapper::solve(){   
    hitmap.mlock = new mutex(); 
    vector< vector<Color> > colormap(scene.camera.film->getN(), std::vector<Color>(scene.camera.film->getM(), Color(0, 0, 0)));
    vector< vector<Color> > directmap(scene.camera.film->getN(), std::vector<Color>(scene.camera.film->getM(), Color(0, 0, 0)));
    buildHitMap();
    // for(int i = 0;i < scene.camera.film->getN();i++)
    //     for(int j = 0;j < scene.camera.film->getM();j++)
    //         directmap[i][j] = scene.camera.film->getColor(i, j);
    cerr<< "finish buildHitMap"<<endl;
    const int rounds = 100000, photon_num = 1000000;
    double source_energy = 700;
    double r = 0.2;
    const int THNUM = 4;
    vector<long long> seed;
    for(int i = 0;i < THNUM;i++)
        seed.push_back(100 + i);
    std::vector<thread> thpool(THNUM);
    auto func = [photon_num, THNUM, &seed, &r](int x, PhotonMapper* ppm){
    for(int i = 0;i < photon_num;i++){
        seed[x]+= THNUM;
                    //thpool[j] = thread(&PhotonMapper::photonTrace, this, scene.light_source->getPhoton(seed), 0, Color(0,0,0), scene.light_source->color, r, 4, seed);
        ppm->photonTrace(ppm->scene.light_source->getPhoton(seed[x]), 0, Color(0,0,0), ppm->scene.light_source->color, r, 4, seed[x]);
        if(x == 0 && i % 10000 == 0)
        cout << "emit a photon: "<<i<<" / " <<photon_num<<endl;
    }
    };
    for(int rd = 0; rd < rounds; rd++){
        cout << "new round "<<rd <<endl;
        if(rd % 10 == 9) {
            hitmap.points.clear();
            delete hitmap.root;
            hitmap.root = NULL;
            buildHitMap();
        }
        for(int i = 0;i < THNUM;i++)
            thpool[i] = thread(func, i, this);
        for(int i = 0;i < THNUM;i++)
            thpool[i].join();
        for(int i = 0;i < scene.camera.film->getN();i++)
            for(int j = 0;j < scene.camera.film->getM();j++){
                colormap[i][j] = colormap[i][j] * rd / (rd + 1);
            }
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

void PhotonMapper::photonTrace(const Ray& ray, int times, Color absorbing, Color phi, double r, int d3, long long seed){
    Collider c;
    Object * obj = scene.firstObject(ray, c);
    if(!c.collided_num) return;
    Vec3 pos = ray.pos + ray.dir * c.t;
    bool from_out = (c.normal_vector / ray.dir < 0);

    if(!from_out && scene.medium){
        double tmedium = scene.medium -> collide(ray, c.t);
        if(tmedium > 0){// hit the medium
            hitmap.update(hitmap.root, ray.pos + ray.dir * tmedium, phi, ray.dir, Vec3(0,0,0), r);
            return ;
        }
    }

    Vec3 normal_vector = from_out? c.normal_vector: c.normal_vector * -1;//real normal_vector 
    phi = phi * (absorbing * (-beer_const) * (ray.pos - pos).mod()).Exp();// beer
    double p = hal(d3, seed);
    if(p < c.m->diffuse_percent){
        hitmap.update(hitmap.root, pos, phi, ray.dir, normal_vector, r);
        if(times < 10){
            phi = phi * obj->getTexture(pos, c.collided_num - 1);
            double t;
            if(phi.r > phi.g && phi.r > phi.b){t = 1.0 / phi.r; phi = phi * t;}
            else if(phi.g > phi.r && phi.g > phi.b){t = 1.0 / phi.g; phi = phi * t;}
            else {t = 1.0 / phi.b; phi = phi * t;}
            if(fabs(t) > INF) return;
            //-----------------
            double r1 = 2. * PI * hal(d3 + 1, seed), r2 = hal(d3 + 2, seed);
            double r2s = sqrt(r2);
            Vec3 w = normal_vector, u = ((fabs(w.x) > .1 ? Vec3(0,1,0) : Vec3(1,0,0)) * w).unitize();
            Vec3 v = w * u, d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1-r2)).unitize();
            photonTrace(Ray(pos, d), times + 1, absorbing, phi, r, d3 + 3, seed);
        }
    }else if (p < c.m->diffuse_percent + c.m->refract_percent){
        if(times < 15){
            Ray refract_ray(pos,Vec3(0,0,0));
            double next_ri = from_out? 1.0/c.m->refract_index: c.m->refract_index;
            if(ray.dir.refract(normal_vector, next_ri, refract_ray.dir))
                photonTrace(refract_ray, times + 1, from_out?c.m->absorb_color: Color(0, 0, 0), phi, r, d3 + 3, seed);
            else
            {    
                Ray reflect_ray(pos, ray.dir.reflect(normal_vector));
                phi = phi * obj->getTexture(pos, c.collided_num - 1);
                double t;
                if(phi.r > phi.g && phi.r > phi.b){t = 1.0 / phi.r; phi = phi * t;}
                else if(phi.g > phi.r && phi.g > phi.b){t = 1.0 / phi.g; phi = phi * t;}
                else {t = 1.0 / phi.b; phi = phi * t;}
                if(fabs(t) > INF) return;
                photonTrace(reflect_ray, times + 1, absorbing, phi, r, d3 + 3, seed);
            }
        }else if (p < c.m->diffuse_percent + c.m->refract_percent + c.m->reflect_percent && times < 15){

                Ray reflect_ray(pos, ray.dir.reflect(normal_vector));
                phi = phi * obj->getTexture(pos, c.collided_num - 1);
                double t;
                if(phi.r > phi.g && phi.r > phi.b){t = 1.0 / phi.r; phi = phi * t;}
                else if(phi.g > phi.r && phi.g > phi.b){t = 1.0 / phi.g; phi = phi * t;}
                else {t = 1.0 / phi.b; phi = phi * t;}
                if(fabs(t) > INF) return;
                photonTrace(reflect_ray, times + 1, absorbing, phi, r, d3 + 3, seed);
        }
    }
}