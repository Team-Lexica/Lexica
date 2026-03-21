# Lexica
Transforming the future of engineering and thus the future of innovation

## Overview

The system is built in three main parts:
1. 🟢 Shapes → define distance  
2. 🔗 Operations → modify distance  
3. 🌐 Grid → evaluates distance everywhere


Every shape is defined by a single idea:

👉 "Given a point, how far am I from my surface?"

This is implemented using Signed Distance Functions (SDFs).

👉 Think:

Every shape knows how to measure distance from itself

Each shape is its own class:
- Sphere
- Cube
- Cylinder
- Cuboid

Each one overrides the same function:

distance(point)

Implementation: [`shapes.cpp`](./Private/shapes.cpp)

