#include<iostream>
#include<vector>
#include"renderer/renderer.h"
using namespace YeahooQAQ;


int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    cv::Scalar background_color(255, 255, 255);
    cv::Scalar color(0, 0, 0);
    Renderer randerer(1000, 1000, background_color);
    randerer.StartClock();

    Vec2f vertex[3] = {
        {50.0f, 50.0f},
        {600.0f, 70.0f},
        {700.0f, 300.0f}
    };

    randerer.Draw2DRectangle(vertex, color);

    randerer.GetTimeCost();
    randerer.SaveImage(filename);        
    return 0;
}