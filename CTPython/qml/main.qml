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
                        anchors.fill: parent
                        active: index === 3
                        sourceComponent: glView
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

      Component {
        id: glView
        GLView3D {
                anchors.fill: parent
            }
      }
}
}