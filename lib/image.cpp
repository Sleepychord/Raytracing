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
void Image::setColor(int x, int y, Color c){
    mat.at<Vec3d>(x, y) = Vec3d(c.b * 255, c.g * 255, c.r * 255);
}
void Image::save(std::string s){
    imwrite(s.c_str(), mat);
}
void Image::read(std::string s){
    mat = imread(s);
} 