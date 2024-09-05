import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import "../control"


Rectangle  {

    width: 355
    height: 154 + deviceGroups.height
    visible: true
    color: "transparent"

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
                    text: qsTr("设备组别")
                    color: "#DBDBDB"
                    font.pixelSize: 16
                }

                Rectangle {
                    radius: 8
                    width: 220
                    height: 32
                    color: "#45454B"

                    RowLayout{

                        anchors.verticalCenter: parent.verticalCenter

                        Text{
                            font.pixelSize: 18
                            leftPadding: 16
                            Layout.preferredWidth: 190
                            color: "#DBDBDB"
                            text: deviceGroups.curGroup
                        }

                        Image {
                            Layout.preferredHeight: 14
                            Layout.preferredWidth: 14
                            source: "qrc:/res/image/yvr_arrow_down.png"
                        }
                    }


                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                            {
                            if(deviceGroups.visible)
                                deviceGroups.close()
                             else
                                deviceGroups.open()
                            }
                    }
                }
            }
        }

        Popup{
            id: deviceGroups
            x:108
            y:118
            padding: 12
            property string curGroup: yvr.vrListModel.groups[0]
            background: Rectangle {
                color: "#45454B"
                anchors.fill: parent
                radius: 8
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 0
                Repeater{
                    model: yvr.vrListModel.groups
                    delegate: Rectangle{
                        width: 196
                        height: 34
                        radius: 4
                        color: deviceActionArea.containsMouse ? "#177BE6" : "transparent"
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            text: modelData
                            font.family: utils.yvr_font
                            color: "#FFFFFF"
                            font.pixelSize: 14
                        }

                        MouseArea{
                            id: deviceActionArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: {
                                deviceGroups.curGroup = modelData
                                deviceGroups.close()
                            }
                        }

                        Image {
                            anchors.right: parent.right
                            anchors.rightMargin: 12
                            anchors.verticalCenter: parent.verticalCenter
                            width: 18
                            height: 18
                            visible:  index !== 0
                            source: "qrc:/res/image/yvr_close.png"

                            MouseArea{
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                hoverEnabled: true
                                onClicked: {
                                    yvr.vrListModel.modiftyGroup(false, modelData)
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.topMargin: 12
                    Layout.bottomMargin: 12
                    width: 196
                    height: 1
                    color: "#575761"
                }

                RowLayout{
                    spacing: 8

                    Rectangle {
                        radius: 8
                        width: 124
                        height: 32
                        color: "#515156"

                        TextInput{
                            id: groupName
                            enabled: true
                            anchors.fill: parent
                            wrapMode: Text.WrapAnywhere
                            leftPadding:16
                            rightPadding: 16
                            topPadding: 4
                            font.pixelSize: 18
                            color: "#DBDBDB"
                            selectByMouse: true
                            text: qsTr("组名1")
                        }
                    }

                    YVRButton {
                        width: 64
                        height: 32
                        color: "#3A3A3A"
                        showText: qsTr("添加")
                        textColor: "#C4C4C4"
                        onClickSignal: {
                            yvr.vrListModel.modiftyGroup(true, groupName.text);
                        }
                    }
                }
            }
        }

    }
}
