import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.15

ApplicationWindow {
    width: 1000
    height: 600
    visible: true
    title: "TreeView with Quad Layout"

    RowLayout {
        anchors.fill: parent
        spacing: 4

        // TreeView on the left
        TreeView {
            id: treeView
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            clip: true

            model: yourTreeModel
            selectionModel: yourSelectionModel

            delegate: Item {
                implicitWidth: padding + label.x + label.implicitWidth + padding
                implicitHeight: label.implicitHeight * 1.5

                readonly property real indentation: 20
                readonly property real padding: 5

                required property TreeView treeView
                required property bool isTreeNode
                required property bool expanded
                required property bool hasChildren
                required property int depth
                required property int row
                required property int column
                required property bool current

                Rectangle {
                    anchors.fill: parent
                    color: current ? Qt.application.palette.highlight : "black"
                    opacity: (treeView.alternatingRows && row % 2 !== 0) ? 0.3 : 0.1
                }

                Label {
                    id: indicator
                    x: padding + (depth * indentation)
                    anchors.verticalCenter: parent.verticalCenter
                    visible: isTreeNode && hasChildren
                    text: "▶"
                    rotation: expanded ? 90 : 0

                    Behavior on rotation {
                        NumberAnimation {
                            duration: 100
                            easing.type: Easing.OutQuart
                        }
                    }

                    TapHandler {
                        onSingleTapped: {
                            let index = treeView.index(row, column)
                            treeView.selectionModel.setCurrentIndex(index, 0)
                            treeView.toggleExpanded(index)
                        }
                    }
                }

                Label {
                    id: label
                    x: padding + (isTreeNode ? (depth + 1) * indentation : 0)
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - padding - x
                    clip: true
                    text: model.display
                }
            }
        }

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
}