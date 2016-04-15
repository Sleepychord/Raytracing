#include "image.h"
using namespace std;
using namespace cv;
int Image::getN()const{
    return mat.rows;
}
int Image::getM()const{
    return mat.cols;
}
Color Image::getColor(int x, int y){
    return Color(mat.at<Vec3d>(x, y)[2] / 255, mat.at<Vec3d>(x, y)[1] / 255, mat.at<Vec3d>(x, y)[0] / 255);
}
Color Image::getColor(double x, double y){
    int x0 = int(x) % mat.rows;
    int y0 = int(y) % mat.cols;
    int x1 = (x0 + 1) % mat.rows;
    int y1 = (y0 + 1) % mat.cols;
    double r_x = x - int(x);
    double r_y = y - int(y);
    Color ret(0,0,0);
    ret += getColor(x0, y0) * r_x * r_y;
    ret += getColor(x1, y0) * (1 - r_x) * r_y;
    ret += getColor(x0, y1) * r_x * (1 - r_y);
    ret += getColor(x1, y1) * (1 - r_x) * (1 - r_y);
    return ret;
}
void Image::setColor(int x, int y, Color c){
    mat.at<Vec3d>(x, y) = Vec3d(c.b * 255, c.g * 255, c.r * 255);
}
void Image::save(std::string s){
    imwrite(s.c_str(), mat);
}
void Image::read(std::string s){
    mat = imread(s);
} 