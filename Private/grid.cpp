#include "grid.h"
#include <fstream>
#include <iostream>

// A canvas is being defined 
Grid::Grid(int s, float vs)
{
    size = s;
    voxel_size = vs;
    model.resize(size * size * size);
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
                Vector p(
                    (x - size/2) * voxel_size,
                    (y - size/2) * voxel_size,
                    (z - size/2) * voxel_size
                );

                float d = shape.distance(p);
                model[x + size * (y + size * z)] = d;
            }
        }
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
        float d = model[x + size * (y + size * z)];

        if(d < 0)
        {
            float px = (x - size/2) * voxel_size;
            float py = (y - size/2) * voxel_size;
            float pz = (z - size/2) * voxel_size;

            file << "v " << px << " " << py << " " << pz << std::endl;
        }
    }

    file.close();
}
