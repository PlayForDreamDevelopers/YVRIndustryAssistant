import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQml 2.0
import QtQuick.Controls 2.1
import YVRManager 1.0
import "./../control"

Item {
    id: root
    property bool selectDevice: false
    property bool selectAllItem: false
    ColumnLayout {
        spacing: 20
        width: parent.width
        height: parent.height

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            Layout.topMargin: 26
            Layout.leftMargin: 30
            spacing: 12

            Text {
                text: qsTr("设备在线/总数： ") + yvr.vrListModel.onlineAndAll
                font.bold: true
                color: "#FFFFFF"
                textFormat: Qt.RichText
                font.pixelSize: 20

            }

            Item {
                Layout.fillWidth: true
            }

            YVRImageText{
                showText: qsTr("在线")
                showIcon: "qrc:/res/image/yvr_online.png"
            }

            YVRImageText{
                showText: qsTr("离线")
                showIcon: "qrc:/res/image/yvr_offline.png"
            }

            Rectangle{
                width: 1
                height: 20
                color: "#7F7F7F"
            }

            YVRImageText{
                showText: qsTr("未佩戴")
                showIcon: "qrc:/res/image/yvr_idle.png"
            }
            YVRImageText{
                showText: qsTr("已佩戴")
                showIcon: "qrc:/res/image/yvr_onuse.png"
            }

            Rectangle{
                width: 1
                height: 20
                color: "#7F7F7F"
            }

            YVRImageText{
                showText: qsTr("不受控")
                showIcon: "qrc:/res/image/yvr_uncontrol.png"
            }
            YVRImageText{
                showText: qsTr("受控中")
                showIcon: "qrc:/res/image/yvr_control.png"
            }


            Rectangle{
                width: 1
                height: 20
                color: "#7F7F7F"
            }

            YVRImageText{
                showText: qsTr("播放")
                showIcon: "qrc:/res/image/yvr_play.png"
            }
            YVRImageText{
                Layout.rightMargin: 57
                showText: qsTr("暂停")
                showIcon: "qrc:/res/image/yvr_pause.png"
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            Layout.leftMargin: 30
            spacing: 12
            YVRButton{
                visible: selectDevice
                textPixel: 16
                width: 52
                height: 34
                color: "#33343D"
                showText: qsTr("全选")

                onClickSignal: {
                    selectAllItem = ! selectAllItem
                }
            }

            Rectangle{
                width: 100
                height: 34
                color: "#33343D"

                RowLayout{
                    anchors.centerIn: parent
                    Text {
                        id: combox
                        Layout.preferredWidth: 58
                        text: comboxItem.curGroup
                        font.pixelSize: 14
                        color: "#FFFFFF"
                    }

                    Image {
                        Layout.preferredHeight: 14
                        Layout.preferredWidth: 14
                        source: "qrc:/res/image/yvr_arrow_down.png"
                    }
                }

                Popup {
                    id: comboxItem
                    width: 100
                    padding: 12
                    y: 36
                    property string curGroup: qsTr("全部分组")
                    background: Rectangle{
                        anchors.fill: parent
                        color: "#33343D"
                    }

                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                    ColumnLayout{
                        anchors.horizontalCenter: parent.horizontalCenter
                        Repeater{
                            model: {

                                var groups = [qsTr("全部分组")]
                                var list = yvr.vrListModel.groups

                                for(var i = 0; i < list.length; i++) {
                                    groups.push(list[i])
                                }

                                return groups;
                            }
                            delegate: Rectangle{
                                width: 90
                                height: 30
                                radius: 4
                                color: btnArea.containsMouse ? "#177BE6" : "transparent"
                                Text {
                                    anchors.centerIn: parent
                                    text: modelData
                                    font.family: utils.yvr_font
                                    color: "#FFFFFF"
                                    font.pixelSize: 14
                                }

                                MouseArea{
                                    id: btnArea
                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    hoverEnabled: true
                                    onClicked: {
                                        comboxItem.curGroup = modelData
                                        comboxItem.close()
                                        yvr.vrListModel.selectGroup(modelData)
                                    }
                                }
                            }
                        }
                    }
                }

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if(comboxItem.visible)
                            comboxItem.close()
                         else
                            comboxItem.open()
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            YVRIconButton{
                visible: selectDevice
                showIcon: "qrc:/res/image/yvr_action_close.png"
                showText: qsTr("关机")
                onClickSignal: yvr.rebootP(selectAllItem)
            }

            YVRIconButton{
                visible: selectDevice
                showIcon: "qrc:/res/image/yvr_action_restart.png"
                showText: qsTr("重启")
                onClickSignal: yvr.rebootN(selectAllItem)
            }

            YVRIconButton{
                visible: selectDevice
                showIcon: "qrc:/res/image/yvr_delete.png"
                showText: qsTr("删除")
                onClickSignal: {

                    subWindow.source = "qrc:/qml/windows/Reconfirm.qml"
                    subWindow.item.showTitle = qsTr("删除设备")
                    subWindow.item.showImage = "qrc:/res/image/yvr_tips_delete.png"
                    subWindow.item.showContent = qsTr("请确认是否删除此设备？")

                    subWindow.item.ensure.connect(function(){
                        yvr.vrListModel.deleteDevices(selectAllItem)
                    })
                    mainWin.showSubWin()

                }
            }

            YVRIconButton{
                visible: selectDevice
                Layout.leftMargin: 22
                showIcon: "qrc:/res/image/yvr_action_group.png"
                showText:qsTr("分组")

                onClickSignal: {
                    subWindow.source = "qrc:/qml/windows/ModifyGroup.qml"
                    mainWin.showSubWin()
                }
            }

            YVRIconButton{
                visible: false
                showIcon: "qrc:/res/image/yvr_action_preview.png"
                showText: qsTr("监控")
            }

            Rectangle{
                Layout.leftMargin: 22
                width: 144
                height: 44
                color: "transparent"

                RowLayout
                {
                    spacing: 8
                    anchors.centerIn: parent
                    Image {
                        Layout.preferredHeight: 28
                        Layout.preferredWidth: 28
                        source: "qrc:/res/image/yvr_add_devices.png"
                    }

                    Text {
                        text: qsTr("添加设备")
                        font.pixelSize: 20
                        color: "#FFFFFF"
                    }
                }

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: dicoverDevice.source = "qrc:/qml/components/DeviceDiscoverPage.qml"
                }
            }

            Rectangle{
                Layout.rightMargin: 44
                width: 144
                height: 44
                color: selectDevice ? "#33343D" : "transparent"

                RowLayout
                {
                    spacing: 8
                    anchors.centerIn: parent
                    Image {
                        Layout.preferredHeight: 28
                        Layout.preferredWidth: 28
                        source: "qrc:/res/image/yvr_select_mode.png"
                    }

                    Text {
                        text: qsTr("批量管理")
                        font.pixelSize: 20
                        color: "#FFFFFF"
                    }
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        selectDevice = !selectDevice
                    }
                }
            }
        }

        GridView{
            Layout.preferredWidth: parent.width
            Layout.fillHeight: true
            leftMargin: 20
            rightMargin: 20
            topMargin: 0
            cellHeight: 233
            cellWidth: 378
            model: yvr.vrListModel
            clip: true

            delegate: Item {
                id: delegateItem
                width: 378
                height: 233
                YVRDevicesItem {
                    vrItemSn: vrSN
                    vrItemGroup: vrGroup
                    vrItemId: vrID
                    online: vrEnable
                    deviceStatus: vrState
                    power : vrBattery

                    anchors.centerIn: parent
                    selected: vrSelect
                    selectedMode: selectDevice
                    selectAll: selectAllItem

                    onClickSignal: {

                        var pos = mapToItem(root, 0, 0)

                        menu.vrId = vrID
                        menu.vrGroup = vrGroup
                        menu.sn = vrSN

                        menu.x = pos.x + 132
                        menu.y = pos.y  + 215
                        menu.open()
                    }
                }
            }
        }
    }

    Menu{
        id: menu
        property string sn: ""
        property string vrId: ""
        property string vrGroup: ""
        width: 220
        height: 162
        padding: 12

        background: Rectangle{
            color: "#45454B"
            radius: 4
        }

        Repeater{
            model: [ qsTr("编辑"), qsTr("重启"), qsTr("关机"),qsTr("删除")]
            delegate: MenuItem {
                    width: 196
                    height: 34
                    background: Rectangle{
                        anchors.centerIn: parent
                        radius: 4
                        anchors.fill: parent
                        color: cellArea.containsMouse ? "#177BE6" : "transparent"
                    }

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        text: modelData
                        font.family: utils.yvr_font
                        color: "#FFFFFF"
                        font.pixelSize: 18
                    }

                    MouseArea{
                        id: cellArea
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {

                            if(index == 0)
                            {
                                subWindow.source = "qrc:/qml/windows/ModifyDevices.qml"
                                subWindow.item.deviceID = menu.vrId
                                subWindow.item.deviceSN = menu.sn
                                subWindow.item.deviceGroup = menu.vrGroup
                                subWindow.item.discover = false
                                mainWin.showSubWin()
                            }else if(index == 1)
                            {
                                yvr.rebootN(menu.sn)
                            }else if(index == 2)
                            {
                                yvr.rebootP(menu.sn)
                            }else if(index == 3)
                            {

                                subWindow.source = "qrc:/qml/windows/Reconfirm.qml"
                                subWindow.item.showTitle = qsTr("删除设备")
                                subWindow.item.showImage = "qrc:/res/image/yvr_tips_delete.png"
                                subWindow.item.showContent = qsTr("请确认是否删除此设备？")

                                subWindow.item.ensure.connect(function(){
                                     yvr.vrListModel.deleteDevice(menu.sn)
                                })

                                mainWin.showSubWin()

                            }

                            menu.close()
                        }
                    }
                }
        }
    }

    Loader{
        id: dicoverDevice
        anchors.fill: parent
    }
}

