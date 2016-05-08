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
    return Color(mat.at<Vec3b>(x, y)[2] / 255.0, mat.at<Vec3b>(x, y)[1] / 255.0, mat.at<Vec3b>(x, y)[0] / 255.0);
}
Color Image::getColor(double x, double y){
    x = abs(x) / h * getN(), y = abs(y) / w * getM();
    int x0 = int(x) % mat.rows;
    int y0 = int(y) % mat.cols;
    int x1 = (x0 + 1) % mat.rows;
    int y1 = (y0 + 1) % mat.cols;
    double r_x = x - int(x);
    double r_y = y - int(y);
    Color ret(0,0,0);
    //cout<< x0 << " "<< x1<< " "<<y0 <<" "<<y1<<endl;
    ret += getColor(x0, y0) * r_x * r_y;
    ret += getColor(x1, y0) * (1 - r_x) * r_y;
    ret += getColor(x0, y1) * r_x * (1 - r_y);
    ret += getColor(x1, y1) * (1 - r_x) * (1 - r_y);
    //cout << ret.r <<" "<<ret.g <<" "<<ret.b <<endl;
    return ret;
}
void Image::setColor(int x, int y, Color c){
    //cout << c.r << " " << c.g<< " "<< c.b<<endl;
    mat.at<Vec3b>(x, y) = Vec3b((char)(c.b * 255),(char)(c.g * 255),(char)(c.r * 255));
    //cout << (int)mat.at<Vec3b>(x,y)[2] << " " << (int)mat.at<Vec3b>(x,y)[1]<< " "<< (int)mat.at<Vec3b>(x,y)[0]<<endl;

}
void Image::save(std::string s){
    imwrite(s.c_str(), mat);
}
void Image::read(std::string s){
    mat = imread(s);
} 