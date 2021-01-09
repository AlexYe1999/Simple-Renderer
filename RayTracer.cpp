#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace LemonCube;

int main(){
    Renderer renderer(500, 500, Vec3f(0.0f, 0.0f, 0.0f));
    Vec3f eye_pos(0.0f, 0.0f, -6.0f);
    vector<Sphere> spheres{
        {Vec3f(0.0f, 0.0f, 1.0f), 1.0f}
    };
    renderer.LoadSpheres(spheres);

    float theta = 0.0f;
    float theta_per_second = 0.03f;
    char key = '0';

    renderer.ShowProcessing(false);
    while((key = cv::waitKey(2)) != ' '){
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

        renderer.ClearCanvas();
        renderer.SetModelMatrix(sin(theta+=theta_per_second)*20.0f, 0.0f, 0.0f);

        renderer.SetViewMatrix(eye_pos);
        renderer.SetProjectionMatrix(90.0f, 1.0f, 1.0f, 50.0f);
        renderer.MvpTransforme();

        renderer.RayTracing();

        renderer.GetTimeCost();
        renderer.ShowImage("RayTracing", 3);
        renderer.ClearTimeCounter();
    }
    
    renderer.SaveImage("../output0.jpg");
    return 0;
}