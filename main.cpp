#include "grid.h"

int main(){

Grid g(30,0.5);
Cylinder s(5,6);
g.sample(s);
g.exportf("cylinder.obj");



}