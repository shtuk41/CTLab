import vtk
import numpy as np
from vtk.util import numpy_support

# Volume dimensions
Z, Y, X = 1876, 824, 287

# Load data (skip 1024-byte header)
data = np.fromfile(
    r"D:\Files\Cesars\Scissors_Test 2025-7-2 15-11-21.uint16_scv",
    dtype=np.uint16,
    offset=1024
).reshape((Z, Y, X), order='F')

print(data[60, 775, 75])

# Rescaled display range
DISPLAY_MIN = 25709
DISPLAY_MAX = 40000

# Convert to VTK array
vtk_data = numpy_support.numpy_to_vtk(
    num_array=data.ravel(order='F'),
    deep=True,
    array_type=vtk.VTK_FLOAT
)

# Create vtkImageData
image_data = vtk.vtkImageData()
image_data.SetDimensions(X, Y, Z)
image_data.GetPointData().SetScalars(vtk_data)

# Opacity transfer function (smooth ramp)
opacity_tf = vtk.vtkPiecewiseFunction()
mid_val = (DISPLAY_MIN + DISPLAY_MAX) / 2
opacity_tf.AddPoint(DISPLAY_MIN, 0.0)
opacity_tf.AddPoint(mid_val, 0.05)
opacity_tf.AddPoint(DISPLAY_MAX, 0.2)

# Color transfer function (grayscale)
color_tf = vtk.vtkColorTransferFunction()
color_tf.AddRGBPoint(DISPLAY_MIN, 0.0, 0.0, 0.0)
color_tf.AddRGBPoint(DISPLAY_MAX, 1.0, 1.0, 1.0)

# Volume properties
volume_property = vtk.vtkVolumeProperty()
volume_property.SetScalarOpacity(opacity_tf)
volume_property.SetColor(color_tf)
volume_property.SetInterpolationTypeToLinear()
volume_property.ShadeOff()

# Volume mapper and actor
mapper = vtk.vtkSmartVolumeMapper()
mapper.SetInputData(image_data)

volume = vtk.vtkVolume()
volume.SetMapper(mapper)
volume.SetProperty(volume_property)

# Renderer and window
renderer = vtk.vtkRenderer()
renderer.AddVolume(volume)
renderer.SetBackground(0.1, 0.1, 0.1)

window = vtk.vtkRenderWindow()
window.AddRenderer(renderer)
window.SetSize(1200, 800)

interactor = vtk.vtkRenderWindowInteractor()
interactor.SetRenderWindow(window)

# Set interactor style
style = vtk.vtkInteractorStyleTrackballCamera()
interactor.SetInteractorStyle(style)

# Start
renderer.ResetCamera()
interactor.Initialize()
interactor.Start()
