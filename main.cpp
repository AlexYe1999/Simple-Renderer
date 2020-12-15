#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace YeahooQAQ;

int main(int argc, char* argv[]){
    Renderer renderer(1000, 1000, Vec3f(0.0f, 0.0f, 0.0f));
    vector<Light> lights{
        {Vec3f(5, 3, 4),Vec3f(0.5, 0.5, 0.5)}
    };
    renderer.LoadModel("../test_model/cow/cow.obj", "../test_model/cow/cow.png");
    renderer.LoadSets(lights);
    Vec3f eye_pos(0.0f, 0.0f, -15.0f);
    float theta_per_second = 5.0f;
    int count = 0;
    char key = '0';
    renderer.ShowProcessing(false);
    while((key = cv::waitKey(5)) != 'q'){
        if(key == 'w'){
            eye_pos.z += 0.2f;
        }
        else if(key == 's'){
            eye_pos.z -= 0.2f;
        }
        else if(key == 'a'){
            eye_pos.x -= 0.2f;
        }
        else if(key == 'd'){
            eye_pos.x += 0.2f;
        }

        renderer.StartClock();        
        renderer.ShowProcessing(false);

        renderer.ClearCanvas();
        renderer.SetModelMatrix(10.0f, count * theta_per_second, 0.0f);
        renderer.SetViewMatrix(eye_pos);
        renderer.SetProjectionMatrix(30.0f, 1.0f, 0.1, 50.0f);   

        renderer.MvpTransforme();

        renderer.RenderModel();

        renderer.GetTimeCost();
        renderer.ShowImage("Rendering", 8);
        renderer.ClearTimeCounter();
        count++;
    }

    renderer.SaveImage("../output.png");
    return 0;
}