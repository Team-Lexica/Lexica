#pragma once
#include "vector.h"

// Base class for all SDF shapes
// distance(p) returns signed distance from point p to surface
class DefineClass   
{
public:
    virtual float distance(const Vector& p) const = 0; // SDF function
};

// Sphere centered at origin
class Sphere : public DefineClass
{
    float radius; // radius of sphere

public:
    Sphere(float r); // constructor
    float distance(const Vector& p) const override; // |p| - radius
};

// Infinite cylinder along one axis (assumed axis-aligned)
// height may clamp it to finite cylinder
class Cylinder : public DefineClass
{
    float radius, height; // radius and height of cylinder

public:
    Cylinder(float r, float h); 
    float distance(const Vector& p) const override;
};

// Axis-aligned box (cuboid) centered at origin
class Cuboid : public DefineClass
{
    float l,b,h; // length, breadth, height

public:
    Cuboid(float l,float b,float h); // full dimensions
    Cuboid(float l); // cube (all sides equal)
    float distance(const Vector& p) const override; 
};

// Torus centered at origin
// R = major radius (center to tube center)
// r = minor radius (tube thickness)
class Torus : public DefineClass
{
    float r,R;

public:
    Torus(float R,float r);
    float distance(const Vector& p) const override;
};