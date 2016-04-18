#include "image.h"
#include "raytracer.h"
using namespace std;
int main()
{
    /*Image img(400,400);
    for(int i= 0 ;i<400;i++)
        for(int j = 0;j<400;j++)
            img.setColor(0,0,0);
    img.save("./test.jpg");*/
    Raytracer* render = new Raytracer("./scene/2.txt");
    render->solve();
    /*Ray a = Ray(Vec3(3,0,0),Vec3(0,0,1));
    Collider c =render->scene.light_source->collide(a);
    cerr<<c.collided_num<<endl;*/
}