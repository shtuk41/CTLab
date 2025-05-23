import SimpleITK as sitk
import os

# Folder containing your PNG slices
folder_path = r"C:\Files\CTLab\Slices\Slices3600"

# List and sort the PNG files to ensure proper order
file_names = [f for f in os.listdir(folder_path) if f.endswith('.png')]
file_names.sort(key=lambda x: int(''.join(filter(str.isdigit, x))))

# Prepend folder path to each filename
file_paths = [os.path.join(folder_path, f) for f in file_names]

# Read the image series
reader = sitk.ImageSeriesReader()
reader.SetFileNames(file_paths)
volume = reader.Execute()

# Save the reconstructed 3D volume (change format if you want)
output_path = "reconstructed_volume.nii.gz"
sitk.WriteImage(volume, output_path)

print(f"Volume saved to {output_path}")