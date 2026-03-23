import numpy as np
import matplotlib.pyplot as plt

# 🔷 Input
filename = input("Enter file name: ")

# 🔷 MUST match your C++ Grid settings
size = 40          # ⚠️ same as your Grid size
voxel_size = 0.5   # ⚠️ same as in C++

# 🔷 Read point cloud
x, y, z = [], [], []

with open(filename) as file:
    for line in file:
        if line.startswith("v "):
            parts = line.split()
            x.append(float(parts[1]))
            y.append(float(parts[2]))
            z.append(float(parts[3]))

x = np.array(x)
y = np.array(y)
z = np.array(z)

# 🔥 Convert world coordinates → voxel indices (CORRECT inverse mapping)
ix = ((x / voxel_size) + size/2 - 0.5).astype(int)
iy = ((y / voxel_size) + size/2 - 0.5).astype(int)
iz = ((z / voxel_size) + size/2 - 0.5).astype(int)

# 🔷 Safety clamp (avoid out-of-bounds)
ix = np.clip(ix, 0, size-1)
iy = np.clip(iy, 0, size-1)
iz = np.clip(iz, 0, size-1)

# 🔷 Create voxel grid
grid = np.zeros((size, size, size), dtype=bool)

# Fill occupied voxels
grid[ix, iy, iz] = True

# 🔷 Plot voxels
fig = plt.figure()
ax = fig.add_subplot(projection='3d')

# Create coordinate grid in WORLD space
x_coords = (np.arange(size+1) - size/2) * voxel_size
y_coords = (np.arange(size+1) - size/2) * voxel_size
z_coords = (np.arange(size+1) - size/2) * voxel_size

X, Y, Z = np.meshgrid(x_coords, y_coords, z_coords, indexing='ij')

ax.voxels(X, Y, Z, grid, edgecolor='k')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()