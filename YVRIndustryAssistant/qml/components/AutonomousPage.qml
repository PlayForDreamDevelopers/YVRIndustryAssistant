import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtQml.Models 2.1
import QtQuick.Controls 2.14
import QtQml 2.0
import YVRManager 1.0
import "../control"

Item {
    ColumnLayout{
        width: parent.width
        spacing: 0
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            font.pixelSize: 24
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("添加无人值守方案")
            color: "#FFFFFF"
        }

        Rectangle{
            width: 1042
            height: 500
            radius: 16
            color: "#22242C"

            Layout.topMargin: 16
            Layout.alignment: Qt.AlignHCenter
            ColumnLayout{
                spacing: 0
                anchors.fill: parent

                Rectangle{
                    width: parent.width
                    height: 60
                    radius: 16
                    color: "#2A2C35"

                    RowLayout{
                        spacing: 10
                        width: parent.width
                        anchors.centerIn: parent
                        Text {
                            font.pixelSize: 16
                            Layout.leftMargin: 16
                            text: qsTr("灭屏后再次亮屏")
                            color: "#D8D8D8"
                        }

                        ComboBox {
                            id: control
                            Layout.preferredWidth: 143
                            Layout.preferredHeight: 30
                            model: [qsTr("视频继续播放"),qsTr("视频重新播放")]

                            delegate: ItemDelegate {
                                width: 131
                                height: 30
                                background: Rectangle{
                                    color: parent.highlighted ?  "#40414F" : "#33343D"
                                    radius: 8
                                    anchors.fill: parent
                                }

                                contentItem: Text {
                                    text: modelData
                                    color: parent.highlighted ? "#FFFFFF" : "#99FFFFFF"
                                    font.pixelSize: 14
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }
                                highlighted: control.highlightedIndex === index
                            }

                            indicator:Image {
                                    anchors.right: parent.right
                                    anchors.rightMargin: 16
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 16
                                    height: 16
                                    source: "qrc:/res/image/yvr_arrow_down.png"
                                }

                            contentItem: Text {
                                leftPadding: 16
                                rightPadding: control.indicator.width + control.spacing

                                text: control.displayText
                                font.pixelSize: 14
                                color: "#FFFFFF"
                                verticalAlignment: Text.AlignVCenter
                            }

                            background: Rectangle {
                                implicitWidth: 143
                                implicitHeight: 30
                                color: "#41424B"
                                radius: 6
                            }

                            popup: Popup {
                                y: control.height + 5
                                width: control.width
                                implicitHeight: contentItem.implicitHeight + 12
                                padding: 6

                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight + 2
                                    model: control.popup.visible ? control.delegateModel : null
                                    currentIndex: control.highlightedIndex
                                }

                                background: Rectangle {
                                    color: "#33343D"
                                    radius: 8
                                }
                            }
                        }

                        Item{
                            Layout.fillWidth: true
                        }

                        Rectangle{
                            width: 120
                            height: 30
                            radius: 6
                            color: "#383C47"
                            Layout.rightMargin: 16
                            Text {
                                anchors.centerIn: parent
                                font.pixelSize: 14
                                color: "#D8D8D8"
                                text: qsTr("+   添加视频")
                            }

                            MouseArea{
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    subWindow.source = "qrc:/qml/windows/AddResources.qml"
                                    subWindow.item.isVideo = true
                                    subWindow.item.isVideoOnly = true
                                    subWindow.item.isPlayControl = false

                                    mainWin.showSubWin()
                                }
                            }
                        }
                    }
                }

                RowLayout{
                    spacing: 30
                    Layout.topMargin: 36
                    Text {
                        Layout.leftMargin: 50
                        Layout.preferredWidth: 80
                        leftPadding: 16
                        text: qsTr("序号")
                        font.pixelSize: 14
                        color: "#FFFFFF"
                    }
                    Text {
                        Layout.preferredWidth: 189
                        leftPadding: 16
                        text: qsTr("视频名称")
                        color: "#FFFFFF"
                        font.pixelSize: 14
                    }
                    Text {
                        leftPadding: 16
                        Layout.preferredWidth: 223
                        text: qsTr("视频描述")
                        font.pixelSize: 14
                        color: "#FFFFFF"
                    }
                    Text {
                        Layout.preferredWidth: 100
                        leftPadding: 16
                        text: qsTr("视频格式")
                        color: "#FFFFFF"
                        font.pixelSize: 14
                    }
                    Text {
                        Layout.preferredWidth: 100
                        leftPadding: 16
                        text: qsTr("视频大小")
                        color: "#FFFFFF"
                        font.pixelSize: 14
                    }
                    Text {
                        Layout.preferredWidth:100
                        leftPadding: 16
                        color: "#FFFFFF"
                        text: qsTr("操作")
                        font.pixelSize: 14
                    }
                }

                ListView{
                    id: listView
                    Layout.preferredWidth: 1042
                    Layout.fillHeight: true
                    leftMargin: 50
                    rightMargin: 50
                    Layout.topMargin: 20
                    bottomMargin: 20
                    focus: true
                    clip: true

                    displaced: Transition {
                        NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
                    }

                    model: DelegateModel {
                        id: visualModel
                        model: yvr.resourcesListModelAuto

                        delegate: DropArea {
                            id: delegateRoot
                            visible: hasItem

                            width: 942
                            height: 55

                            onEntered: {
                                visualModel.items.move(drag.source.visualIndex, icon.visualIndex)
                                yvr.resourcesListModelAuto.move(drag.source.visualIndex, icon.visualIndex)
                            }

                            property int visualIndex: DelegateModel.itemsIndex
                            Binding { target: icon; property: "visualIndex"; value: visualIndex }

                            Rectangle{
                                id: icon
                                anchors {
                                    horizontalCenter: delegateRoot.horizontalCenter;
                                    verticalCenter: delegateRoot.verticalCenter
                                }
                                property int visualIndex: 0
                                width: 942
                                height: 55
                                radius: 10
                                color: "transparent"

                                RowLayout{
                                    spacing: 30
                                    Text {
                                        Layout.preferredWidth: 80
                                        leftPadding: 16
                                        elide: Text.ElideRight
                                        text: index + 1
                                        font.pixelSize: 14
                                        color: "#FFFFFF"
                                    }

                                    Text {
                                        Layout.preferredWidth: 189
                                        leftPadding: 16
                                        elide: Text.ElideRight
                                        text: showName
                                        color: "#FFFFFF"
                                        font.pixelSize: 14
                                    }

                                    Text {
                                        Layout.preferredWidth: 223
                                        leftPadding: 16
                                        elide: Text.ElideRight
                                        text: describe
                                        font.pixelSize: 14
                                        color: "#FFFFFF"
                                    }

                                    Text {
                                        Layout.preferredWidth: 100
                                        leftPadding: 16
                                        elide: Text.ElideRight
                                        text: type23D
                                        color: "#FFFFFF"
                                        font.pixelSize: 14
                                    }

                                    Text {
                                        Layout.preferredWidth: 100
                                        leftPadding: 16
                                        elide: Text.ElideRight
                                        text: fileSize
                                        color: "#FFFFFF"
                                        font.pixelSize: 14
                                    }

                                    RowLayout {
                                        spacing: 12
                                        Text {
                                            Layout.leftMargin: 16
                                            text: qsTr("编辑")
                                            color: "#99FFFFFF"
                                            font.pixelSize: 14
                                            MouseArea{
                                                anchors.fill: parent
                                                cursorShape: Qt.PointingHandCursor
                                                onClicked: {
                                                    subWindow.source = "qrc:/qml/windows/ModifyResources.qml"
                                                    subWindow.item.isVideo = true
                                                    subWindow.item.isPlayControl = false
                                                    subWindow.item.index = index
                                                    subWindow.item.poster = poster
                                                    subWindow.item.file = file
                                                    subWindow.item.showNameText = showName
                                                    subWindow.item.fileDescText = describe
                                                    subWindow.item.type =  typeIndex - 4

                                                    mainWin.showSubWin()
                                                }
                                            }
                                        }

                                        Text {
                                            Layout.rightMargin: 16
                                            text: qsTr("删除")
                                            color: "#99FFFFFF"
                                            font.pixelSize: 14
                                            MouseArea{
                                                cursorShape: Qt.PointingHandCursor
                                                anchors.fill: parent
                                                onClicked: {

                                                    subWindow.source = "qrc:/qml/windows/TipsPage.qml"
                                                    subWindow.item.ensure.connect(function(){
                                                            yvr.resourcesListModelAuto.deletebyIndex(index)
                                                    })
                                                    mainWin.showSubWin()
                                                }
                                            }
                                        }
                                    }
                                }

                                DragHandler {
                                    id: dragHandler
                                }

                                Drag.active: dragHandler.active
                                Drag.source: icon
                                Drag.hotSpot.x: 10
                                Drag.hotSpot.y: 10

                                states: [
                                    State {
                                        when: icon.Drag.active
                                        ParentChange {
                                            target: icon
                                            parent: listView
                                        }

                                        AnchorChanges {
                                            target: icon
                                            anchors.horizontalCenter: undefined
                                            anchors.verticalCenter: undefined
                                        }
                                    }
                                ]
                            }
                        }
                    }
                }
            }
        }

        YVRButton{
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 24
            showText: qsTr("部署方案")
            onClickSignal: {
                yvr.setPlayControl(true, control.currentIndex)
                mainWin.showPage(YVRManagerCLS.DeploymentPlan)
            }
        }
   }
}
