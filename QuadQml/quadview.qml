import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 600
    title: "Quad View"

    Grid {
        anchors.fill: parent
        columns: 2
        rows: 2
        spacing: 4

        Repeater {
            model: 4
            Rectangle {
                width: parent.width / 2 - 2
                height: parent.height / 2 - 2
                color: ["#d3d3d3", "#add8e6", "#90ee90", "#f08080"][index]
                border.color: "black"
                border.width: 1
                radius: 8

                Text {
                    anchors.centerIn: parent
                    text: "View " + (index + 1)
                    font.pointSize: 20
                    font.bold: true
                }
            }
        }
    }
}
