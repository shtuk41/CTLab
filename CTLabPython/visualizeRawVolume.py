import vtk
import numpy as np
from vtk.util import numpy_support

# Load raw data
data = np.fromfile(r"..\Model\area360.raw", dtype=np.float32).reshape((120, 120, 120))

# Convert to VTK format (column-major / Fortran order)
vtk_data = numpy_support.numpy_to_vtk(
    num_array=data.ravel(order='F'), deep=True, array_type=vtk.VTK_FLOAT
)

# Create VTK image data
image_data = vtk.vtkImageData()
image_data.SetDimensions(120, 120, 120)
image_data.GetPointData().SetScalars(vtk_data)

# Opacity transfer function
THRESHOLD = 5000

opacity_tf = vtk.vtkPiecewiseFunction()
opacity_tf.AddPoint(np.min(data), 0.0)
opacity_tf.AddPoint(THRESHOLD, 1.0)
opacity_tf.AddPoint(THRESHOLD + 1, 1.0)
opacity_tf.AddPoint(np.max(data), 1.0)

# Color transfer function (grayscale)
color_tf = vtk.vtkColorTransferFunction()
color_tf.AddRGBPoint(np.min(data), 0.0, 0.0, 0.0)
color_tf.AddRGBPoint(np.max(data), 1.0, 1.0, 1.0)

# Volume properties
volume_property = vtk.vtkVolumeProperty()
volume_property.SetScalarOpacity(opacity_tf)
volume_property.SetColor(color_tf)
volume_property.SetInterpolationTypeToLinear()
volume_property.ShadeOff()

# Volume mapper
mapper = vtk.vtkSmartVolumeMapper()
mapper.SetInputData(image_data)

# Volume actor
volume = vtk.vtkVolume()
volume.SetMapper(mapper)
volume.SetProperty(volume_property)

# Renderer
renderer = vtk.vtkRenderer()
renderer.AddVolume(volume)
renderer.SetBackground(0.1, 0.1, 0.1)

# Window + Interactor
window = vtk.vtkRenderWindow()
window.AddRenderer(renderer)
interactor = vtk.vtkRenderWindowInteractor()
interactor.SetRenderWindow(window)

print("data minimum: ", np.min(data))
print("data maximum: ", np.max(data))

# Set interactor style to prevent auto rotation
style = vtk.vtkInteractorStyleTrackballCamera()
interactor.SetInteractorStyle(style)

# Reset camera for good initial view
renderer.ResetCamera()

# Start
interactor.Initialize()
interactor.Start()
