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

    DropArea {
        anchors.fill: parent
        onDropped: function(dropEvent) {      // declare a parameter
            if (dropEvent.hasUrls) {
                // console.log("Dropped file:", dropEvent.urls[0])
                VolumeContext.loadVolume(dropEvent.urls[0])
            }
        }
    }

    property GLView3D glView3DInstance
    property GLViewXY glViewXYInstance
    property GLViewYZ glViewYZInstance
    property GLViewXZ glViewXZInstance

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
                    id: glViewXYLoader
                    anchors.fill: parent
                    active: true
                    sourceComponent: glViewXY
                    onLoaded: glViewXYInstance = glViewXYLoader.item
                }
            }

            // View 2
            Item {
                width: quadGrid.width / 2 - 4
                height: quadGrid.height / 2 - 4

                Loader {
                    id: glViewXZLoader
                    anchors.fill: parent
                    active: true
                    sourceComponent: glViewXZ
                    onLoaded: glViewXZInstance = glViewXZLoader.item
                }
            }

            // View 3
             Item {
                width: quadGrid.width / 2 - 4
                height: quadGrid.height / 2 - 4

                Loader {
                    id: glViewYZLoader
                    anchors.fill: parent
                    active: true
                    sourceComponent: glViewYZ
                    onLoaded: glViewYZInstance = glViewYZLoader.item
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
                        if (glViewXYInstance) glViewXYInstance.minVoxelThreshold = value
                        if (glViewYZInstance) glViewYZInstance.minVoxelThreshold = value
                        if (glViewXZInstance) glViewXZInstance.minVoxelThreshold = value
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
                        if (glViewXYInstance) glViewXYInstance.maxVoxelThreshold = value
                        if (glViewYZInstance) glViewYZInstance.maxVoxelThreshold = value
                        if (glViewXZInstance) glViewXZInstance.maxVoxelThreshold = value
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
        id: glViewXY
        GLViewXY { anchors.fill: parent; context: VolumeContext }
    }

    Component {
        id: glViewYZ
        GLViewYZ { anchors.fill: parent; context: VolumeContext }
    }

    Component {
        id: glViewXZ
        GLViewXZ { anchors.fill: parent; context: VolumeContext }
    }
}
