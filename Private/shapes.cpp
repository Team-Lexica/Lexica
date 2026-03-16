#include "shapes.h"
#include <cmath>
#include <algorithm>

//Sphere 
Sphere::Sphere(float r)
{
    radius = r;
}

float Sphere::distance(const Vector& p) const
{
    return p.length() - radius;
}

//Cylider
Cylinder::Cylinder(float r, float h)
{
    radius = r;
    height = h;
}

float Cylinder::distance(const Vector& p) const
{
    float vertical = std::sqrt(p.x * p.x + p.y * p.y) - radius;
    float horizontal = std::abs(p.z) - (height / 2);
    return std::max(vertical, horizontal);
}

//Cube or Cuboid
Cuboid::Cuboid(float l):l(l),b(l),h(l){}

Cuboid::Cuboid(float l,float b,float h):l(l),b(b),h(h){}

float Cuboid::distance(const Vector& p) const
{
 float lx= std::abs(p.x)-l/2;
 float ly= std::abs(p.y)-b/2;
 float lz= std::abs(p.z)-h/2;
 return std::max({lx,ly,lz});
}

//Torus
Torus::Torus(float R,float r):R(R),r(r){}

float Torus::distance(const Vector& p) const
{
    float q=std::sqrt(p.x*p.x+p.y*p.y)-R;
    float dist=std::sqrt(q*q+p.z*p.z)-r;
    return dist ;
}
