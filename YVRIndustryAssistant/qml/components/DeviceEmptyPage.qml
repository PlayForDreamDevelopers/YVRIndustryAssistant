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
            text: qsTr("添加设备")
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
            Layout.topMargin: 84
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
                        dicoverDevice.source = "qrc:/qml/components/DeviceDiscoverPage.qml"
                    }
                }
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 38
                text: qsTr("请先添加设备")
                font.pixelSize: 16
                font.bold: true
                color: "#FFFFFF"
            }

            Text {
                Layout.topMargin: 18
                text: qsTr("添加设备前，请完成以下3步操作：\n1.前往VR设备【设置】-【网络】，将VR设备与电\n脑连接在同一局域网下：\n2.在【资源库】打开播控应用\n3.在播控应用主界面，点击设置。打开接受控制端\n控制开关。完成以上3步。")
                font.pixelSize: 16
                font.bold: true
                color: "#C1C1C1"
            }
        }
    }

    Loader{
        id: dicoverDevice
        anchors.fill: parent
    }
}
