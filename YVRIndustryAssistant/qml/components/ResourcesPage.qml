import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtQml 2.0
import YVRManager 1.0
import "./../control"

Item {

    ColumnLayout{
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0
        visible:  ! gridView.visible

        Text {
            Layout.topMargin: 25
            text: qsTr("播控")
            font.pixelSize: 24
            font.bold: true
            color: "#FFFFFF"
        }

        Rectangle{
            Layout.topMargin: 16
            width: 1128
            height: 1
            color: "#2B2E38"
        }

        ColumnLayout {
            Layout.topMargin: 120
            Layout.alignment: Qt.AlignHCenter
            spacing: 0
            Image {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 127
                Layout.preferredWidth: 127
                source: "qrc:/res/image/yvr_import_devices.png"

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                       mainWin.showPage(YVRManagerCLS.PlanManager)
                    }
                }
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 38
                text: qsTr("请先添加播控方案")
                font.pixelSize: 16
                font.bold: true
                color: "#FFFFFF"
            }

            Text {
                Layout.topMargin: 38
                text: qsTr("当前没有播控方案，请先添加播控方案")
                font.pixelSize: 16
                font.bold: true
                color: "#C1C1C1"
            }
        }
    }

    GridView{
        id: gridView
        visible: yvr.resourcesListModel.rowCount() !== 0
        anchors.fill: parent
        leftMargin: 12
        rightMargin: 12
        topMargin: 12
        bottomMargin: 20
        focus: true
        clip: true
        cellHeight:  315
        cellWidth: 382
        model:  yvr.resourcesListModel

        delegate: Rectangle{
            width: gridView.cellWidth
            height: gridView.cellHeight
            radius: 10
            color: mouseArea.containsMouse ? "#606171" : "transparent"

            MouseArea{
                id: mouseArea
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                hoverEnabled: true
            }

            ColumnLayout{
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                Image {
                    Layout.topMargin: 20
                    Layout.preferredHeight: 200
                    Layout.preferredWidth: 358
                    source: poster
                    fillMode: Image.PreserveAspectCrop
                }
            }

            ColumnLayout{
                spacing: 10
                height: parent.height

                Text {
                    Layout.leftMargin: 24
                    Layout.topMargin: 24
                    color: "#FFFFFF"
                    text: showName
                    font.bold: true
                    Layout.maximumWidth: 330
                    elide: Text.ElideRight
                    font.pixelSize: 20
                }

                RowLayout{
                    spacing: 6
                    Layout.leftMargin: 24

                    YVRButton {
                        width: 36
                        height: 20
                        radius: 6
                        color: "#F69422"
                        showText: type23D
                        textPixel: 12
                    }

                    YVRButton {
                        width: 36
                        height: 20
                        radius: 6
                        color: "#3476FB"
                        showText: typeImageVideo ?  qsTr("视频") : qsTr("图像")
                        textPixel: 12
                    }
                }

                Item {
                    Layout.fillHeight: true
                }

                Rectangle{
                    Layout.leftMargin: 12
                    Layout.bottomMargin: 12
                    width: 358
                    height: 80
                    radius: 8

                    RowLayout{
                        spacing: 0
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter

                        ColumnLayout{
                            Layout.leftMargin: 30
                            RowLayout{
                                Rectangle
                                {
                                    width: 10
                                    height: 10
                                    radius: 10
                                    color:  releaseBtn.visible ? "red" : "#00E38E"

                                }

                                Text {
                                    text: releaseBtn.visible ?  qsTr("待发布") :  qsTr("已发布")
                                    font.pixelSize: 12
                                    color: "#D8D8D8"
                                }
                            }

                            Text {
                                visible: !releaseBtn.visible
                                text: qsTr("发布时间：")  + playTime
                                font.pixelSize: 12
                                color: "#D8D8D8"
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }


                        Image {
                            visible: !releaseBtn.visible
                            Layout.preferredHeight: 44
                            Layout.preferredWidth: 44
                            source: "qrc:/res/image/yvr_create_plan.png"

                            MouseArea{
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    subWindow.source = "qrc:/qml/windows/PublishPage.qml"
                                    subWindow.item.showTitle = qsTr("编辑播控")
                                    subWindow.item.videoIndex = fileIndex
                                    subWindow.item.vrList = yvr.vrListModel.getPlayVRList(fileIndex)
                                    mainWin.showSubWin()
                                }
                            }
                        }

                        Image {
                            visible: !releaseBtn.visible
                            Layout.leftMargin: 20
                            Layout.rightMargin: 30
                            Layout.preferredHeight: 44
                            Layout.preferredWidth: 44
                            source: "qrc:/res/image/yvr_delete_plan.png"

                            MouseArea{
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    subWindow.source = "qrc:/qml/windows/Reconfirm.qml"
                                    subWindow.item.showContent = qsTr("取消后，可重新发布")
                                    subWindow.item.showTitle = qsTr("取消发布")
                                    subWindow.item.showImage = "qrc:/res/image/yvr_tips_cancel.png"
                                    subWindow.item.ensure.connect(function(){
                                        yvr.pause(fileIndex)
                                    })

                                    mainWin.showSubWin()
                                }
                            }
                        }

                        YVRIconButton {
                            id: releaseBtn
                            visible: playTime === ""
                            Layout.rightMargin: 30
                            color: "#3476FB"
                            showText: qsTr("发布")
                            showIcon: "qrc:/res/image/yvr_release.png"
                            width: 80
                            height: 30
                            radius: 8

                            onClickSignal: {

                                var list = yvr.vrListModel.getPlayVRList(fileIndex);

                                if(list.rowCount() !== 0)
                                {
                                    subWindow.source = "qrc:/qml/windows/PublishPage.qml"
                                    subWindow.item.showTitle = qsTr("创建播控")
                                    subWindow.item.videoIndex = fileIndex
                                    subWindow.item.isVideo = typeImageVideo
                                    subWindow.item.vrList = list
                                    mainWin.showSubWin()

                                }else
                                {
                                    subWindow.source = "qrc:/qml/windows/Reconfirm.qml"
                                    subWindow.item.showContent = qsTr("暂无可用设备")
                                    subWindow.item.showTitle = qsTr("无法创建")
                                    subWindow.item.showImage = "qrc:/res/image/yvr_tips_glass.png"
                                    mainWin.showSubWin()
                                }

                            }

                        }
                    }

                    gradient: Gradient {
                        orientation: Gradient.Vertical
                        GradientStop { position: 0.0; color: "#20000000" }
                        GradientStop { position: 1.0; color: "#000000" }
                    }
                }
            }
        }
    }

}
