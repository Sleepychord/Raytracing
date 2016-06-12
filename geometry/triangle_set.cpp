#include "triangle_set.h"
using namespace std;
Collider TriangleSet::collide(Ray& ray){
    Collider ret;
    ret.collided_num = 0;
    ret.t = INF;
    for(int i = 0;i < triangles.size();i++){
        double dis_per_t = ray.dir / normal_vector[i];
        if(fabs(dis_per_t) < EPS) continue;
        double t = ((vertices[triangles[i].v0] - ray.pos) / normal_vector[i]) / dis_per_t;
        if(t < EPS || t >= ret.t) continue;
        Vec3 intersection = ray.pos + ray.dir * t;
        Vec3 l1 = vertices[triangles[i].v0] - intersection, l2 = vertices[triangles[i].v1] - intersection, l3 = vertices[triangles[i].v2] - intersection;
        Vec3 r1 = l1 * l2, r2 = l2 * l3, r3 = l3 * l1;
        if(r1 / r2 < 0 || r2 / r3 < 0 || r3 / r1 < 0) continue;
        ret.collided_num = 1;
        ret.t = t;
        if(dis_per_t < 0)
            ret.normal_vector = normal_vector[i];
        else ret.normal_vector = normal_vector[i] * -1;
        ret.m = triangles[i].m;
    }
    return ret;
}

Color TriangleSet::getTexture(Vec3& v0, int i){
    if(triangles[i].m == NULL)
        return material->color;
    else
    if(triangles[i].m->img.getM()){
            double x0 = v0.x, y0 = v0.y;
            double x1 = vertices[triangles[i].v0].x, y1 = vertices[triangles[i].v0].y;
            double x2 = vertices[triangles[i].v1].x, y2 = vertices[triangles[i].v1].y;
            double x3 = vertices[triangles[i].v2].x, y3 = vertices[triangles[i].v2].y;
            if(fabs(normal_vector[i].x) > fabs(normal_vector[i].y)){
                if(fabs(normal_vector[i].x) > fabs(normal_vector[i].z)) 
                    x1 = vertices[triangles[i].v0].z, x2 = vertices[triangles[i].v1].z, x3 = vertices[triangles[i].v2].z, x0 = v0.z;
            }else if(fabs(normal_vector[i].y) > fabs(normal_vector[i].z))
                    y1 = vertices[triangles[i].v0].z, y2 = vertices[triangles[i].v1].z, y3 = vertices[triangles[i].v2].z, y0 = v0.z;
            double u = ((x0 - x3)*(y2 - y3) - (y0 - y3)*(x2 - x3)) / ((x1 - x3)*(y2 - y3) - (y1 - y3)*(x2 - x3));
            double v = (fabs(x2 - x3) > EPS) ? ((x0 - x3) - u * (x1 - x3)) / (x2 - x3) : ((y0 - y3) - u * (y1 - y3)) / (y2 - y3);
            assert(!(u < 0 || v < 0 || 1 - u - v < 0));
            double xx = (imgx[triangles[i].v0] * u + imgx[triangles[i].v1] * v + imgx[triangles[i].v2] * (1 - u - v)),
                   yy = (imgy[triangles[i].v0] * u + imgy[triangles[i].v1] * v + imgy[triangles[i].v2] * (1 - u - v));
        return triangles[i].m->img.getColor(xx, yy);
    }
    else return triangles[i].m->color;
}
std::istream& operator >>(std::istream& fin, TriangleSet& s)
{
    string tmp;
    while(fin >> tmp){
        if(tmp == "end") break;
        if(tmp == "material") {
            s.material = new Material();
            fin >> (*(s.material));
        }
        else if (tmp == "obj"){
            string filename, t;
            Vec3 pos, axis;
            double p, angel;
            fin >> filename >> pos >> p >> axis >>angel;
            angel = angel * PI / 180.0;
            ifstream resource(filename);
            cerr<< "reading obj"<<endl;
                while(1){
                    bool remain_file = true;
                    while(t != "v" && t != "f" && (remain_file = (bool)(resource >> t)) );
                    if(!remain_file) break;
                    Vec3 vertex;
                    Triangle tri;
                    if(t == "v")
                    {                    
                        resource >> vertex;
                        vertex = (vertex * p).rotate(axis, angel) + pos;
                        s.vertices.push_back(vertex);
                    }                    
                    else {
                        resource >> tri.v0 >> tri.v1 >>tri.v2;
                        tri.v0 --;
                        tri.v1 --;
                        tri.v2 --;
                        s.triangles.push_back(tri);
                    }
                    if(!(resource >> t))break;
                }
            cerr<< "finish reading obj" <<endl;
        }
        else {
            cerr << "read TriangleSet error, string is "<<tmp <<endl;
            assert(0);
        }
    }
    s.build();
    return fin;
}
void TriangleSet::build(){
    for(int i = 0;i < triangles.size();i++){
        normal_vector.push_back(((vertices[triangles[i].v1] - vertices[triangles[i].v0]) * (vertices[triangles[i].v2] - vertices[triangles[i].v0])).unitize());
    }
}