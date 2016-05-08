#pragma once
#include <opencv2/opencv.hpp>
#include <cstring>
#include <cmath>
#include "color.h"
#include "const.h"
#include <iostream>
class Image{
    cv::Mat mat;
public:
    double h,w;
    int getN()const;
    int getM()const;
    Color getColor(int x, int y);
    Color getColor(double x, double y);
    void setColor(int x, int y, Color c);
    void save(std::string);
    void read(std::string); 
    Image(int n, int m):mat(n, m, CV_8UC3){}
    Image(std::string filename){ read(filename); }
};