import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Load the CSV file (change the filename as needed)
# Set header=None if your file has no header line
df = pd.read_csv(r"..\Model\spherePoints.csv", header=None)

# Extract columns as x, y, z
x, y, z = df[0], df[1], df[2]

# Plotting
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x, y, z, c='blue', marker='o')

# Optional: set axis labels
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()
