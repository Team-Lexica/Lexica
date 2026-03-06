#pragma once
#include <cmath>
#include <vector>
using namespace std;

struct Vector{
 float x,y,z;
 Vector(float x,float y,float z){
   this->x=x;
   this->y=y;
   this->z=z; 
 }
 float length() const{
    return sqrt(x*x + y*y + z*z);
 }
};

