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

    // This property will hold the actual GLView3D instance
    property GLView3D glViewInstance

    RowLayout {
        anchors.fill: parent
        spacing: 4

        // Quad layout on the right
        Grid {
            id: quadGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            rows: 2
            spacing: 4

            Repeater {
                model: 4
                Item {
                    width: quadGrid.width / 2 - 4
                    height: quadGrid.height / 2 - 4

                    // Show OpenGL only in last item (index 3)
                    Loader {
                        id: glLoader
                        anchors.fill: parent
                        active: index === 3
                        sourceComponent: glView
                        onLoaded: {
                            // Store the loaded GLView3D instance
                            glViewInstance = glLoader.item
                        }
                    }

                    Rectangle {
                        anchors.fill: parent
                        visible: index !== 3
                        color: ["#d3d3d3", "#add8e6", "#90ee90", "#ffffff"][index]
                        border.color: "black"
                        border.width: 1
                        radius: 8

                        Label {
                            anchors.centerIn: parent
                            text: "View " + (index + 1)
                            font.pointSize: 20
                            font.bold: true
                        }
                    }
                }
            }
        }

        // Sliders on the left
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
                        if (glViewInstance) {
                            glViewInstance.minVoxelThreshold = value
                        }
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
                        if (glViewInstance) {
                            glViewInstance.maxVoxelThreshold = value
                        }
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

    // Component definition for GLView3D
    Component {
        id: glView
        GLView3D {
            anchors.fill: parent
        }
    }
}
