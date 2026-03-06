#include<iostream>
#include "shapes.h"
#include "vector.h"
#include "grid.h"
using namespace std;
int main(){
 Grid A(30,0.5);
 Sphere s(5);
 A.sample(s);
 Grid B(30,0.5);
 Cylinder c(5,5);
 B.sample(c);
 A.exportf("sphere.obj");
 B.exportf("cylinder.obj");
 




}