import open3d as o3d
import numpy as np
import pandas as pd

#SOURCE

source = o3d.geometry.TriangleMesh.create_sphere(radius=1)
source.compute_vertex_normals()
source.paint_uniform_color([1.0, 0.0, 0.0])

#DETECTOR
width, height = 100, 100
center = np.array([700, 0, 0])

# Rectangle in YZ plane (normal along X)
half_w, half_h = width / 2, height / 2
corners = np.array([
    [0, -half_w, -half_h],
    [0,  half_w, -half_h],
    [0,  half_w,  half_h],
    [0, -half_w,  half_h]
]) + center

# Triangles
triangles = np.array([
    [2, 1, 0],
    [3, 2, 0]
])

# Create mesh
detector = o3d.geometry.TriangleMesh()
detector.vertices = o3d.utility.Vector3dVector(corners)
detector.triangles = o3d.utility.Vector3iVector(triangles)
detector.compute_vertex_normals()
detector.paint_uniform_color([0.0, 1.0, 0.0])  # green




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
#o3d.visualization.draw_geometries([source, detector, pcd3])