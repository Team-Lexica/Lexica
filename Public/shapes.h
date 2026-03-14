#pragma once
#include "vector.h"

class DefineClass
{
public:
    virtual float distance(const Vector& p) const = 0;
};

class Sphere : public DefineClass
{
    float radius;

public:
    Sphere(float r);
    float distance(const Vector& p) const override;
};

class Cylinder : public DefineClass
{
    float radius, height;

public:
    Cylinder(float r, float h);
    float distance(const Vector& p) const override;
};

class Cuboid : public DefineClass
{
    float l,b,h;
public:
    Cuboid(float l,float b,float h);
    Cuboid(float l);
    float distance(const Vector& p) const override; 
};

class Torus : public DefineClass
{
    float r,R;
public:
    Torus(float R,float r);
    float distance(const Vector& p) const override;
};