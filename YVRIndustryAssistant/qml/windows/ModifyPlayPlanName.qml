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

        Image{
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            width:24
            height: 24

            source: "qrc:/res/image/yvr_close.png"
            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: mainWin.hideSubWin()
            }
        }


        ColumnLayout{
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 18

            Text {
                Layout.topMargin: 34
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("播控名称")
                font.bold: true
                color: "#FFFFFF"
                font.pixelSize: 16
            }

            YVRTextInput{
                id: name
                Layout.topMargin: 34
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
