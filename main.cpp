#include "image.h"
using namespace std;
int main()
{
    Color a(0,0,1);
    Image img(400, 400);
    for(int i=0;i<400;i++)
        for(int j = 0;j<400;j++)
        {
            img.setColor(i,j,a);
        }
    img.save("./ff.jpg");
}