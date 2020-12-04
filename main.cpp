#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace YeahooQAQ;


int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    cv::Scalar background_color(1.0, 1.0, 1.0);
    cv::Scalar flat_color(0.5, 0.5, 0.5);
    cv::Scalar wire_color(0.0, 0.0, 0.0);
    Renderer renderer(1000, 1000, background_color);
    renderer.LoadModel("../test_model/cow.obj");

    Vec3f eye_pos(0.0f, 0.0f, 5.0f);
    float theta_per_second = 5.0f;
    int count = 0; 
    char key = '0';
    renderer.ShowProcessing(false);
    while(key = cv::waitKey(50) != 'q'){
        if(key == 'w'){
            eye_pos.z+=0.1f;
        }
        else if(key == 's'){
            eye_pos.z-=0.1f;
        }
        else if(key == 'a'){
            eye_pos.x-=0.1f;
        }
        else if(key == 'd'){
            eye_pos.x+=0.1f;
        }

        renderer.ClearCanvas();
        renderer.SetModelMatrix(0.0f, count * theta_per_second,0.0f);
        renderer.SetViewMatrix(eye_pos);
        renderer.SetProjectionMatrix(45.0f, 1.0f, 0.1, 10.0f);   

        renderer.StartClock();
        renderer.LoadTransformedVertex();

        renderer.RenderPointModel();
        renderer.RenderModel();
        renderer.RenderWireModel(wire_color);

        renderer.GetTimeCost();
        
        renderer.ShowImage("Rendering", 100);
        renderer.ClearTimeCounter();
        count++;
    }

    renderer.SaveImage(filename);
    return 0;
}