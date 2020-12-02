#include<iostream>
#include<vector>
#include"renderer/renderer.h"
using namespace YeahooQAQ;


int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    cv::Scalar background_color(1.0, 1.0, 1.0);
    cv::Scalar flat_color(0.5, 0.5, 0.5);
    cv::Scalar wire_color(0.0, 0.0, 0.0);
    Renderer randerer(1000, 1000, background_color);
    randerer.LoadModel("../test_model/cow.obj");

    randerer.StartClock();

    randerer.RenderFlatModel(flat_color, 1.0f);    
    randerer.RenderWireModel(wire_color, 1.0f);

    randerer.GetTimeCost();
    randerer.SaveImage(filename);   
    return 0;
}