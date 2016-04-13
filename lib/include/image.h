#pragma once
#include <opencv2/opencv.hpp>
#include <cstring>
#include "color.h"
#include "const.h"
class Image{
    cv::Mat mat;
public:
    int getN()const;
    int getM()const;
    Color getColor(int x, int y);
    void setColor(int x, int y, Color c);
    void save(std::string);
    void read(std::string); 
    Image(int n, int m):mat(n, m, CV_64FC3){}
};