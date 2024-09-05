import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtQml 2.0
import QtQml.Models 2.1
import QtQuick.Controls 2.1
import YVRManager 1.0
import "./../control"



Image {
    id: root
    anchors.fill: parent
    source: "qrc:/res/image/yvr_playcontrol_back.png"
    property int curIndex: -1

    Rectangle{
        width: parent.width
        height: 88
        gradient: Gradient {
            orientation: Gradient.Vertical
            GradientStop { position: 0.0; color: "#CC33343D" }
            GradientStop { position: 1.0; color: "#997C7E92" }
        }

        RowLayout{
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter

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
                    }
                }
            }

            ColumnLayout{
                Layout.leftMargin: 44
                RowLayout{
                    spacing: 30
                    Text {
                        text: yvr.resourcesListModelAdd.name
                        color: "#FFFFFF"
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Image {
                        Layout.preferredHeight: 18
                        Layout.preferredWidth: 18
                        source: "qrc:/res/image/yvr_edit.png"
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                subWindow.source = "qrc:/qml/windows/ModifyPlayPlanName.qml"
                                mainWin.showSubWin()
                            }
                        }
                    }
                }

                Text {
                    text: yvr.resourcesListModelAdd.content
                    font.pixelSize: 12
                    color: "#FFFFFF"
                }
            }

            Item {
                Layout.fillWidth: true
            }

            YVRButton{
                width: 100
                height: 34
                Layout.rightMargin: 34
                showText: qsTr("删除方案")
                color: "#3A3A3A"
                textColor: "#C4C4C4"
                textPixel: 18
                onClickSignal: {
                    subWindow.source = "qrc:/qml/windows/Reconfirm.qml"
                    subWindow.item.showTitle = qsTr("删除方案")
                    subWindow.item.showImage = "qrc:/res/image/yvr_tips_delete.png"
                    subWindow.item.showContent = qsTr("播控方案内容也同步删除！")

                    subWindow.item.ensure.connect(function(){
                        yvr.clearPlan()
                        root.parent.source = ""
                    })
                    mainWin.showSubWin()
                }
            }
        }
    }

    Rectangle
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 130
        width: 822
        height: 476
        radius: 12

        RowLayout{
            spacing: 0
            anchors.fill: parent
            ColumnLayout{
                Layout.leftMargin: 12
                Layout.topMargin: 24
                Layout.preferredWidth: 153

                Image {
                    Layout.bottomMargin: 18
                    Layout.preferredWidth: 134
                    Layout.preferredHeight: 50
                    source: yvr.resourcesListModelAdd.logo === "" ? "qrc:/res/image/yvr_logo_add.png" : yvr.resourcesListModelAdd.logo

                    Rectangle{
                        anchors.fill: parent
                        radius: 8
                        color: "#66000000"
                        visible: logoArea.containsMouse && yvr.resourcesListModelAdd.logo !== ""

                        Image {
                            width: 18
                            height: 18
                            anchors.centerIn: parent
                            source: "qrc:/res/image/yvr_edit.png"
                        }
                    }

                    MouseArea{
                        id:logoArea
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {
                            subWindow.source = "qrc:/qml/windows/AddLogo.qml"
                            mainWin.showSubWin()
                        }
                    }
                }

                Repeater{
                    model: yvr.resourcesListModelAdd.videoGroups

                    delegate: Rectangle{
                        width: 141
                        height: 34
                        radius: 4
                        color: curIndex == index ? "#33343D" :  videoTypeArea.containsMouse ? "#6E6E6E" : "transparent"
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            text: modelData
                            font.family: utils.yvr_font
                            color: "#FFFFFF"
                            font.pixelSize: 14
                        }

                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 24
                            width: 12
                            height: 12
                            rotation: 180
                            source: "qrc:/res/image/yvr_back.png"
                        }

                        MouseArea{
                            id: videoTypeArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            onClicked: {

                                if(mouse.button === Qt.RightButton)
                                {
                                    subWindow.source = "qrc:/qml/windows/ModifyVideoType.qml"
                                    subWindow.item.pos = index
                                    subWindow.item.name = modelData

                                    var pos = mapToItem(root, 0, 0)
                                    menu.x = pos.x + 32
                                    menu.y = pos.y  + 40
                                    menu.pos = index
                                    menu.isGroup = true
                                    menu.open()
                                }else
                                {
                                    curIndex = index
                                }
                            }
                        }
                    }
                }

                Text {
                    text: qsTr("+  添加新分类")
                    color: "#6E6E6E"
                    font.pixelSize: 12
                    Layout.leftMargin: 6
                    Layout.topMargin: 12
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            var pos = yvr.resourcesListModelAdd.addOrDelGroup(0, true)

                            subWindow.source = "qrc:/qml/windows/ModifyVideoType.qml"
                            subWindow.item.pos = pos
                            subWindow.item.name = yvr.resourcesListModelAdd.videoGroups[pos]
                            mainWin.showSubWin()
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }

                Text {
                    color: "#6E6E6E"
                    font.pixelSize: 8
                    Layout.bottomMargin: 18
                    Layout.leftMargin: 6
                    text: yvr.getVersion()
                }
            }

            GridView{
                id: gridView
                Layout.preferredWidth: 645
                height: 452
                leftMargin: 18
                rightMargin: 18
                topMargin: 12
                bottomMargin: 20
                focus: true
                clip: true
                cellHeight:  130
                cellWidth: 203

                displaced: Transition {
                    NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
                }

               model:  DelegateModel {
                    id: visualModel
                    model: yvr.resourcesListModelAdd

                    delegate: DropArea {
                        id: delegateRoot
                        width: gridView.cellWidth
                        height: gridView.cellHeight

                        onEntered: {
                            visualModel.items.move(drag.source.visualIndex, icon.visualIndex)
                            yvr.resourcesListModelAdd.move(drag.source.visualIndex, icon.visualIndex)
                        }

                        property int visualIndex: DelegateModel.itemsIndex
                        Binding { target: icon; property: "visualIndex"; value: visualIndex }

                        YVRResourcesItem{
                            id: icon

                            property int visualIndex: 0

                            DragHandler {
                                id: dragHandler
                            }

                            Drag.active: dragHandler.active
                            Drag.source:  icon
                            Drag.hotSpot.x: 10
                            Drag.hotSpot.y: 10

                            states: [
                                State {
                                    when: icon.Drag.active
                                    ParentChange {
                                        target: icon
                                        parent: gridView
                                    }

                                    AnchorChanges {
                                        target: icon
                                        anchors.horizontalCenter: undefined
                                        anchors.verticalCenter: undefined
                                    }
                                }
                            ]

                            onClickSignal: {
                                if(!hasItem)
                                {
                                    subWindow.source = "qrc:/qml/windows/AddResources.qml"
                                    subWindow.item.isPlayControl = true
                                    subWindow.item.selectItem = yvr.resourcesListModelAdd.getGroupSelect(fileIndex)
                                    mainWin.showSubWin()
                                }else
                                {

                                    subWindow.source = "qrc:/qml/windows/ModifyResources.qml"
                                    subWindow.item.isPlayControl = true
                                    subWindow.item.showNameText = showName
                                    subWindow.item.index = index
                                    subWindow.item.pos = fileIndex
                                    subWindow.item.poster = poster
                                    subWindow.item.file = file
                                    subWindow.item.showNameText = showName
                                    subWindow.item.fileDescText = describe
                                    subWindow.item.type =  typeIndex - 4
                                    subWindow.item.selectItem = yvr.resourcesListModelAdd.getGroupSelect(fileIndex)
                                    subWindow.item.isVideo = typeImageVideo

                                    var pos = mapToItem(root, 0, 0)
                                    menu.x = pos.x + 32
                                    menu.y = pos.y  + 140
                                    menu.pos = fileIndex
                                    menu.isGroup = false
                                    menu.open()
                                }
                            }
                        }
                    }
               }
            }
        }


        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "#141414" }
            GradientStop { position: 0.19; color: "#141414" }

            GradientStop { position: 0.2; color: "#33343D" }
            GradientStop { position: 1.0; color: "#33343D" }
        }

    }


    Menu{
        id: menu
        property int pos: -1
        property bool isGroup: false
        width: 220
        height: 92
        padding: 12

        background: Rectangle{
            color: "#45454B"
            radius: 4
        }

        Repeater{
            model: [ qsTr("编辑"),qsTr("删除")]
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

                        if(menu.isGroup)
                        {
                            if(index == 0)
                            {
                                mainWin.showSubWin()
                            }else
                            {
                                yvr.resourcesListModelAdd.addOrDelGroup(menu.pos, false);
                            }
                        }else
                        {
                            if(index == 0)
                            {
                                mainWin.showSubWin()
                            }else
                            {
                                yvr.resourcesListModelAdd.deletebyIndex(menu.pos);
                            }
                        }

                        menu.close()
                    }
                }
            }
        }
    }
}
