import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import "../control"

Rectangle {

    width: 420
    height: itemArea.height + 30
    color: "transparent"
    property string tipsText: qsTr("你确定要删除该资源？")
    property string tipsTitle: qsTr("确定删除")

    signal ensure()

    Rectangle{
        anchors.fill: parent
        radius: 12
        color: utils.yvr_itemdowncolor
        ColumnLayout {
            id: itemArea
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Text {
                Layout.topMargin: 30
                Layout.maximumWidth: 300
                Layout.alignment: Qt.AlignHCenter
                text: tipsTitle
                wrapMode: Text.WordWrap
                font.family: utils.yvr_font
                color: "#FFFFFF"
                font.pixelSize: 32
            }

            Text {
                Layout.topMargin: 30
                Layout.maximumWidth: 300
                Layout.alignment: Qt.AlignHCenter
                text: tipsText
                wrapMode: Text.WordWrap
                font.family: utils.yvr_font
                color: "#FFFFFF"
                font.pixelSize: 18
            }

           RowLayout{
               Layout.topMargin: 32
               YVRButton{
                   showText: qsTr("取消")
                   color: "#383C47"
                   onClickSignal: {
                        mainWin.hideSubWin()
                   }
               }

               YVRButton{
                   showText: qsTr("确定")
                   onClickSignal: {
                        ensure()
                        mainWin.hideSubWin()
                   }
               }
           }
        }
    }
}
