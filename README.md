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

## 🌌 From a Point to a World

Every complex structure in Lexica starts from something extremely simple:

👉 a single point in 3D space

## 🌐 The Grid — “Turning space into structure”

The grid is how we turn space into something computable.

We define a 3D region using:
- Size → how far the space extends
- Voxel size → distance between points

For example :

When i write  Grid(40 , 0.3)   40->Size  0.3-> Voxel size 
Then a 3d Grid is created in space like a canvas

<p align="center">
  <img src="./assets/grid.png" width="500"/>
</p>

<p align="center">
  <i>A 3d canvas in space</i>
</p>

size → controls the boundary (how far we go)

voxelSize → controls resolution (how fine we scan)

Now the distance function has been defines and the canvas is ready , what left is creating a distance feild in this canvas and get the surface we need

### What happens during sampling

At each generated point:

→ call distance(point)
→ result depends on current shape / operation

The grid is constant.

Only this changes:
distance(point)

And that alone creates completely different structures.

Everytime a shapes is called or an operation is applied the distance function in that child class is overridden

See: [`grid.cpp`](./Private/grid.cpp)
