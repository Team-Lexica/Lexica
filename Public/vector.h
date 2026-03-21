#pragma once

struct Vector{
float x,y,z;
Vector();
Vector(float x,float y, float z);
float length() const;
// length() gives the distance from origin
};