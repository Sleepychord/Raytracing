#include "vec3.h"
#include "color.h"
#include "image.h"
#include "Plane.h"
#include <vector>
class Camera
{
public:
    Camera():film(NULL){}
    ~Camera(){delete film;}
    Image* film;
    Vec3 center, height, width, lens;
    double h, w;
    friend std::istream& operator >> (std::istream& fin, Camera&);
    Vec3 getPoint(double x, double y);
    std::vector<Vec3> getSamplePoints(double x, double y, int num);
}; 