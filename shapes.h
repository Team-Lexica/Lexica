#pragma once
#include "vector.h"

class DefineClass{
    public:
    virtual float distance(const Vector& p) const = 0;
};




class Sphere: public DefineClass{

float radius;

public:

Sphere(float r){
    radius =r;
}
float distance(const Vector & p) const override{
    return p.length()-radius ;
}
};

class Cylinder: public DefineClass{
float radius,height;
public:
Cylinder(float r,float h){
    radius=r;
    height=h;
}
float distance(const Vector & p) const override{
    float vertical=sqrt(p.x*p.x + p.y*p.y)-radius;
    float horizontal=p.z-(height/2);
    return max(vertical,horizontal);
}
};