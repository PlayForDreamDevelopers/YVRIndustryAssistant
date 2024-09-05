import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0

FramelessWindow {
    needShadow: false
    width: 400
    height: 300
    visible: true
    color: "transparent"
    titleBar: titleBar

    Rectangle{
        anchors.fill: parent
        radius: 12
        color: "#22242C"
        ColumnLayout {
            width: parent.width
            spacing: 0
            RowLayout{
                id:titleBar
                height: 82
                Text {
                    Layout.leftMargin: 30
                    Layout.topMargin: 40
                    text: qsTr("提示")
                    font.bold: true
                    font.family: "Source Han Sans CN Medium"
                    color: "#FFFFFF"
                    font.pixelSize: 36
                }

                Item {
                    Layout.fillWidth: true
                }

                Image{
                    Layout.rightMargin: 30
                    width:24
                    height: 24
                    source: "qrc:/res/image/yvr_close.png"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: close()
                    }
                }
            }

            Item{
                Layout.fillHeight: true
            }

            Text {
                Layout.topMargin: 50
                text: qsTr("应用程序已打开")
                Layout.alignment: Qt.AlignHCenter
                font.family: "Source Han Sans CN Medium"
                color: "#FFFFFF"
                font.pixelSize: 20
            }

            Rectangle {
                width: 180
                height: 54
                radius: 8
                Layout.topMargin: 40
                Layout.alignment: Qt.AlignHCenter
                color: "#177BE6"
                Text {
                    anchors.centerIn: parent
                    text: qsTr("确定")
                    font.family: "Source Han Sans CN Medium"
                    color: "#FFFFFF"
                    font.bold: true
                    font.pixelSize: 20
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        close()
                    }
                }
            }
        }
    }

    Timer{
        interval: 4000; running: true; repeat: false
        onTriggered: close()
    }
}
