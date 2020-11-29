#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace YeahooQAQ;

int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    Scalar background_color(255, 255, 255);
    Scalar line_color(0, 0, 0);
    Renderer randerer(500, 500, background_color);
    randerer.StartClock();

    randerer.LoadModel("../test_model/face.obj");
    randerer.Show2DModel(line_color);

    randerer.GetTimeCost();
    randerer.SaveImage(filename);        
    return 0;
}