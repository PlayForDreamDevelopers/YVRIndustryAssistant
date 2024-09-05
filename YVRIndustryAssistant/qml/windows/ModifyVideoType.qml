import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import "../control"


Rectangle {

    width: 355
    height: 154
    color: "transparent"
    property string name: ""
    property int pos: -1

    Rectangle{
        width: 355
        height: 154
        color: "#2E313B"
        radius: 16
        RowLayout{
            width: parent.width
            height: 72
            id: titleBar
            Image{
                Layout.leftMargin: 34
                Layout.preferredHeight:18
                Layout.preferredWidth:18
                source: "qrc:/res/image/yvr_close.png"
                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: mainWin.hideSubWin()
                }
            }

            Text {
                Layout.leftMargin: 84
                text: qsTr("编辑分组")
                font.bold: true
                color: "#FFFFFF"
                font.pixelSize: 16
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                Layout.rightMargin: 34
                text: qsTr("确认")
                font.bold: true
                color: "#3476FB"
                font.pixelSize: 14

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                     onClicked:{
                          yvr.resourcesListModelAdd.modifyGroup(pos, input.text)
                          mainWin.hideSubWin()
                     }
                }
            }
        }

        ColumnLayout{
            anchors.top: titleBar.bottom
            anchors.topMargin: 12
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 16

            RowLayout{
                spacing: 18
                Text {
                    Layout.preferredWidth: 60
                    text: qsTr("分类名称")
                    color: "#DBDBDB"
                    font.pixelSize: 16
                }

                Rectangle {
                    radius: 8
                    width: 220
                    height: 32
                    color: "#45454B"

                    TextInput{
                        id: input
                        enabled: true
                        anchors.fill: parent
                        wrapMode: Text.WrapAnywhere
                        leftPadding:16
                        rightPadding: 16
                        topPadding: 4
                        font.pixelSize: 18
                        color: "#DBDBDB"
                        selectByMouse: true
                        text: name
                    }
                }
            }
        }

    }
}
