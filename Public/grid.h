#pragma once

#include "shapes.h"
#include "vector.h"
#include <vector>
#include <string>

class Grid
{
public:
    int size;
    float voxel_size;
    std::vector<float> model;

    Grid(int s, float vs);

    void sample(DefineClass& shape);
    void exportf(const std::string& filename);
};