#pragma once
#include "shapes.h"
#include "vector.h"
#include <fstream>
class Grid{
    public:
 int size;
 float voxel_size;
 vector<float>model;

 Grid(int s,float vs){
    size=s;
    voxel_size=vs;
    model.resize(size*size*size);
 }
  void sample(DefineClass & shape){
    for(int x=0;x<size;x++){
        for(int y=0;y<size;y++){
            for(int z=0;z<size;z++){
                Vector p(
                (x-size/2)*voxel_size,
                (y-size/2)*voxel_size,
                (z-size/2)*voxel_size
            );

            float d = shape.distance(p);
            model[x+size*(y+size*z)]=d;
            }
        }
    }
  }
  void exportf(string filename){

    ofstream file(filename);

    for(int x=0;x<size;x++)
    for(int y=0;y<size;y++)
    for(int z=0;z<size;z++){

        float d = model[x+size*(y+size*z)];

        if(d < 0){

            float px=(x-size/2)*voxel_size;
            float py=(y-size/2)*voxel_size;
            float pz=(z-size/2)*voxel_size;

            file<<"v "<<px<<" "<<py<<" "<<pz<<endl;
        }
    }

    file.close();
}
};