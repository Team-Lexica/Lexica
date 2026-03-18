#include "operations.h"
#include <cmath>

Union::Union(DefineClass* a,DefineClass* b):a(a),b(b){}
float Union::distance(const Vector& p) const
{
 return std::min(a->distance(p),b->distance(p));
}

Intersect::Intersect(DefineClass* a,DefineClass* b):a(a),b(b){}
float Intersect::distance(const Vector& p) const
{
 return std::max(a->distance(p),b->distance(p));
}

Subtract::Subtract(DefineClass* a,DefineClass* b):a(a),b(b){}
float Subtract::distance(const Vector& p) const
{
 return std::max(a->distance(p),-(b->distance(p)));
}