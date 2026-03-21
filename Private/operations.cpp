#include "operations.h"
#include <cmath>
// these are operations to be applied on primitive shapes and generate new shapes
// child class of DefineClass 



/**takes two shapes reference and link them to their distance function and a new distance 
 is being created and overrided**/
 

 // Union - we will take the min of two distances for the same point for two seperate shapes in space 
Union::Union(DefineClass* a,DefineClass* b):a(a),b(b){}

float Union::distance(const Vector& p) const
{
 return std::min(a->distance(p),b->distance(p));
}

// Intersect- max of two distances for the same point for two shapes in space
Intersect::Intersect(DefineClass* a,DefineClass* b):a(a),b(b){}
float Intersect::distance(const Vector& p) const
{
 return std::max(a->distance(p),b->distance(p));
}

// Subtract- shape1 intersection (not shape2)
Subtract::Subtract(DefineClass* a,DefineClass* b):a(a),b(b){}
float Subtract::distance(const Vector& p) const
{
 return std::max(a->distance(p),-(b->distance(p)));
}

// a and b can be primitve or operated shapes just the object of the DefineClass