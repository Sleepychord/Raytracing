#include "vec3.h"
using namespace std;
Vec3 Vec3::operator +(const Vec3& b){
    return Vec3(x + b.x, y + b.y, z + b.z);
}
Vec3 Vec3::operator -(const Vec3& b){
    return Vec3(x - b.x, y - b.y, z - b.z);    
}
Vec3 Vec3::operator *(const Vec3& b){
    return Vec3(y * b.z - b.y * z, -(x * b.z - b.x * z), x * b.y - b.x * y);
}//cross
Vec3 Vec3::operator /(const Vec3& b){
    return Vec3(x * b.x, y * b.y, z * b.z);
}
//dot 
Vec3 Vec3::operator *(double k){
    return Vec3(x * k, y * k, z * k);
}
Vec3 Vec3::operator /(double k){
    return Vec3(x / k, y / k, z / k);
}
double Vec3::mod(){
    return sqrt(x * x + y * y + z * z);
}
Vec3 Vec3::unitize(){
    return (*this) / mod();
}
Vec3 Vec3::reflect(const Vec3& n){
    return (*this) - ((*this) / n * 2) * n;
}
Vec3 Vec3::rotate(Vec3 axis, double alpha){
    Vec3 ret;
    double cost = cos( alpha );
    double sint = sin( alpha );
    ret.x += x * ( axis.x * axis.x + ( 1 - axis.x * axis.x ) * cost );
    ret.x += y * ( axis.x * axis.y * ( 1 - cost ) - axis.z * sint );
    ret.x += z * ( axis.x * axis.z * ( 1 - cost ) + axis.y * sint );
    ret.y += x * ( axis.y * axis.x * ( 1 - cost ) + axis.z * sint );
    ret.y += y * ( axis.y * axis.y + ( 1 - axis.y * axis.y ) * cost );
    ret.y += z * ( axis.y * axis.z * ( 1 - cost ) - axis.x * sint );
    ret.z += x * ( axis.z * axis.x * ( 1 - cost ) - axis.y * sint );
    ret.z += y * ( axis.z * axis.y * ( 1 - cost ) + axis.x * sint );
    ret.z += z * ( axis.z * axis.z + ( 1 - axis.z * axis.z ) * cost );
    return ret;
}
istream& operator >>(istream& fin, Vec3& a){
    return (fin >> a.x >>a.y >>a.z);
}
