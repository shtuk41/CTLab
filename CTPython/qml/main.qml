import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 6.10
import QuadLib 1.0
import QuadControl 1.0

ApplicationWindow {
    width: 1000
    height: 600
    visible: true
    title: "Volume View Control"

    property GLView3D glView3DInstance
    property GLViewAxial glViewAxialInstance
    property GLViewCoronal glViewCoronalInstance
    property GLViewSagittal glViewSagittalInstance

    RowLayout {
        anchors.fill: parent
        spacing: 4

        Grid {
            id: quadGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            rows: 2
            spacing: 4

            // View 1
            Item {
                width: quadGrid.width / 2 - 4
                height: quadGrid.height / 2 - 4

                Loader {
                    id: glViewAxialLoader
                    anchors.fill: parent
                    active: true
                    sourceComponent: glViewAxial
                    onLoaded: glViewAxialInstance = glViewAxialLoader.item
                }
            }

            // View 2
            Item {
                width: quadGrid.width / 2 - 4
                height: quadGrid.height / 2 - 4

                Loader {
                    id: glViewSagittalLoader
                    anchors.fill: parent
                    active: true
                    sourceComponent: glViewSagittal
                    onLoaded: glViewSagittalInstance = glViewSagittalLoader.item
                }
            }

            // View 3
             Item {
                width: quadGrid.width / 2 - 4
                height: quadGrid.height / 2 - 4

                Loader {
                    id: glViewCoronalLoader
                    anchors.fill: parent
                    active: true
                    sourceComponent: glViewCoronal
                    onLoaded: glViewCoronalInstance = glViewCoronalLoader.item
                }
            }



            // View 4 (3D)
            Item {
                width: quadGrid.width / 2 - 4
                height: quadGrid.height / 2 - 4

                Loader {
                    id: glView3dLoader
                    anchors.fill: parent
                    active: true
                    sourceComponent: glView3d
                    onLoaded: glView3DInstance = glView3dLoader.item
                }
            }
        }

        ColumnLayout {
            Layout.preferredWidth: 200
            spacing: 20

            Label { text: "Min"; font.bold: true }
            RowLayout {
                Slider {
                    id: minVoxelThresholdValueSlider
                    from: 0
                    to: 65536
                    stepSize: 1
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (glView3DInstance) glView3DInstance.minVoxelThreshold = value
                        if (glViewAxialInstance) glViewAxialInstance.minVoxelThreshold = value
                        if (glViewCoronalInstance) glViewCoronalInstance.minVoxelThreshold = value
                        if (glViewSagittalInstance) glViewSagittalInstance.minVoxelThreshold = value
                    }
                }
                Label {
                    text: Math.round(minVoxelThresholdValueSlider.value).toString()
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignVCenter
                }
            }

            Label { text: "Max"; font.bold: true }
            RowLayout {
                Slider {
                    id: maxVoxelThresholdValueSlider
                    from: 0
                    to: 65536
                    stepSize: 1
                    value: 65536
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (glView3DInstance) glView3DInstance.maxVoxelThreshold = value
                        if (glViewAxialInstance) glViewAxialInstance.maxVoxelThreshold = value
                        if (glViewCoronalInstance) glViewCoronalInstance.maxVoxelThreshold = value
                        if (glViewSagittalInstance) glViewSagittalInstance.maxVoxelThreshold = value
                    }
                }
                Label {
                    text: Math.round(maxVoxelThresholdValueSlider.value).toString()
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }
    }

    Component {
        id: glView3d
        GLView3D { anchors.fill: parent; context: VolumeContext }
    }

    Component {
        id: glViewAxial
        GLViewAxial { anchors.fill: parent; context: VolumeContext }
    }

    Component {
        id: glViewCoronal
        GLViewCoronal { anchors.fill: parent; context: VolumeContext }
    }

    Component {
        id: glViewSagittal
        GLViewSagittal { anchors.fill: parent; context: VolumeContext }
    }
}
