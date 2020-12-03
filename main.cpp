#include<iostream>
#include<vector>
#include"renderer/renderer.h"
using namespace YeahooQAQ;


int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    cv::Scalar background_color(1.0, 1.0, 1.0);
    cv::Scalar flat_color(0.5, 0.5, 0.5);
    cv::Scalar wire_color(0.0, 0.0, 0.0);
    Renderer renderer(1000, 1000, background_color);

    renderer.LoadModel("../test_model/cow.obj");
    Vec3f eye_pos(0.0f, 0.0f, 3.0f);
     renderer.SetViewMatrix(eye_pos);
    renderer.SetProjectionMatrix(45.0f, 1.0f, 0.2f, 2.0f);
    float theta_per_second = 5.0f; 
    int count = 0; 
    while(true){

        renderer.StartClock();
        renderer.SetModelMatrix(count * theta_per_second, 0.0f, 0.0f);

        renderer.RenderModel();
        renderer.RenderWireModel();

        renderer.GetTimeCost();
        
        renderer.ShowImage("Rendering", 100);
        renderer.ClearTimeCounter();
        renderer.ClearCanvas();
        count++;
    }

    renderer.SaveImage(filename);
    return 0;
}