#include "hit_map.h"
using namespace std;

void HitMap::build()
{
    root = new Node();
    std::vector<int> h;
    for(int i = 0;i < points.size();i++)
        h.push_back(i);
    root->dimension = 0;
    buildtree(root, h);
}

void HitMap::buildtree(Node* x, const std::vector<int>& p){
    if(p.size() < 9){
        x->data = p;
        x->dimension = -1;
        //cerr<< "building a leaf with "<<p.size()<<" hitpoints" <<endl;
        return ;
    }
    std::vector<pair<double, int> > tmp;
    tmp.clear();
    for(auto &num: p){
        if(x->dimension == 0) tmp.push_back(make_pair(points[num].pos.x, num));
        if(x->dimension == 1) tmp.push_back(make_pair(points[num].pos.y, num));
        if(x->dimension == 2) tmp.push_back(make_pair(points[num].pos.z, num));
    }
    sort(tmp.begin(), tmp.end());
    x->split = (*(tmp.begin() + tmp.size() / 2)).first;
    std::vector<int> half;
    for(int i = 0;i < tmp.size() / 2;i++)
        half.push_back(tmp[i].second);
    x->l = new Node();
    x->l->dimension = (x->dimension + 1) % 3;
    buildtree(x->l, half);
    
    half.clear();
    for(int i = tmp.size() / 2; i < tmp.size();i++)
        half.push_back(tmp[i].second);
    x->r = new Node();
    x->r->dimension = (x->dimension + 1) % 3;
    buildtree(x->r, half);
}

void HitMap::update(Node * x, Vec3 pos, Color phi, Vec3 dir, Vec3 nv, double r){
    if(x->dimension == -1){
        for(auto num: x->data)
            if((points[num].pos - pos).mod2() < r * r){
                //
                {
                    //mlock->lock();
                    points[num].n ++;
                    if(points[num].normal_vector.mod2() < EPS && nv.mod2() < EPS)
                        points[num].sum = points[num].sum + phi;
                    else if ((points[num].normal_vector * nv).mod2() < EPS){
                        double value = points[num].normal_vector / dir;
                        if(value < 0)
                            points[num].sum = points[num].sum + phi * -value;
                    }
                    //mlock->unlock();
                }
            }
        return;
    }
    if((x->dimension == 0 && pos.x - r <= x->split) || (x->dimension == 1 && pos.y - r <= x->split) || (x->dimension == 2 && pos.z - r <= x->split))
        update(x->l, pos, phi, dir, nv,r);    
    if((x->dimension == 0 && pos.x + r >= x->split) || (x->dimension == 1 && pos.y + r >= x->split) || (x->dimension == 2 && pos.z + r >= x->split))
        update(x->r, pos, phi, dir, nv,r);
}