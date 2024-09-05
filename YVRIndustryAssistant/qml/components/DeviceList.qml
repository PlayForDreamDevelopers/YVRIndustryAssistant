import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14
import QtQml 2.0

Item {
    anchors.fill: parent

    GridLayout{
        width: parent.width
        height: parent.height
        rowSpacing: 20
        columnSpacing: 20
        columns: 2
        Rectangle{
            radius: 16
            color: "#22242C"
            width: 620
            height: 424

            ColumnLayout{
                width:  parent.width
                RowLayout{
                    Layout.topMargin: 20
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20
                    Text {
                        text: qsTr("设备信息")
                        font.pixelSize: 20
                        color: "#FFFFFF"
                    }
                    Item{
                        Layout.fillWidth: true
                    }

                    Repeater{
                        model: [[qsTr("运行日志"), "qrc:/res/image/yvr_help.png"],
                            [qsTr("重启"),"qrc:/res/image/yvr_help.png"],
                            [qsTr("关机"),"qrc:/res/image/yvr_help.png"]]
                        Button{
                            Layout.preferredWidth: btncontent.implicitWidth + 40
                            Layout.preferredHeight: 40
                            background: Rectangle{
                                radius: 6
                                color: "#343541"
                            }

                            contentItem: RowLayout {
                                id: btncontent
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                spacing: 0
                                Image {
                                    Layout.preferredHeight: 20
                                    Layout.preferredWidth: 20
                                    source: modelData[1]
                                }

                                Text {
                                    text: modelData[0]
                                    font.pixelSize: 16
                                    color: "#FFFFFF"
                                }
                            }
                        }
                    }

                }
                Image{
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 45
                    Layout.preferredHeight: 120
                    Layout.preferredWidth: 326
                    source: "qrc:/res/image/yvr_d2.png"
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 20
                    font.pixelSize: 14
                    color: "#CDFFFFFF"
                    text: qsTr("系统版本：1.1.5.265")
                }

                RowLayout{
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 20
                    height: 32
                    spacing: 16

                    Repeater{
                        model: [[qsTr("54%"), "qrc:/res/image/yvr_help.png"],
                            [qsTr("60%"),"qrc:/res/image/yvr_help.png"],
                            [qsTr("60%"),"qrc:/res/image/yvr_help.png"]]
                        delegate: RowLayout{
                                    spacing: 4
                                    Image {
                                        Layout.preferredHeight: 20
                                        Layout.preferredWidth: 20
                                        source: modelData[1]
                                    }

                                    Text {
                                        text: modelData[0]
                                        font.pixelSize: 16
                                        color: "#FFFFFF"
                                    }
                                }
                    }

                }

                RowLayout{
                    height: 40
                    Layout.topMargin: 40
                    Layout.leftMargin: 20
                    Layout.rightMargin: 20

                    Text {
                        font.pixelSize: 20
                        color: "#FFFFFF"
                        text: qsTr("设备")
                    }

                    Rectangle{
                        id:tabbar
                        Layout.leftMargin: 14
                        width: 284
                        height: 40
                        color: "#343541"
                        radius: 6

                        RowLayout{
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter

                            Text {
                                Layout.leftMargin: 16
                                text: comboBox.curText
                                font.pixelSize: 20
                                color: "#FFFFFF"
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Image {
                                Layout.rightMargin: 16
                                Layout.preferredHeight: 16
                                Layout.preferredWidth: 16
                                source: "qrc:/res/image/yvr_arrow_down.png"
                            }
                        }

                        Popup{
                            id: comboBox
                            width: parent.width
                            height: tabbarContent.implicitHeight + 12
                            property string curText: "3200x1600"
                            property bool showCombox: comboxEditArea.containsMouse

                            background: Rectangle {
                                anchors.fill: parent
                                color: "#343541"
                                radius: 4
                            }
                            ColumnLayout{
                                id: tabbarContent
                                anchors.centerIn: parent
                                spacing: 6
                                Repeater{
                                    model: ["3200x1600", "2400x1200","1600x800"]
                                    delegate: Rectangle{
                                        width: 272
                                        height: 30
                                        radius: 4
                                        color: btnArea.containsMouse ? "#40414F" : "transparent"
                                        Text {
                                            anchors.left: parent.left
                                            anchors.leftMargin: 10
                                            text: modelData
                                            font.family: utils.yvr_font
                                            color: "#FFFFFF"
                                            font.pixelSize: 16
                                        }

                                        MouseArea{
                                            id: btnArea
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            hoverEnabled: true
                                            onClicked: {
                                                comboBox.curText = modelData
                                                comboBox.close()
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        MouseArea{
                            id: comboxEditArea
                            height: parent.height
                            width: 260
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            hoverEnabled: true
                            onClicked: {
                                comboBox.y = tabbar.height + 10
                                comboBox.open()
                            }
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                    Text {
                        font.pixelSize: 20
                        color: "#CDFFFFFF"
                        text: qsTr("已连接")
                    }
                }
            }
        }

        Rectangle{
            radius: 16
            color: "#22242C"
            Layout.fillWidth: true
            height: 424
        }
        Rectangle{
            radius: 16
            color: "#22242C"
            width: 620
            Layout.fillHeight: true
        }
        Rectangle{
            radius: 16
            color: "#22242C"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
