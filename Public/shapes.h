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