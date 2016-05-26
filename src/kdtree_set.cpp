#include "kdtree_set.h"
using namespace std;
Collider KdtreeSet::search(Node* x, Ray& ray, double t1, double t2){
    if(x->dimension == -1){
        Collider ret;
        ret.collided_num = 0;
        ret.t = INF;
        for(auto i: x->data){
            double dis_per_t = ray.dir / normal_vector[i];
            if(fabs(dis_per_t) < EPS) continue;
            double t = ((vertices[triangles[i].v0] - ray.pos) / normal_vector[i]) / dis_per_t;
            if(t < EPS || t >= ret.t) continue;
            Vec3 v0 = ray.pos + ray.dir * t;
            double x0 = v0.x, y0 = v0.y;
            // Vec3 l1 = vertices[triangles[i].v0] - intersection, l2 = vertices[triangles[i].v1] - intersection, l3 = vertices[triangles[i].v2] - intersection;
            // Vec3 r1 = l1 * l2, r2 = l2 * l3, r3 = l3 * l1;
            // if(r1 / r2 < 0 || r2 / r3 < 0 || r3 / r1 < 0) continue;
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
            if(u < 0 || v < 0 || 1 - u - v < 0) continue;
            Vec3 nv;
            nv = (normal_v[triangles[i].v0] * u + normal_v[triangles[i].v1] * v + normal_v[triangles[i].v2] * (1 - u - v)).unitize();
            ret.collided_num = 1;
            ret.t = t;
            //if(dis_per_t < 0)
            ret.normal_vector = nv;
            //else ret.normal_vector = nv * -1;
        }
        return ret;
    }else{
        double t = ray.cut(x->dimension, x->split);
        if(t == INF){
            if(x->dimension == 0){
                if(ray.pos.x < x->split) return search(x->l, ray, t1, t2);
                else return search(x->r ,ray, t1, t2);
            }
            if(x->dimension == 1){
                if(ray.pos.y < x->split) return search(x->l, ray, t1, t2);
                else return search(x->r ,ray, t1, t2);
            }
            if(x->dimension == 2){
                if(ray.pos.z < x->split) return search(x->l, ray, t1, t2);
                else return search(x->r ,ray, t1, t2);
            }
        }
        Node * first = x->l , * last = x -> r, *tmp;
        if((x->dimension == 0 && ray.dir.x < 0) || (x->dimension == 1 && ray.dir.y < 0) || (x->dimension == 2 && ray.dir.z < 0))
            tmp = first, first = last, last = tmp;
        if(t <= t1) return search(last, ray, t1, t2);
        if(t >= t2) return search(first, ray, t1, t2);
        Collider ret = search(first, ray, t1, t);
        if(ret.collided_num == 0 || ret.t > t) return search(last, ray, t, t2);
            else return ret;
    }
}
Collider KdtreeSet::collide(Ray& ray){
    double mint = 0, maxt = INF, p;
    Collider ret;
    ret.collided_num = 0;
    //slab of x axis
    double t1 = ray.cut(0, box.min[0]);
    if(t1 == INF){
        if(box.min[0] >= ray.pos.x || ray.pos.x >= box.max[0]) return ret;
    }else{
        double t2 = ray.cut(0, box.max[0]);
        if(t1 > t2) p = t1, t1 = t2, t2 = p;
        if(t1 > mint) mint = t1;
        if(t2 < maxt) maxt = t2;
    }
    //slab of y axis
    t1 = ray.cut(1, box.min[1]);
    if(t1 == INF){
        if(box.min[1] >= ray.pos.y || ray.pos.y >= box.max[1]) return ret;
    }else{
        double t2 = ray.cut(1, box.max[1]);
        if(t1 > t2) p = t1, t1 = t2, t2 = p;
        if(t1 > mint) mint = t1;
        if(t2 < maxt) maxt = t2;
    }
    //slab of z axis
    t1 = ray.cut(2, box.min[2]);
    if(t1 == INF){
        if(box.min[2] >= ray.pos.z || ray.pos.z >= box.max[2]) return ret;
    }else{
        double t2 = ray.cut(2, box.max[2]);
        if(t1 > t2) p = t1, t1 = t2, t2 = p;
        if(t1 > mint) mint = t1;
        if(t2 < maxt) maxt = t2;
    }
    if(mint >= maxt) return ret;
    return search(root, ray, mint, maxt);
}
void KdtreeSet::build(){
    cerr<< "building Kdtree"<<endl;
    root = new Node();
    std::vector<int> tris;
    AABB& b = box;
    for(int i = 0;i<3;i++) b.max[i] = -INF, b.min[i] = INF;
    for(int i = 0;i < triangles.size();i++){
        tris.push_back(i);
    }
    for(auto &v : vertices){
        if(v.x < b.min[0]) b.min[0] = v.x;
        if(v.x > b.max[0]) b.max[0] = v.x;
        if(v.y < b.min[1]) b.min[1] = v.y;
        if(v.y > b.max[1]) b.max[1] = v.y;
        if(v.z < b.min[2]) b.min[2] = v.z;
        if(v.z > b.max[2]) b.max[2] = v.z;
    }
    used.resize(triangles.size());
    buildtree(root, tris, b);
    cerr<< "finish building"<<endl;
    std::vector<double> all(vertices.size(),0);
    for(int i = 0;i < vertices.size();i++)
        normal_v.push_back(Vec3(0,0,0));
    for(int i = 0;i < triangles.size();i++){
        Vec3 face_normal = ((vertices[triangles[i].v1] - vertices[triangles[i].v0]) * (vertices[triangles[i].v2] - vertices[triangles[i].v1])).unitize();
        normal_vector.push_back(face_normal);
        int x = triangles[i].v0, y = triangles[i].v1 , z = triangles[i].v2;
        //v0
        double angle = acos((vertices[z] - vertices[x]).unitize() / (vertices[y] - vertices[x]).unitize());
        all[x] += angle;
        normal_v[x] = normal_v[x] + face_normal * angle;
        //v1
        angle = acos((vertices[z] - vertices[y]).unitize() / (vertices[x] - vertices[y]).unitize());
        all[y] += angle;
        normal_v[y] = normal_v[y] + face_normal * angle;
        //v2
        angle = acos((vertices[x] - vertices[z]).unitize() / (vertices[y] - vertices[z]).unitize());
        all[z] += angle;
        normal_v[z] = normal_v[z] + face_normal * angle;
    }
    for (int i = 0; i < vertices.size(); ++i)
    {
        normal_v[i] = normal_v[i] / all[i];
        //cout <<all[i] <<"normal_v "<< normal_v[i].x <<" "<<normal_v[i].y<<" " <<normal_v[i].z<<endl;
    }
}
void KdtreeSet::buildtree(Node * x, const std::vector<int>& tris, AABB b){
    const double LIMIT = 2;
    if(((b.max[0] - b.min[0]) * (b.max[1] - b.min[1]) + (b.max[0] - b.min[0]) * (b.max[2] - b.min[2]) + (b.max[1] - b.min[1]) * (b.max[2] - b.min[2])) * tris.size() < LIMIT){
        x->data = tris;
        x->dimension = -1;
        cerr<< "a leaf node with "<<tris.size()<<" triangles"<<endl;
        return;
    }
    double best = INF;
    for(int d = 0; d < 3;d++){
        points.clear();
        for(int i = 0;i < tris.size();i++)
        {
            if(d == 0){
                points.push_back(make_pair(vertices[triangles[tris[i]].v0].x, i));
                points.push_back(make_pair(vertices[triangles[tris[i]].v1].x, i));
                points.push_back(make_pair(vertices[triangles[tris[i]].v2].x, i));
            }
            if(d == 1){
                points.push_back(make_pair(vertices[triangles[tris[i]].v0].y, i));
                points.push_back(make_pair(vertices[triangles[tris[i]].v1].y, i));
                points.push_back(make_pair(vertices[triangles[tris[i]].v2].y, i));
            }   
            if(d == 2){
                points.push_back(make_pair(vertices[triangles[tris[i]].v0].z, i));
                points.push_back(make_pair(vertices[triangles[tris[i]].v1].z, i));
                points.push_back(make_pair(vertices[triangles[tris[i]].v2].z, i));
            }          
            used[i] = 0;              
        }
        sort(points.begin(), points.end());
        int left = 0, right = tris.size();
        for(int i = 0;i < points.size();i++){
            double s1, s2;
            if(d == 0) 
                s1 = ((points[i].first - b.min[0]) * (b.max[1] - b.min[1]) + (points[i].first - b.min[0])*(b.max[2] - b.min[2]) 
                     + (b.max[1] - b.min[1])*(b.max[2] - b.min[2])),
                s2 = ((b.max[0] - points[i].first) * (b.max[1] - b.min[1]) + (b.max[0] - points[i].first)*(b.max[2] - b.min[2]) 
                     + (b.max[1] - b.min[1])*(b.max[2] - b.min[2]));
            else if (d == 1)
                s1 = ((points[i].first - b.min[1]) * (b.max[0] - b.min[0]) + (points[i].first - b.min[1])*(b.max[2] - b.min[2]) 
                     + (b.max[0] - b.min[0])*(b.max[2] - b.min[2])),
                s2 = ((b.max[1] - points[i].first) * (b.max[0] - b.min[0]) + (b.max[1] - points[i].first)*(b.max[2] - b.min[2]) 
                     + (b.max[0] - b.min[0])*(b.max[2] - b.min[2]));
            else 
                s1 = ((points[i].first - b.min[2]) * (b.max[1] - b.min[1]) + (points[i].first - b.min[2])*(b.max[0] - b.min[0]) 
                     + (b.max[1] - b.min[1])*(b.max[0] - b.min[0])),
                s2 = ((b.max[2] - points[i].first) * (b.max[1] - b.min[1]) + (b.max[2] - points[i].first)*(b.max[0] - b.min[0]) 
                     + (b.max[1] - b.min[1])*(b.max[0] - b.min[0]));
            double value = left * s1 + right * s2;
            if(value < best) best = value, x->split = points[i].first, x->dimension = d;
            if(used[points[i].second] == 0) left ++;
            used[points[i].second]++;
            if(used[points[i].second] == 3) right --;
        }
    }
    std::vector<int> half;
    //build left child
    for(int t : tris)
    {   
        if(x->dimension == 0)     
            if(vertices[triangles[t].v0].x <= x->split || vertices[triangles[t].v1].x <= x->split || vertices[triangles[t].v2].x <= x->split)
                half.push_back(t);
        if(x->dimension == 1)     
            if(vertices[triangles[t].v0].y <= x->split || vertices[triangles[t].v1].y <= x->split || vertices[triangles[t].v2].y <= x->split)
                half.push_back(t);
        if(x->dimension == 2)     
            if(vertices[triangles[t].v0].z <= x->split || vertices[triangles[t].v1].z <= x->split || vertices[triangles[t].v2].z <= x->split)
                half.push_back(t);        
    }
    if(half.size() == tris.size()){
        x->data = tris;
        x->dimension = -1;
        cerr<< "a leaf node with "<<tris.size()<<" triangles"<<endl;
        return;
    }
    x->l = new Node();
    AABB next = b;
    next.max[x->dimension] = x->split;
    buildtree(x->l, half, next);
    //build right child 
    half.clear();
    for(int t : tris)
    {   
        if(x->dimension == 0)     
            if(vertices[triangles[t].v0].x >= x->split || vertices[triangles[t].v1].x >= x->split || vertices[triangles[t].v2].x >= x->split)
                half.push_back(t);
        if(x->dimension == 1)     
            if(vertices[triangles[t].v0].y >= x->split || vertices[triangles[t].v1].y >= x->split || vertices[triangles[t].v2].y >= x->split)
                half.push_back(t);
        if(x->dimension == 2)     
            if(vertices[triangles[t].v0].z >= x->split || vertices[triangles[t].v1].z >= x->split || vertices[triangles[t].v2].z >= x->split)
                half.push_back(t);        
    }
    if(half.size() == tris.size()){
        delete x->l;
        x->l = NULL;
        x->data = tris;
        x->dimension = -1;
        cerr<< "a leaf node with "<<tris.size()<<" triangles"<<endl;
        return;
    }
    x->r = new Node();
    next.max[x->dimension] = b.max[x->dimension];
    next.min[x->dimension] = x->split;
    buildtree(x->r ,half, next);
}