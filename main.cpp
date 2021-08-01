#include"renderer/renderer.hpp"
#include<iostream>
#include<vector>
using namespace LemonCube;

int main(int argc, char* argv[]){
    Renderer<float> renderer(500, 500, Vec3f(0.0f, 0.0f, 0.0f));
    vector<shared_ptr<LightSource>> lights{
        make_shared<ShadowLight>(
            Vec3f(5, 5, 5), Vec3f(1.0f, 1.0f, 1.0f), 
            Vec3f(0.0f, 0.0f, 0.0f), 600
        ),
     
        make_shared<LightSource>(Vec3f(0, -3, 0),Vec3f(1.0f, 1.0f, 1.0f)),
        make_shared<LightSource>(Vec3f(-2, 0, 0),Vec3f(1.0f, 1.0f, 1.0f)), 
        make_shared<LightSource>(Vec3f(0, 2, 0),Vec3f(1.0f, 1.0f, 1.0f)),     
        make_shared<LightSource>(Vec3f(0, 0, -2),Vec3f(1.0f, 1.0f, 1.0f)),  
        make_shared<LightSource>(Vec3f(-2, -2, 2),Vec3f(1.0f, 1.0f, 1.0f)), 
        make_shared<LightSource>(Vec3f(2, -2, -2),Vec3f(1.0f, 1.0f, 1.0f)),
        make_shared<LightSource>(Vec3f(-2, 2, -2),Vec3f(1.0f, 1.0f, 1.0f))
        
    };
    array<Vec3f, 4> vertices{
        Vec3f(1.5f,-0.75f,1.5f),Vec3f(1.5f,-0.75f,-1.5f),
        Vec3f(-1.5f,-0.75f,-1.5f),Vec3f(-1.5f,-0.75f,1.5f)
    };
    array<Vec3f, 4> normals{
        Vec3f(0.0f,1.0f,0.0f),Vec3f(0.0f,1.0f,0.0f),
        Vec3f(0.0f,1.0f,0.0f),Vec3f(0.0f,1.0f,0.0f)
    };
    renderer.LoadRectangle(vertices, normals);
    /*
    array<Vec3f,4> vertices1{
        Vec3f(0.7f, 0.6f, 0.8f),Vec3f(0.7f, 0.6f, 0.1f),
        Vec3f(-0.7f, 0.6f, 0.1f),Vec3f(-0.7f, 0.6f, 0.8f)
    };
    array<Vec3f,4> normals1{
        Vec3f(0.0f,1.0f,0.0f),Vec3f(0.0f,1.0f,0.0f),
        Vec3f(0.0f,1.0f,0.0f),Vec3f(0.0f,1.0f,0.0f)
    };
    renderer.LoadRectangle(vertices1, normals1);
    */
    renderer.SetShader(make_shared<ShadowShader<float>>());   
    renderer.LoadLightSource(lights);
    renderer.LoadModel("../test_model/cow/cow.obj", "../test_model/cow/cow.png");
    renderer.RenderModel(true);

    Vec3f eye_pos(0.0f, 0.0f, -4.0f);
    float theta_per_second = 1.0f;
    int count = 0;
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
        renderer.SetModelMatrix(10.0f, count * theta_per_second, 0.0f);
        renderer.SetViewMatrix(eye_pos);
        renderer.SetProjectionMatrix(90.0f, 1.0f, 1.0f, 50.0f);   

        renderer.VertexShader();

        renderer.Rendering();

        renderer.GetTimeCost();
        renderer.ShowImage("Rendering", 20);
        renderer.ClearTimeCounter();
        count++;
    }

    renderer.SaveImage("../output0.jpg");
    return 0;
}