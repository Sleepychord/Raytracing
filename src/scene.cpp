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
                s.readFromObj(inputfile);
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

void Scene::readFromObj(std::string inputfile){
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
      
    for (size_t i = 0; i < shapes.size(); i++) {
      printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
      printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
      printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
      assert((shapes[i].mesh.indices.size() % 3) == 0);
      for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
        printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
      }

      printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
      assert((shapes[i].mesh.positions.size() % 3) == 0);
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
        printf("  v[%ld] = (%f, %f, %f)\n", v,
          shapes[i].mesh.positions[3*v+0],
          shapes[i].mesh.positions[3*v+1],
          shapes[i].mesh.positions[3*v+2]);
      }
    }

    for (size_t i = 0; i < materials.size(); i++) {
      printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
      printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
      printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
      printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
      printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
      printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
      printf("  material.Ns = %f\n", materials[i].shininess);
      printf("  material.Ni = %f\n", materials[i].ior);
      printf("  material.dissolve = %f\n", materials[i].dissolve);
      printf("  material.illum = %d\n", materials[i].illum);
      printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
      printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
      printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
      printf("  material.map_Ns = %s\n", materials[i].specular_highlight_texname.c_str());
      std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
      std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
      for (; it != itEnd; it++) {
        printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
      }
      printf("\n");
    }

}
