#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace YeahooQAQ;

int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    Scalar background_color(255, 255, 255);
    Scalar line_color(0, 0, 255);
    Renderer randerer(100, 100, background_color);
    randerer.StartClock();

    randerer.DrawLine(Point2i(50, 50), Point2i(90, 90), line_color);
    randerer.DrawLine(Point2i(30, 20), Point2i(10, 20), line_color);
    randerer.GetTimeCost();

    randerer.SaveImage(filename);        

    return 0;
}