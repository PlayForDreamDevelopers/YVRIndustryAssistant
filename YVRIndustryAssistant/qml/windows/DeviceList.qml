import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import "../control"

Rectangle  {
    width: 1200
    height: 694
    visible: true
    color: "transparent"

    Rectangle{
        id: rect
        anchors.fill: parent
        color: "#22242C"
        radius: 16

        ColumnLayout{
            spacing: 0
            width: parent.width
            RowLayout{
                width: parent.width
                height: 82
                Text {
                    Layout.leftMargin: 428
                    Layout.topMargin: 30
                    text: qsTr("设备监控")
                    font.bold: true
                    color: "#FFFFFF"
                    font.pixelSize: 36
                }

                Item {
                    Layout.fillWidth: true
                }

                Image{
                    Layout.rightMargin: 35
                    width:24
                    height: 24
                    source: "qrc:/res/image/yvr_close.png"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: mainWin.hideSubWin()
                    }
                }
            }

            RowLayout{
                width: parent.width
                Layout.topMargin: 44
                spacing: 0
                Rectangle{
                    width: 20
                    height: 20
                    Layout.leftMargin: 30
                    color: "transparent"
                    radius: 4
                    border.width: 2
                    border.color: "#B2B2B5"

                    Image{
                        id: selectAll
                        anchors.fill: parent
                        visible: yvr.selectAll
                        source: "qrc:/res/image/yvr_select.png"
                    }

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            var res = !selectAll.visible
                            selectAll.visible = res
                            yvr.selectAll= res
                        }
                    }
                }

                Text {
                    font.pixelSize: 18
                    Layout.leftMargin: 6
                    color: "#FFFFFF"
                    text: qsTr("全选")
                }

                Item {
                    Layout.fillWidth: true
                }

                Text {
                    font.pixelSize: 18
                    color: "#FFFFFF"
                    text: qsTr("设备状态:")
                }

                RowLayout{
                    Layout.rightMargin: 30
                    Layout.leftMargin: 20
                    Repeater{
                        model: [[qsTr("离线"),"qrc:/res/image/yvr_offline.png"],
                            [qsTr("未佩戴"),"qrc:/res/image/yvr_idle.png"],
                            [qsTr("已佩戴"),"qrc:/res/image/yvr_onuse.png"],
                            [qsTr("不受控"),"qrc:/res/image/yvr_uncontrol.png"],
                            [qsTr("受控中"),"qrc:/res/image/yvr_control.png"],
                            [qsTr("播放"),"qrc:/res/image/yvr_play.png"],
                            [qsTr("暂停"),"qrc:/res/image/yvr_pause.png"],
                            [qsTr("播放异常"),"qrc:/res/image/yvr_play_error.png"]]
                        delegate: RowLayout{
                            Text {
                                font.pixelSize: 18
                                color: "#FFFFFF"
                                text:modelData[0]
                            }

                            Image {
                                Layout.preferredHeight: 14
                                Layout.preferredWidth: 14
                                source: modelData[1]
                            }
                        }
                    }
                }

            }

            GridView{
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 420
                leftMargin: 20
                rightMargin: 20
                topMargin: 20
                cellHeight: 128
                cellWidth: 290
                model: yvr.vrListModel
                clip: true

                delegate: Item {
                    id: delegateItem
                    width: 290
                    height: 128
                    Rectangle{
                        anchors.centerIn: parent
                        width: 270
                        height: 108
                        color: "#33343D"
                        radius: 8
                        enabled: vrEnable
                        opacity: enabled ? 1 : 0.4

                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onEntered: {
                                var pos = delegateItem.mapToItem(rect, 0, 0)
                                status.x = pos.x + (290 - 104)/2
                                status.y = pos.y - 46
                                status.listModel = vrState
                                status.open()
                            }
                            onExited: {
                                status.close()
                            }
                        }

                        ColumnLayout{
                            width: parent.width
                            spacing: 8
                            RowLayout{
                                Layout.topMargin: 18
                                spacing: 7
                                Rectangle{
                                    width: 20
                                    height: 20
                                    Layout.leftMargin: 10
                                    color: "transparent"
                                    radius: 4
                                    border.width: 2
                                    border.color: "#B2B2B5"

                                    Image{
                                        id: selectImage
                                        anchors.fill: parent
                                        property bool doSelected: vrSelect
                                        visible: ( yvr.selectAll || doSelected ) && vrEnable
                                        source: "qrc:/res/image/yvr_select.png"
                                    }

                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            selectImage.doSelected = !selectImage.doSelected
                                            yvr.selectOrCancelDevices(vrSN, selectImage.doSelected, true)
                                        }
                                    }
                                }

                                Text {
                                    Layout.preferredWidth: 190
                                    font.pixelSize: 16
                                    color: "#FFFFFF"
                                    text: qsTr("ID: ") + vrID
                                }

                                Image {
                                    Layout.preferredHeight: 18
                                    Layout.preferredWidth: 18
                                    source: vrState[0]
                                }
                            }

                            Text {
                                Layout.leftMargin: 37
                                font.pixelSize: 16
                                color: "#FFFFFF"
                                text: qsTr("SN: ") + vrSN
                            }

                            Rectangle{
                                id: singleControlBtn
                                Layout.alignment: Qt.AlignRight
                                Layout.rightMargin: 10
                                width: 48
                                height: 24
                                color: state ?  "#1E68FF": "#41424B"
                                radius: 12

                                property bool state: vrControl

                                Rectangle{
                                    anchors.left: parent.left
                                    anchors.leftMargin: singleControlBtn.state ? 28 : 4
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 16
                                    height: 16
                                    radius: 8
                                    color: "#D8D8D8"
                                }

                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        yvr.changeControlBySn(vrSN, singleControlBtn.state)
                                        //singleControlBtn.state = !singleControlBtn.state
                                    }
                                }
                            }
                        }
                    }
                }
            }

            RowLayout{
                spacing: 0
                width: parent.width
                Layout.topMargin: 50
                opacity: enabled ? 1.0: 0.6
                enabled:yvr.canDoAction
                Text {
                    Layout.leftMargin: 30
                    font.pixelSize: 18
                    color: "#FFFFFF"
                    text: qsTr("控制设备")
                }

                Rectangle{
                    id: controlBtn
                    Layout.leftMargin: 10
                    width: 48
                    height: 24
                    color: yvr.canDoControl ? "#1E68FF" : "#41424B"
                    radius: 12


                    Rectangle{
                        anchors.left: parent.left
                        anchors.leftMargin: yvr.canDoControl ? 28 : 4
                        anchors.verticalCenter: parent.verticalCenter
                        width: 16
                        height: 16
                        radius: 8
                        color: "#D8D8D8"
                    }

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            yvr.changeControlSelected(yvr.canDoControl )
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                YVRButton{
                    width: 140
                    color: "#606171"
                    showText: qsTr("关闭设备")
                    onClickSignal: {
                        yvr.rebootP()
                    }
                }

                YVRButton{
                    Layout.rightMargin: 30
                    Layout.leftMargin: 20
                    width: 140
                    color: "#606171"
                    showText: qsTr("重启设备")
                    onClickSignal: {
                        yvr.rebootN()
                    }
                }
            }
        }
     }

    Popup{
        id: status
        width: 104
        height: 46
        padding: 0
        property var listModel: []
        background: Item {
            width: parent.width
            height: parent.height
            ColumnLayout{
                width: parent.width
                spacing: 0
                Rectangle{
                    width: 104
                    height: 40
                    radius: 8
                    color: "#33343D"
                }

                Image {
                    Layout.preferredWidth: 14
                    Layout.preferredHeight: 7
                    Layout.alignment: Qt.AlignHCenter
                    source: "qrc:/res/image/yvr_combine.png"
                }
            }
        }

        RowLayout{
            x: 12
            y: 10
            spacing: 10
            Repeater{
                model: status.listModel
                Image {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    source: modelData
                }
            }
        }
    }

//    onVisibleChanged: {
//        yvr.setRefreshState(visible)
//    }

//    Component.onCompleted: {
//        yvr.setRefreshState(true)
//    }
}
