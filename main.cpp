#include "image.h"
#include "photon_mapper.h"
#include "raytracer.h"
using namespace std;
int main()
{
    //Image img(400,400);
    //img.read("./resource/floor.bmp");
    //img.save("./ff.jpg");
    PhotonMapper* render = new PhotonMapper("./scene/6.txt");
    render->solve();
    /*Ray a = Ray(Vec3(3,0,0),Vec3(0,0,1));
    Collider c =render->scene.light_source->collide(a);
    cerr<<c.collided_num<<endl;*/
}