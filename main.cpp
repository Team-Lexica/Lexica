#include "grid.h"

int main(){

Grid g(40,0.5);
Sphere s(7);
g.sample(s);
g.exportf("sphere.obj");

Cylinder c(7 ,7);
g.sample(c);
g.exportf("cylinder.obj");

Cuboid A(7);
g.sample(A);
g.exportf("cube.obj");

}