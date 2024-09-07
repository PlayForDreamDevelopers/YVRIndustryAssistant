import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import "../control"

Rectangle {
    id: root
    color: "#22242C"
    property bool hasfinish: false
    RowLayout
    {
        x: 30
        y: 26
        Image {
            Layout.leftMargin: 30
            Layout.preferredHeight: 28
            Layout.preferredWidth: 28
            source: "qrc:/res/image/yvr_back.png"

            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: root.parent.source = ""
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("部署方案-离线部署")
            font.pixelSize: 24
            color: "#FFFFFF"
        }

    }

    ColumnLayout{
        spacing: 0
        width: parent.width
        anchors.centerIn: parent

        Rectangle{
            width: 840
            height: textContent.height + 28
            radius: 16
            color: "#2E313B"
            Layout.topMargin: 58
            Layout.alignment: Qt.AlignHCenter

            ColumnLayout{
                id: textContent
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 24
                Text {
                    Layout.topMargin: 16
                    text: qsTr("方式一：直接将导出资源包拷贝至设备根目录")
                    font.pixelSize: 18
                    color: "#FFFFFF"
                }

                Text {
                    Layout.topMargin: 8
                    text: qsTr("适用于小批量设备部署")
                    font.pixelSize: 14
                    color: "#FFFFFF"
                }

                Text {
                    Layout.topMargin: 8
                    text: qsTr("1、将设备开机，并插入PC上；\n"+
                                "2、将离线导出后的文件夹拷贝至VR设备的根目录下；\n"+
                                "3、拷贝结束，则该设备部署成功；\n"+
                                "4、重启设备；\n"+
                                "5、对其他需要的设备逐一部署；")
                    font.pixelSize: 14
                    lineHeight: 1.2
                    color: "#B2FFFFFF"
                }


                Text {
                    Layout.topMargin: 10
                    text: qsTr("方式二：通过SD卡/U盘部署")
                    font.pixelSize: 18
                    color: "#FFFFFF"
                }
                Text {
                    Layout.topMargin: 8
                    text: qsTr("适用于大批量设备，可借助SD卡或U盘，批量操作部署，节省部署时间")
                    font.pixelSize: 14
                    color: "#FFFFFF"
                }


                Text {
                    Layout.topMargin: 8
                    text: qsTr("1、在VR设备内打开“行业助手”，详见说明书；\n"+
                                "2、将离线导出后的文件夹，拷贝至SD卡或U盘的根目录下；\n"+
                                "3、将SD卡或U盘插入VR设备中；\n"+
                                "4、点击“部署”按钮，等待设备自动执行部署，部署时间根据文件大小不同；\n"+
                                "5、设备中弹出“方案部署成功”，设备将自动重启；\n"+
                                "6、对其他需要的设备逐一部署；")
                    font.pixelSize: 14
                    color: "#B2FFFFFF"
                    lineHeight: 1.2
                }


                Text {
                    Layout.topMargin: 10
                    text: qsTr("离线部署：将目前现有的配置内容，统一下载至本地路径，然后将下载内容拷贝至VR设备目录下。")
                    font.pixelSize: 14
                    color: "#F69422"
                }
            }

            MouseArea{
                anchors.fill: parent
                propagateComposedEvents: false
            }
        }

        YVRButton{
            Layout.topMargin: 34
            Layout.alignment: Qt.AlignHCenter
            width: 276
            height: 54
            visible: !hasfinish
            color: "#3476FB"
            showText: "离线导出资源"
            onClickSignal: {
                yvr.deploymentPlan()
                hasfinish = true
            }
        }

        Text {
            visible: hasfinish
            Layout.topMargin: 24
            Layout.alignment: Qt.AlignHCenter
            text: yvr.fileSavePath
            font.pixelSize: 18
            color: "#979797"
        }

        Text {
            visible: hasfinish
            Layout.topMargin: 34
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("离线资源导出完毕,点击可打开文件夹")
            font.pixelSize: 18
            color: "#FFFFFF"
            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: yvr.gotoFolder(yvr.fileSavePath)
            }
        }
    }
}
