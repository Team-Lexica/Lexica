#pragma once
#include "shapes.h"

class Union: public DefineClass{
DefineClass* a;
DefineClass* b;

public:
Union(DefineClass* a,DefineClass* b);
float distance(const Vector& p) const override;
};

class Intersect: public DefineClass{
DefineClass* a;
DefineClass* b;

public:
Intersect(DefineClass* a,DefineClass* b);
float distance(const Vector& p) const override;
};

class Subtract: public DefineClass{
DefineClass* a;
DefineClass* b;

public:
Subtract(DefineClass* a,DefineClass* b);
float distance(const Vector& p) const override;
};