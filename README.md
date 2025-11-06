# CTLab
Simulation of CT acquisition process and volume reconstruction.  

- Points inside the model:

![Screenshot](Readme/PointsInsideModel.png)

- Initial scanning configuration:

![Screenshot](Readme/initialConfiguration.png)

- First projection:

![Screenshot](Readme/firstProjection.png)  

- Phantom visualization:

![Screenshot](Readme/planes.png)  

- Reconstruction using SimpleITK and viewed with napari

![Screenshot](Readme/ReconstructedSimpleITK.png)

- Volume rendered in quad view

![Screenshot](Readme/cupVolume.png)

- Real volume captured with Zeiss Metrotom rendered in quad view (QWidgets)

![Screenshot](Readme/ceasarsMetrotom.png)

- Views implemented in Qt/OpenGL CPP and loaded into quad view in Python QML (PySide 6.0)

![Screenshot](Readme/pythonQML.png)



References:
I can't find where the wolf's 3d model came from.  

- Dependencies

    Python based reconstruction depends on SimpleITK

    pip install SimpleITK

    Python visualization depends on napari package

    pip install napari[all]

    CUDA 12.8

    Qt 6.9.1

    PySide 6







