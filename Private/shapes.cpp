#include "shapes.h"
#include <cmath>
#include <algorithm>

Sphere::Sphere(float r)
{
    radius = r;
}

float Sphere::distance(const Vector& p) const
{
    return p.length() - radius;
}

Cylinder::Cylinder(float r, float h)
{
    radius = r;
    height = h;
}

float Cylinder::distance(const Vector& p) const
{
    float vertical = std::sqrt(p.x * p.x + p.y * p.y) - radius;
    float horizontal = p.z - (height / 2);
    return std::max(vertical, horizontal);
}
