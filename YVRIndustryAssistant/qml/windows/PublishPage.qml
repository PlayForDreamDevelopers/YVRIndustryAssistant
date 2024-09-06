import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQuick.Dialogs 1.3
import "../control"

Rectangle{
    width: 520
    height: 468
    color: "#33343D"
    radius: 16
    property string showTitle: qsTr("创建播控")
    property int videoIndex : -1
    property var vrList
    signal ensure()
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
            Layout.leftMargin: 184
            text: showTitle
            font.bold: true
            color: "#FFFFFF"
            font.pixelSize: 16
        }

        Item {
            Layout.fillWidth: true
        }

        Text {
            Layout.rightMargin: 34
            text: qsTr("发布")
            font.bold: true
            color: "#3476FB"
            font.pixelSize: 14

            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked:{
                    yvr.play(videoIndex, loopPlay.visible, selectAll.visible)
                    mainWin.hideSubWin()
                    subWindow.source = ""
                }
            }
        }
    }

    ColumnLayout {
        anchors.top: parent.top
        anchors.topMargin: 90
        spacing: 8

        Text {
            Layout.leftMargin: 34
            text: selectAll.visible ? ("选择设备("+ vrList.rowCount() + ")") : vrList.selectCount
            font.pixelSize: 14
            font.bold: true
            color: "#DBDBDB"
        }

        Rectangle{
            Layout.leftMargin: 34
            width: 482
            height: 222
            color: "#45454B"
            radius: 8

            RowLayout{
                spacing: 0
                anchors.top: parent.top
                anchors.topMargin: 24

                RowLayout{
                    spacing: 0
                    Layout.preferredWidth: 70
                    Layout.leftMargin: 20
                    Rectangle{
                        width: 20
                        height: 20
                        Layout.leftMargin: 8
                        color: "transparent"
                        radius: 4
                        border.width: 2
                        border.color: "#B2B2B5"

                        Image{
                            id: selectAll
                            anchors.fill: parent
                            visible: false
                            source: "qrc:/res/image/yvr_select.png"
                        }

                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                selectAll.visible = !selectAll.visible
                            }
                        }
                    }

                    Text {
                        text: qsTr("全选")
                        font.pixelSize: 14
                        color: "#FFFFFF"
                    }
                }

                Text {
                    Layout.leftMargin: 30
                    Layout.preferredWidth: 60
                    text: qsTr("ID")
                    font.pixelSize: 12
                    color: "#FFFFFF"
                }

                Text {
                    Layout.leftMargin: 20
                    Layout.preferredWidth: 180
                    text: qsTr("设备SN号")
                    color: "#FFFFFF"
                    font.pixelSize: 12
                }

                Text {
                    Layout.leftMargin: 10
                    Layout.preferredWidth:80
                    color: "#FFFFFF"
                    text: qsTr("状态")
                    font.pixelSize: 12
                }
            }

            ListView{
                anchors.top: parent.top
                anchors.topMargin: 59
                width: parent.width
                height: 160
                bottomMargin: 16
                model: vrList
                clip: true
                delegate: RowLayout{
                    height: 54
                    spacing: 0
                    Rectangle{
                        width: 20
                        height: 20
                        Layout.leftMargin: 28
                        color: "transparent"
                        radius: 4
                        border.width: 2
                        border.color: "#B2B2B5"

                        Image{
                            anchors.fill: parent
                            visible:  ( selectAll.visible || vrSelected) && vrCanSelected
                            source: "qrc:/res/image/yvr_select.png"
                        }

                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                vrList.select(index, !vrSelected);
                            }
                        }

                    }

                    Text {
                        Layout.leftMargin: 70
                        Layout.preferredWidth: 60
                        text: vrID
                        color: "#99FFFFFF"
                        font.pixelSize: 12
                    }

                    Text {
                        Layout.leftMargin: 0
                        Layout.preferredWidth: 180
                        text: vrSN
                        color: "#99FFFFFF"
                        font.pixelSize: 12
                    }

                    Text {
                        Layout.leftMargin: 0
                        Layout.preferredWidth: 80
                        text: vrState
                        horizontalAlignment: Text.AlignHCenter
                        color: "#99FFFFFF"
                        font.pixelSize: 12
                    }
                }
            }
        }


        Text {
            Layout.leftMargin: 34
            text: qsTr("播放策略")
            font.pixelSize: 12
            font.bold: true
            color: "#FFFFFF"
        }

        Rectangle{
            width: 452
            height: 44
            color: "#45454B"
            radius: 8
            Layout.leftMargin: 34


            Rectangle{
                width: 20
                height: 20
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                radius: 4
                border.width: 2
                border.color: "#B2B2B5"

                Image{
                    id: loopPlay
                    anchors.fill: parent
                    source: "qrc:/res/image/yvr_select.png"
                }

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        loopPlay.visible = !loopPlay.visible
                    }
                }
            }

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 43
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("视频循环播放")
                font.pixelSize: 14
                font.bold: true
                color: "#DBDBDB"
            }
        }

    }
}

