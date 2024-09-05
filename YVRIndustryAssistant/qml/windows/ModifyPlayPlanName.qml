import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import "../control"


Rectangle  {
    width: 459
    height: 344
    visible: true
    color: "transparent"
    Rectangle{
        width: 459
        height: 344
        color: "#22242C"
        radius: 16
        RowLayout{
            id: titleBar
            width: parent.width
            height: 72

            Text {
                Layout.leftMargin: 164
                text: qsTr("播控名称")
                font.bold: true
                color: "#FFFFFF"
                font.pixelSize: 16
            }

            Item {
                Layout.fillWidth: true
            }

            Image{
                Layout.rightMargin: 34
                Layout.preferredHeight:18
                Layout.preferredWidth:18
                source: "qrc:/res/image/yvr_close.png"
                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: mainWin.hideSubWin()
                }
            }
        }

        ColumnLayout{
            anchors.top: titleBar.bottom
            anchors.topMargin: 12
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 18

            YVRTextInput{
                id: name
                height: 44
                width: 363
                textPixel: 14
                defaultText: yvr.resourcesListModelAdd.name
                tipText: qsTr("请输入播控名称")
            }

            YVRTextInput{
                id: content
                height: 88
                width: 363
                textPixel: 14
                defaultText: yvr.resourcesListModelAdd.content
                tipText: qsTr("请输入播控简介")
            }


            YVRButton{
                Layout.topMargin: 16
                Layout.alignment: Qt.AlignHCenter
                width: 220
                height: 46
                onClickSignal: {
                    yvr.resourcesListModelAdd.name = name.inputText
                    yvr.resourcesListModelAdd.content = content.inputText
                    mainWin.hideSubWin()
                }
            }

        }
    }
}
