#include "grid.h"

int main(){
Grid g(40,0.3); // creates the grid with size 40 and voxel size 0.3 in grid.cpp

Cylinder c(2.5,10); // overrides the distance function that is defined in the Cylinder class in shapes.cpp

Sphere s(4); // override the distance function that is defined in the sphere class in shapes.cpp

Subtract sub(&s ,&c); // again overrides the distance function the way defined in operations.cpp

g.sample(sub);// now it starts iterating over the complete grid and calculate sdf value based on the distance function formed above


g.exportf("subtracted.obj"); // exports the points that lies inside

}