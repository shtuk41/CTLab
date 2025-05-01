import open3d as o3d
import numpy as np
import pandas as pd

# Load first CSV
df1 = pd.read_csv(r"..\Model\area.csv", header=None)
points1 = df1.values.astype(np.float64)
pcd1 = o3d.geometry.PointCloud()
pcd1.points = o3d.utility.Vector3dVector(points1)
color1 = np.array([[1.0, 0.0, 0.0]])  # Red
pcd1.colors = o3d.utility.Vector3dVector(np.tile(color1, (len(points1), 1)))

# Load second CSV
df2 = pd.read_csv(r"..\Model\object.csv", header=None)
points2 = df2.values.astype(np.float64)
pcd2 = o3d.geometry.PointCloud()
pcd2.points = o3d.utility.Vector3dVector(points2)
color2 = np.array([[0.0, 1.0, 0.0]])  # Green
pcd2.colors = o3d.utility.Vector3dVector(np.tile(color2, (len(points2), 1)))

# Load second CSV
df3 = pd.read_csv(r"..\Model\insidePoints.csv", header=None)
points3 = df3.values.astype(np.float64)
pcd3 = o3d.geometry.PointCloud()
pcd3.points = o3d.utility.Vector3dVector(points3)
color3 = np.array([[0.0, 0.0, 1.0]])  # Green
pcd3.colors = o3d.utility.Vector3dVector(np.tile(color3, (len(points3), 1)))

# Visualize both
o3d.visualization.draw_geometries([pcd3])
