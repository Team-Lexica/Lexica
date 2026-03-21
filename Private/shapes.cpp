#include "shapes.h"
#include <cmath>
#include <algorithm>

// each shape is the child class of the class DefineClass (the declaration is there in shapes.h header file)
// we are overriding the distance function each time

//Sphere 
Sphere::Sphere(float r)
{
    radius = r;
}
/*For a sphere surface lies at the radial distance from origin */
float Sphere::distance(const Vector& p) const
{
    return p.length() - radius;  // p.length() give the distance from origin for the particular point
}

//Cylider
Cylinder::Cylinder(float r, float h)
{
    radius = r;
    height = h;
}
/*In cylinder we have three surfaces the radial one and two circular plates at the top and bottom */
float Cylinder::distance(const Vector& p) const
{
    float vertical = std::sqrt(p.x * p.x + p.y * p.y) - radius; // calculate the sdf value for the radial part just in the XY plane
    float horizontal = std::abs(p.z) - (height / 2); 
    /*calculate sdf value from either of the two circular plates if
    z positive then from the upper plate and if z negative then from the lower plate whichever surface is closest*/

    return std::max(vertical, horizontal);// now to lie inside the cylinder both ways the sdf value should be negative so we did intersection
}

//Cube or Cuboid

Cuboid::Cuboid(float l):l(l),b(l),h(l){}  // constructor if one dimension is given - cube

Cuboid::Cuboid(float l,float b,float h):l(l),b(b),h(h){} // constructor if 3 dimensions are given- cuboid

float Cuboid::distance(const Vector& p) const
{
 float lx= std::abs(p.x)-l/2;       
 float ly= std::abs(p.y)-b/2;       
 float lz= std::abs(p.z)-h/2;  
 /* for a cuboid we have total 6 surfaces but at a time the point will be closest to the three surfaces depending 
 on the octate which it lies in so we have used absolutes values of points */        

 return std::max({lx,ly,lz});
 // the three sdf value from 3 surface all should lie inside then the point will lie inside
}

//Torus
Torus::Torus(float R,float r):R(R),r(r){}
/* In torus we have two radius R - ring radius   r- tube radius */

float Torus::distance(const Vector& p) const
{
    // first we calculated how far is the point from the ring circumference in the XY plane 
    // now imagine looking from the ring circumference to that point ( not from origin now )
    float q=std::sqrt(p.x*p.x+p.y*p.y)-R;
    // now we check the z coordinate of that point 
    // and calculate the radial distane from the circumference with q and z coordinate of the point and then calculate sdf value with the pipe radius
    float dist=std::sqrt(q*q+p.z*p.z)-r;
    return dist ;
}
