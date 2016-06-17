#include "scene.h"
using namespace std;
std::istream& operator >> (std::istream& fin, Scene& s){
    string tmp;
    while(fin >> tmp){
        if(tmp == "Camera") fin >> s.camera;
        else if(tmp == "PointSource"){
            PointSource* ps = new PointSource();
            fin >> (*ps);
            s.light_source = ps;
        }else if(tmp == "RectSource"){
            RectSource* ps = new RectSource();
            fin >> (*ps);
            s.light_source = ps;            
        }
        else{
            if(tmp == "Sphere"){
                Sphere * new_obj = new Sphere();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);
            }else if( tmp == "Plane"){
                Plane * new_obj = new Plane();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);
            }else if( tmp == "TriangleSet"){
                TriangleSet * new_obj = new TriangleSet();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);
            }else if(tmp == "KdtreeSet"){
                TriangleSet * new_obj = new KdtreeSet();
                fin >> (*new_obj);
                s.objs.push_back(new_obj);                
            }else if(tmp == "Obj"){
                string inputfile;
                fin >> inputfile;
                double b;
                Vec3 dpos, angle;
                fin >> b >> dpos >> angle;
                s.readFromObj(inputfile, b, dpos, angle);
            }
            else if(tmp == "Medium"){
                s.medium = new Medium();
                fin >> (* s.medium);
            }
            else{
                cerr << "read error in scene, tmp is "<<tmp<<endl;
                assert(0);
            }

        }
    }
    return fin;    
}

Object* Scene::firstObject(Ray ray, Collider& c){
    c.collided_num = 0;
    Object* ret = NULL;
    for(int i = 0;i < objs.size(); i++)
    {
        Collider tmp = objs[i]->collide(ray);
        if(tmp.collided_num && (!c.collided_num || tmp.t < c.t - EPS)) c = tmp, ret = objs[i];
    }
    return ret;
}
bool Scene::isBlocked(Ray ray){
    double maxt = light_source->collide(ray).t;
    for(int i = 0;i < objs.size(); i++)
    {
        Collider tmp = objs[i]->collide(ray);
        if(tmp.collided_num && tmp.t < maxt + EPS) return true;
    }
    return false;
}

void Scene::readFromObj(std::string inputfile, double b, Vec3 dpos, Vec3 angle){

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
      
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());
      
    if (!err.empty()) { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }

    if (!ret) {
      exit(1);
    }

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    
    std::vector<Material*> ms;
    for (size_t i = 0; i < materials.size(); i++) {
      Material * tmp = new Material();
      tmp->reflect_percent = 1 - materials[i].specular[0];
      tmp->refract_percent = materials[i].shininess;
      tmp->diffuse_percent = 1 - tmp->refract_percent - tmp->reflect_percent;
      tmp->color = Color(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
      tmp->absorb_color = Color(1,1,1) - tmp->color;
      tmp->refract_index = materials[i].ior;
      if(materials[i].diffuse_texname != ""){
        tmp->img.read(materials[i].diffuse_texname);
        tmp->img.h = tmp->img.w = 1;
      }
      ms.push_back(tmp);

    }
  
    for (size_t i = 0; i < shapes.size(); i++) {
      KdtreeSet* tmp = new KdtreeSet();
      printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
      printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
      printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
      assert((shapes[i].mesh.indices.size() % 3) == 0);
      for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
        Triangle tri;
        tri.v0 = shapes[i].mesh.indices[3*f+0];
        tri.v1 = shapes[i].mesh.indices[3*f+1];
        tri.v2 = shapes[i].mesh.indices[3*f+2];
        tri.m = ms[shapes[i].mesh.material_ids[f]];
        tmp->triangles.push_back(tri);
      }

      printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
      cerr<<shapes[i].mesh.positions.size() / 3<<" vs "<<shapes[i].mesh.texcoords.size() / 2<<endl;
      assert((shapes[i].mesh.positions.size() % 3) == 0);
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
        Vec3 tt = Vec3(shapes[i].mesh.positions[3*v+0], shapes[i].mesh.positions[3*v+1], shapes[i].mesh.positions[3*v+2]);
        tt = tt.rotate(Vec3(1,0,0), angle.x/ 180.0 * PI);
        tt = tt.rotate(Vec3(0,1,0), angle.y/ 180.0 * PI);
        tt = tt.rotate(Vec3(0,0,1), angle.z/ 180.0 * PI);

        tmp->vertices.push_back(tt * b + dpos);
        //if(tmp->vertices.size()==1) {cerr<<"pos: "<<tmp->vertices[0].x <<" "<<tmp->vertices[0].y<<" "<<tmp->vertices[0].z<<endl;sleep(5);}
        if(shapes[i].mesh.texcoords.size() > 2 * v ){
            tmp->imgy.push_back(shapes[i].mesh.texcoords[2 * v]);
            tmp->imgx.push_back(shapes[i].mesh.texcoords[2 * v + 1]);
        }
      }
      tmp->build();
      objs.push_back(tmp);
    }

 
}
