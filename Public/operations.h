#pragma once
#include "shapes.h"

// Boolean Union: combines two shapes
// SDF = min(d1, d2) → closest surface dominates
class Union: public DefineClass{ 
    DefineClass* a;        
    DefineClass* b;

public:
    Union(DefineClass* a,DefineClass* b);
    float distance(const Vector& p) const override;
};

// Boolean Intersection: common region of both shapes
// SDF = max(d1, d2) → inside only where both are inside
class Intersect: public DefineClass{
    DefineClass* a;
    DefineClass* b;

public:
    Intersect(DefineClass* a,DefineClass* b);
    float distance(const Vector& p) const override;
};

// Boolean Subtraction: A - B (cut B out of A)
// SDF = max(d1, -d2) → keep A, remove region inside B
class Subtract: public DefineClass{
    DefineClass* a;
    DefineClass* b;

public:
    Subtract(DefineClass* a,DefineClass* b);
    float distance(const Vector& p) const override;
};