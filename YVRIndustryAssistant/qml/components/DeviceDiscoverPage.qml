import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import "./../control"
import YVRManager 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "#22242C"
    property bool selectAll: false
    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        RowLayout{

            Layout.topMargin: 25
            Image {
                Layout.leftMargin: 30
                Layout.preferredHeight: 28
                Layout.preferredWidth: 28
                source: "qrc:/res/image/yvr_back.png"

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        root.parent.source = ""
                        mainWin.showPage(YVRManagerCLS.DeviceManager)
                    }
                }
            }

            Text {
                Layout.leftMargin: 24
                text: qsTr("添加设备")
                font.pixelSize: 24
                font.bold: true
                color: "#FFFFFF"
            }

            Item {
                Layout.fillWidth: true
            }

            YVRButton{
                showText: qsTr("手动添加")
                width: 100
                height: 34
                color: "#3A3A3A"
                onClickSignal: {
                    subWindow.source = "qrc:/qml/windows/AddDevices.qml"
                    subWindow.item.acceptImport.connect(function(){
                        root.parent.source = ""
                    })

                    mainWin.showSubWin()
                }
            }

            YVRButton{
                Layout.leftMargin: 12
                Layout.rightMargin: 30
                showText: qsTr("添加")
                width: 100
                height: 34
                color: "#3A3A3A"
                onClickSignal: {
                    yvr.addDiscoverDeice(selectAll)
                    root.parent.source = ""
                    mainWin.showPage(YVRManagerCLS.DeviceManager)
                }
            }
        }

        RowLayout{
            Layout.topMargin: 40

            Text {
                Layout.leftMargin: 30
                text: qsTr("发现设备数量：") + yvr.vrDicoverListModel.count
                font.pixelSize: 14
                font.bold: true
                color: "#FFFFFF"
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                Layout.rightMargin: 30
                text: qsTr("如何让设备出现在设备列表？")
                font.pixelSize: 14
                font.bold: true
                color: "#FFFFFF"

                MouseArea{
                    id: showTips
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                }
            }
       }

        Rectangle
        {
            Layout.topMargin: 40
            Layout.fillWidth: true
            height: 46
            Layout.leftMargin: 30
            Layout.rightMargin: 30
            color: "#3C3F4C"
            RowLayout{

                anchors.verticalCenter: parent.verticalCenter
                spacing: 0

                Rectangle{
                        width: 20
                        height: 20
                        Layout.leftMargin: 16
                        color: "transparent"
                        radius: 4
                        border.width: 2
                        border.color: "#B2B2B5"

                        Image{
                            anchors.fill: parent
                            visible: selectAll
                            source: "qrc:/res/image/yvr_select.png"
                        }

                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                selectAll = !selectAll
                            }
                        }
                    }

                Text {
                    Layout.leftMargin: 12
                        text: qsTr("全选")
                        font.pixelSize: 18
                        color: "#FFFFFF"
                }


                Text {
                    Layout.leftMargin: 120
                    Layout.preferredWidth: 60
                    text: qsTr("设备ID")
                    font.pixelSize: 18
                    color: "#FFFFFF"
                }

                Text {
                    Layout.leftMargin: 280
                    Layout.preferredWidth: 200
                    text: qsTr("设备SN号")
                    color: "#FFFFFF"
                    font.pixelSize: 18
                }

                Text {
                    Layout.leftMargin: 280
                    Layout.preferredWidth:120
                    color: "#FFFFFF"
                    text: qsTr("更多操作")
                    font.pixelSize: 18
                }
            }
        }

        ListView{
            Layout.topMargin: 0
            Layout.fillHeight: true
            width: parent.width
            bottomMargin: 16
            model: yvr.vrDicoverListModel
            clip: true
            delegate: RowLayout{
                height: 54
                spacing: 0
                Rectangle{
                    width: 20
                    height: 20
                    Layout.leftMargin: 48
                    color: "transparent"
                    radius: 4
                    border.width: 2
                    border.color: "#B2B2B5"

                    Image{
                        id: selectImage
                        anchors.fill: parent
                        property bool doSelected: vrSelect
                        visible: selectAll || doSelected
                        source: "qrc:/res/image/yvr_select.png"
                    }

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            selectImage.doSelected = !selectImage.doSelected
                            yvr.vrDicoverListModel.select(vrSN, selectImage.doSelected)
                        }
                    }

                }

                Text {
                    Layout.leftMargin: 180
                    Layout.preferredWidth: 120
                    text: vrID
                    color: "#FFFFFF"
                    font.pixelSize: 18
                }

                Text {
                    Layout.leftMargin: 160
                    Layout.preferredWidth: 300
                    text: vrSN
                    color: "#FFFFFF"
                    font.pixelSize: 18
                }

                Text {
                    Layout.leftMargin: 240
                    text: qsTr("编辑")
                    color: "#3476FB"
                    font.pixelSize: 18

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            subWindow.source = "qrc:/qml/windows/ModifyDevices.qml"
                            subWindow.item.deviceID = vrID
                            subWindow.item.deviceSN = vrSN
                            subWindow.item.deviceGroup = vrGroup
                            subWindow.item.discover = true
                            mainWin.showSubWin()
                        }
                    }
                }
            }
        }
    }

    Popup{
        id: deviceGroups
        visible: showTips.containsMouse
        padding: 24
        x:745
        y:133
        width: 417
        height: 198
        background: Rectangle {
            color: "#45454B"
            anchors.fill: parent
            radius: 8
        }

        ColumnLayout{
            spacing: 0

            Text {
                Layout.leftMargin: 10
                text: qsTr("如何让设备出现在设备列表？")
                font.pixelSize: 18
                font.bold: true
                color: "#FFFFFF"
            }

            Text {
                Layout.topMargin: 10
                Layout.leftMargin: 10
                Layout.maximumWidth: 350
                text: qsTr("添加设备前，请完成以下3步操作：\n1.前往VR设备【设置】-【网络】，将VR设备与电脑连接在同一局域网下：\n2.在【资源库】打开播控应用\n3.在播控应用主界面，点击设置。打开接受控制端控制开关。完成以上3步。")
                font.pixelSize: 14
                font.bold: true
                color: "#C1C1C1"
                wrapMode: Text.WrapAnywhere
            }
        }
    }

}
