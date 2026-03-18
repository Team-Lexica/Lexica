import matplotlib.pyplot as plt

x = []
y = []
z = []
s=input()
with open(s) as file:
    for line in file:
        if line.startswith("v "):
            parts = line.split()
            x.append(float(parts[1]))
            y.append(float(parts[2]))
            z.append(float(parts[3]))

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(x, y, z, s=2,color="green")

plt.show()