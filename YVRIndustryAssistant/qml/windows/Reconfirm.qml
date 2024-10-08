﻿import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQuick.Dialogs 1.3
import "../control"


Rectangle {
    width: 301
    height: 340
    visible: true
    color: "transparent"
    property string showTitle: qsTr("无法创建")
    property string showImage: "qrc:/res/image/yvr_tips_cancel.png"
    property string showContent:qsTr("播控方案内容也同步删除！")

    signal ensure()

    Rectangle{
        anchors.fill: parent
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
                onClicked: {
                    mainWin.hideSubWin()
                    subWindow.source = ""
                }
            }
        }

        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 34
                text: showTitle
                font.bold: true
                color: "#FFFFFF"
                font.pixelSize: 24
            }


            Image {
                Layout.topMargin: 44
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 64
                Layout.preferredWidth: 64
                source: showImage
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 12
                Layout.maximumWidth: 240
                wrapMode: Text.WrapAnywhere
                text: showContent
                font.pixelSize: 14
                font.bold: true
                color: "#DBDBDB"
            }

            YVRButton{
                Layout.topMargin: 44
                width: 220
                height: 46
                showText: qsTr("确认")
                Layout.alignment: Qt.AlignHCenter
                onClickSignal: {
                    ensure()
                    mainWin.hideSubWin()
                    subWindow.source = ""
                }
            }
        }
    }
}
