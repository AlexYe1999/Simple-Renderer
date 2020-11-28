#include"include/renderer.h"
#include<iostream>
#include<vector>
using namespace YeahooQAQ;

int main(int argc, char* argv[]){
    string filename = "./Output.jpg";
    Renderer randerer(720, 480, Scalar(0, 0, 0));
    


    

    randerer.SaveImage(filename);        

    return 0;
}