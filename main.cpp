#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace YeahooQAQ;


int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    cv::Scalar background_color(1.0, 1.0, 1.0);
    Renderer renderer(1000, 1000, background_color);
    renderer.LoadModel("../test_model/cow/cow.obj", "../test_model/cow/cow.png");

    Vec3f eye_pos(0.0f, 0.0f, -15.0f);
    float theta_per_second = 5.0f;
    int count = 0;
    char key = '0';
    renderer.ShowProcessing(false);
    while(false && (key = cv::waitKey(5)) != 'q'){
        if(key == 'w'){
            eye_pos.z += 0.1f;
        }
        else if(key == 's'){
            eye_pos.z -= 0.1f;
        }
        else if(key == 'a'){
            eye_pos.x -= 0.1f;
        }
        else if(key == 'd'){
            eye_pos.x += 0.1;
        }
        renderer.ShowProcessing(false);
        renderer.ClearCanvas();
        renderer.SetModelMatrix(45.0f, count * theta_per_second, 0.0f);
        renderer.SetViewMatrix(eye_pos);
        renderer.SetProjectionMatrix(30.0f, 1.0f, 0.1, 50.0f);   

        renderer.StartClock();
        renderer.LoadTransformedVertex();

        renderer.RenderModel();

        renderer.GetTimeCost();
        
        renderer.ShowImage("Rendering", 100);
        renderer.ClearTimeCounter();
        count++;
    }

    renderer.SaveImage(filename);
    return 0;
}