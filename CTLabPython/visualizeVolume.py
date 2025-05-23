import napari
import SimpleITK as sitk
import numpy as np

volume = sitk.ReadImage("reconstructed_volume.nii.gz")
volume_np = sitk.GetArrayFromImage(volume)  # shape: [slices, height, width]

viewer = napari.Viewer()
viewer.add_image(volume_np, name='Volume')
napari.run()