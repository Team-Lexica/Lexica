#pragma once

#include "shapes.h"
#include "vector.h"
#include "operations.h"
#include <vector>
#include <string>

class Grid
{
public:
   
    int size;  
    float voxel_size;
    std::vector<float> model; // a 1 d array storing all the 3d coordinates
    std::vector<float> rho;

    Grid(int s, float vs); 

    void sample(DefineClass& shape); 
     
    void sampleDensity(float epsilon);
    
    void exportf(const std::string& filename);




};