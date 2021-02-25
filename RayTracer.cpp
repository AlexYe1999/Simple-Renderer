#include"renderer/renderer.h"
#include<iostream>
#include<vector>
using namespace LemonCube;

void SetProperties(vector<shared_ptr<Hitable>>& properties);

int main(){
    Renderer renderer(900, 600, Vec3f(0.0f, 0.0f, 0.0f));
    Vec3f eye_pos(0.8f, 1.0f, -5.0f);
    vector<shared_ptr<Hitable>> obj_ptr;

    SetProperties(obj_ptr);
    renderer.LoadObjectPtr(obj_ptr);
    renderer.SetModelMatrix(2.0f, -40.0f, 0.0f);
    renderer.SetProjectionMatrix(90.0f, 1.5f, 1.0f, 50.0f);
    renderer.MSAA(true, 100);

    renderer.StartClock();

    renderer.ClearCanvas();

    renderer.SetViewMatrix(eye_pos);
    renderer.VertexShader();
    renderer.RayTracing(100);

    renderer.GetTimeCost();
    renderer.ClearTimeCounter();

    renderer.SaveImage("../output0.jpg");
    return 0;
}

void SetProperties(vector<shared_ptr<Hitable>>& properties){
    shared_ptr<Material> material  = make_shared<Lambertian>(Vec3f(1.0f, 1.0f, 1.0f));
    properties.push_back(make_shared<Sphere>(Vec3f(0.0f,-1000.0f,0.0f), 1000.0f, material));

    for (int a = -20; a < 20; a++) {
        for (int b = -20; b < 20; b++) {
            float choose_mat = rand() % 101 * 0.01f;
            Vec3f center(static_cast<float>(a) + rand() % 51 * 0.018f, 0.2f, static_cast<float>(b) + rand() % 51 * 0.018f);
            Vec3f albedo = Vec3f(0.5f+rand() % 51 * 0.01f, 0.5f+rand() % 51 * 0.01f, 0.5f+rand() % 51 * 0.01f);

            if (abs(center.x) > 3.0f || abs(center.z) > 1.0f) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.6f) {
                    // diffuse
                    sphere_material = make_shared<Lambertian>(albedo.normalized() * 1.5f);
                    properties.push_back(make_shared<Sphere>(center, 0.2f, sphere_material));
                } 
                else if (choose_mat < 0.7) {
                    // metal
                    float fuzz = rand() % 51 * 0.01f;
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    properties.push_back(make_shared<Sphere>(center, 0.2f, sphere_material));
                } 
                else{
                    // glass
                    float fuzz = rand() % 51 * 0.01f;
                    sphere_material = make_shared<Dielectric>(albedo, 1.7f * choose_mat);
                    properties.push_back(make_shared<Sphere>(center, 0.2f, sphere_material));
                    if(choose_mat > 0.83){
                        properties.push_back(make_shared<Sphere>(center, choose_mat * -0.2f , sphere_material));  
                    }
                }

            }
        }
    }

    material = make_shared<Dielectric>(Vec3f(0.98f, 0.98f, 0.98f), 1.5);
    properties.push_back(make_shared<Sphere>(Vec3f(-3, 1, 0), 1.0f, material));

    properties.push_back(make_shared<Sphere>(Vec3f(3, 1, 0), 1.0f, material));
    properties.push_back(make_shared<Sphere>(Vec3f(3, 1, 0), -0.9f, material));

    material = make_shared<Lambertian>(Vec3f(0.7f, 0.5f, 0.4f));
    properties.push_back(make_shared<Sphere>(Vec3f(-1, 1, 0), 1.0f, material));

    material = make_shared<Metal>(Vec3f(0.7f, 0.6f, 0.5f), 0.0f);
    properties.push_back(make_shared<Sphere>(Vec3f(1, 1, 0), 1.0f, material));

}