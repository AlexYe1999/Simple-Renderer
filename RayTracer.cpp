#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace LemonCube;

int main(){
    Renderer renderer(700, 700, Vec3f(0.0f, 0.0f, 0.0f));
    Vec3f eye_pos(0.0f, 2.0f, -16.0f);
    shared_ptr<Material> red = make_shared<Lambertian>(Vec3f(1.0f, 0.3f, 0.2f));
    shared_ptr<Material> green = make_shared<Lambertian>(Vec3f(0.2f, 1.0f, 0.1f));
    shared_ptr<Material> blue = make_shared<Lambertian>(Vec3f(0.1f, 0.0f, 1.0f));
    shared_ptr<Material> white_metal = make_shared<Metal>(Vec3f(0.8f, 0.8f, 0.8f), 0.05f);
    shared_ptr<Material> white_metal_glass_hollow = make_shared<Dielectric>(Vec3f(0.95f, 0.95f, 0.95f), 1.3f);
    shared_ptr<Material> white_glass = make_shared<Dielectric>(Vec3f(0.95f, 0.95f, 0.95f), 1.5f); 
    shared_ptr<Material> yellow = make_shared<Lambertian>(Vec3f(0.8f, 0.8f, 0.0f));
    vector<shared_ptr<Hitable>> obj_ptr{
        make_shared<Sphere>(Vec3f(2.0f, 0.8f, -7.0f), 1.0f, white_metal_glass_hollow),
        make_shared<Sphere>(Vec3f(2.0f, 0.8f, -7.0f), -0.8f, white_metal_glass_hollow),
        make_shared<Sphere>(Vec3f(-1.0f, 0.5f, -9.0f), 0.8f, white_glass),
        make_shared<Sphere>(Vec3f(0.0f, 3.0f, 0.0f), 3.0f, white_metal),
        make_shared<Sphere>(Vec3f(-4.0f, 0.9f, -4.0f), 1.0f, red),
        make_shared<Sphere>(Vec3f(2.0f, 5.0f, -7.0f), 1.0f, green),
        make_shared<Sphere>(Vec3f(5.0f, 2.0f, -1.0f), 2.0f, blue),
        make_shared<Sphere>(Vec3f(0.0f, -200.0f, 0.0f), 200.0f, yellow)
    };
    renderer.LoadObjectPtr(obj_ptr);
    renderer.SetProjectionMatrix(90.0f, 1.0f, 1.0f, 50.0f);
    renderer.MSAA(true, 100);   
    float theta = 0.0f;
    float theta_per_second = 0.03f;
    char key = '0';
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
        renderer.SetModelMatrix(sin(theta+=theta_per_second)*10.0f, theta, 0.0f);

        renderer.SetViewMatrix(eye_pos);
        renderer.MvpTransforme();
        renderer.RayTracing(50);

        renderer.GetTimeCost();
        renderer.SaveImage("../output0.jpg");
        //renderer.ShowImage("RayTracing", 3);
        renderer.ClearTimeCounter();
    }
    renderer.SaveImage("../output0.jpg");
    return 0;
}