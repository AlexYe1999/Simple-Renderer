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

    Vec3f eye_pos(0.0f, 0.0f, 17.0f);
    renderer.SetViewMatrix(eye_pos);
    renderer.SetProjectionMatrix(45.0f, 1.0f, 0.1, 50.0f);
    float theta_per_second = 5.0f;
    int count = 0; 
    while(true){
        char key = cv::waitKey(15);
        if(key == 'w'){
            eye_pos.z++;
        }
        else if(key == 's'){
            eye_pos.z--;
        }
        else if(key == 'a'){
            eye_pos.x--;
        }
        else if(key == 'd'){
            eye_pos.x++;
        }
        renderer.SetViewMatrix(eye_pos);
        renderer.SetModelMatrix(count * theta_per_second, 180.0f, 0.0f);
        
        renderer.StartClock();
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