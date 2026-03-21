#include "vector.h"
#include <cmath>

// A struct that holds a 3d point in space

       // we have two consturctors

// If empty initialise as origin
Vector::Vector():x(0),y(0),z(0){}

// For a particular point in space
Vector::Vector(float x,float y ,float z):x(x),y(y),z(z){}

float Vector::length()const
{
  return std::sqrt(x*x+y*y+z*z);  // defines the distance from origin
}

