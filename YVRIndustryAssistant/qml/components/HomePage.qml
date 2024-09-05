import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import "./../control"
import YVRManager 1.0

Item {

    ColumnLayout{
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0

        Text {
            Layout.topMargin: 25
            text: qsTr("方案管理")
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

        RowLayout{
            Layout.topMargin: 60
            Layout.alignment: Qt.AlignHCenter
            spacing: 20
            Button{
                Layout.preferredWidth: 280
                Layout.preferredHeight: 410
                background: Rectangle {
                                radius: 16
                                color: "transparent"
                            }

                contentItem: Image {
                    source: "qrc:/res/image/yvr_play_control.png"

                    YVRButton{
                        visible: yvr.resourcesListModelAdd.init
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 100
                        anchors.horizontalCenter: parent.horizontalCenter
                        showText: qsTr("修改方案")
                        textPixel: 14
                        textColor: "#FFFFFF"
                        color: "#3476FB"
                        width: 160
                        radius: 4
                        height: 26

                        onClickSignal: {
                             createOrDeployment.source = "qrc:/qml/components/ResourcesAddPage.qml"
                        }
                    }

                    YVRButton{
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 54
                        anchors.horizontalCenter: parent.horizontalCenter
                        showText: yvr.resourcesListModelAdd.init ? qsTr("部署") : qsTr("新建方案")
                        radius: 4
                        textPixel: 14
                        textColor: "#FFFFFF"
                        color: "#3476FB"
                        width: 160
                        height: 26

                        onClickSignal: {
                            if(yvr.resourcesListModelAdd.init)
                            {
                                createOrDeployment.source = "qrc:/qml/components/DeploymentPlan.qml"
                            }else
                            {
                                yvr.resourcesListModelAdd.name = qsTr("播控方案01")
                                yvr.resourcesListModelAdd.content = qsTr("播控方案介绍")
                                createOrDeployment.source = "qrc:/qml/components/ResourcesAddPage.qml"
                            }
                        }
                    }
                }

                onHoveredChanged: {
                    if(hovered)
                    {
                       scale = 0.95
                    }else
                    {
                        scale = 1.0
                    }
                }

                onClicked: {
                    mainWin.showPage(yvr.firstPlayControl() ?  YVRManagerCLS.AddPlayControl : YVRManagerCLS.PlayControl)
                }

                Behavior on scale { PropertyAnimation {duration: 50} }

                hoverEnabled: true
            }

            Button{
                visible: false
                Layout.preferredWidth: 280
                Layout.preferredHeight: 410
                background: Rectangle {
                    radius: 16
                    color: "transparent"
                }

                contentItem: Image {
                    source: "qrc:/res/image/yvr_none_control.png"
                }

                onHoveredChanged: {
                    if(hovered)
                    {
                        scale = 0.95
                    }else
                    {
                        scale = 1.0
                    }
                }

                onClicked: {
                    mainWin.showPage(YVRManagerCLS.Autonomous)
                }

                Behavior on scale { PropertyAnimation {duration: 50} }

                hoverEnabled: true
            }
        }
    }

    Loader{
        id: createOrDeployment
        anchors.fill: parent
    }
}
