#include<iostream>
#include<vector>
#include"renderer/renderer.h"
using namespace YeahooQAQ;


int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    cv::Scalar background_color(255, 255, 255);
    cv::Scalar color(0, 0, 255);
    Renderer randerer(1000, 1000, background_color);
    randerer.LoadModel("/home/yeahoo/桌面/TinyRenderer/test_model/spot_triangulated_good.obj");

    randerer.StartClock();
    randerer.ShowFlatModel(color, 0.99f);

    randerer.GetTimeCost();
    randerer.SaveImage(filename);   
    return 0;
}