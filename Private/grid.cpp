#include "grid.h"
#include <fstream>
#include <iostream>

// A canvas is being defined 
Grid::Grid(int s, float vs)
{
    size = s;
    voxel_size = vs;
    model.resize(size * size * size);
    /*
    model is a 1d array(declared in grid.h) that will store the sdf value of all the 3d points in space
    
    why 1d array and not a 3d array ? ---- because memory access is much faster
    */
}

/* we iterate all over the grid that we created and calculate distance(point)(a overriden function
obtained after applying all the operations ) for each and every point in space and store in the model array*/

void Grid::sample(DefineClass& shape)
{
    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y < size; y++)
        {
            for(int z = 0; z < size; z++)
            {
                /*here it is calculating the point coordinates  from origin (size by 2 - to get distance from center of the axis)*/
                Vector p(
                    (x+0.5 - size/2) * voxel_size, // voxel size is the gap between two points on the axis
                    (y+0.5 - size/2) * voxel_size,
                    (z+0.5 - size/2) * voxel_size
                );

                float d = shape.distance(p);// distance(p) the overridden function will give the distance of the vector p from surface

                model[x + size * (y + size * z)] = d; // 3d array can be arranged in 1d array using this formula
                // storing the distance value for all the 3d points in model
            }
        }
    }
}

void Grid::sampleDensity(float epsilon)
{
    rho.resize(size * size * size);
    for(int i = 0; i < size * size * size; i++)
    {
        float val = 0.5f - model[i] / epsilon;
        // clamp to [0,1]
        if(val < 0.0f) val = 0.0f;
        if(val > 1.0f) val = 1.0f;
        rho[i] = val;
    }
}

// extracting the points with negative sdf value 

void Grid::exportf(const std::string& filename)
{
    std::ofstream file(filename);

    for(int x = 0; x < size; x++)
    for(int y = 0; y < size; y++)
    for(int z = 0; z < size; z++)
    {
        /* Now iterating in the model array to check wether the point lies inside or outside of the surface*/
        float d = model[x + size * (y + size * z)];

        if(d < 0)
        {
            float px = (x+0.5 - size/2) * voxel_size; 
            float py = (y+0.5 - size/2) * voxel_size;
            float pz = (z+0.5 - size/2) * voxel_size;

            file << "v " << px << " " << py << " " << pz << std::endl; // writing it in the output file 
        }
    }

    file.close();
}
