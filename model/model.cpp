#include"model.h"

namespace YeahooQAQ{
using namespace std;

Model::Model(string& filename){
    ifstream in;
    in.open(filename, ifstream::in);
    if(in.fail()){
        cout<<"Initialazation of model is failed\n";
        return;
    }
    
    string line;
    char trash;

    while(!in.eof()){
        getline(in, line);
        istringstream iss(line.c_str());
        if(!line.compare(0, 2, "v ")){
            Vec3f v;
            iss >> trash;            
            for(int i = 0; i < 3; i++){ iss >> v[i]; }
            vertices_.push_back(v);
        }
        else if(!line.compare(0, 2, "f ")){
            vector<int> f;
            int itrash, index;
            iss >> trash;
            while(iss >> index >> trash >> itrash>>trash>>itrash){
                index--;
                f.push_back(index);
            }
            surfaces_.push_back(f);
        }

    }
    cout<<"Model: \n";
    cout<<"  v: "<< vertices_.size() <<"\n  surface: "<<surfaces_.size()<<"\n\n";
}

Model::~Model(){};


}