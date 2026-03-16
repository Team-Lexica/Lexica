#include "grid.h"

int main(){

Grid g(40,0.3);


Torus t(6,2);
g.sample(t);
g.exportf("torus.obj");

Cylinder C(3,4);
g.sample(C);
g.exportf("cylinder.obj");
}