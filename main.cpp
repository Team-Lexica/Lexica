#include "grid.h"

int main(){
Grid g(40,0.3);
Cylinder c(2.5,10);
Sphere s(4);
Subtract sub(&s ,&c);

g.sample(s);
g.exportf("sphere.obj");
g.sample(sub);
g.exportf("subtracted.obj");

}