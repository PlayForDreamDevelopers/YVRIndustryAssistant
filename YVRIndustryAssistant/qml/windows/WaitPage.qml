import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQuick.Controls 2.14
import QtQml 2.0
import "../common"

FramelessWindow {
    needShadow: false
    width: 380
    height: 314
    modality: Qt.ApplicationModal
    color: "transparent"
    property string uuid

    Rectangle{
        anchors.fill: parent
        radius: 12
        color: utils.yvr_itemdowncolor
        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Text {
                Layout.topMargin: 40
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("视频文件导入中，请耐心等待")
                font.family: utils.yvr_font
                Layout.maximumWidth: 300
                wrapMode: Text.WordWrap
                color: "#FFFFFF"
                font.pixelSize: 20
            }

            QmlBusyIndicator{
                Layout.topMargin: 40
                Layout.alignment: Qt.AlignHCenter
                width: 40
                height: 40
                looptime: Animation.Infinite
            }

            Text {
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("加载中…")
                font.family: utils.yvr_font
                color: "#A4A4A5"
                font.pixelSize: 16
            }

            Rectangle{
                width: 270
                height: 54
                Layout.topMargin: 40
                Layout.alignment: Qt.AlignHCenter
                color: "#4F7FFE"
                radius: 8
                Text {
                    anchors.centerIn: parent
                    text: qsTr("取消")
                    font.family: utils.yvr_font
                    color: "#F3F3F3"
                    font.pixelSize: 24
                }

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        yvr.cacelAction(uuid)
                        close()
                    }
                }
            }
        }
    }
}
