import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQuick.Dialogs 1.3
import "../control"

Rectangle {
    id: win
    width: 800
    height: 506
    visible: true
    color: "transparent"

    signal acceptImport()

    Rectangle{
        anchors.fill: parent
        color: "#45454B"
        radius: 16
        RowLayout{
            width: parent.width
            height: 44
            id: titleBar
            Item {
                Layout.fillWidth: true
            }

            Image{
                Layout.topMargin: 10
                Layout.rightMargin: 24
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

        ColumnLayout{
            anchors.top: titleBar.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 40

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("添加设备")
                font.bold: true
                color: "#FFFFFF"
                font.pixelSize: 24
            }

            Rectangle{
                width: 560
                height: 274
                color: "#4D4D54"
                radius: 8
                ColumnLayout{
                    spacing: 24
                    anchors.horizontalCenter: parent.horizontalCenter
                    Text {
                        Layout.topMargin: 20
                        font.pixelSize: 18
                        wrapMode: Text.WrapAnywhere
                        Layout.maximumWidth: 512
                        color: "#DBDBDB"
                        text: qsTr("一、点击按钮【获取表格示例】，下载SN填写示例表格；\n" +
                                    "二、将您需要录入的设备SN按表格示例填写到示例表格中；\n" +
                                    "1.第一列为设备ID：用户可对设备进行编号\n" +
                                    "2.第二列为设备SN，用户输入完整的设备SN；SN可以在头盔的“设置-通用”下查看“设备序列号”；\n" +
                                    "三、点击【导入设备】，将修改后的设备表格上传导入")
                    }

                    YVRButton{
                        width: 276
                        Layout.alignment: Qt.AlignHCenter
                        color: "transparent"
                        textColor: "#3476FB"
                        showText: qsTr("获取表格示例")
                        onClickSignal: {
                            yvr.showDeviceExampleExcel()
                        }
                    }
                }
            }

            YVRButton{
                width: 276
                showText: qsTr("导入设备")
                Layout.alignment: Qt.AlignHCenter

                onClickSignal: {
                    filedialog.open()
                }
            }
        }
    }

    FileDialog{
        id: filedialog
        title: "PFDM"
        //folder: "file:///D:/video/"
        selectFolder: false
        selectMultiple: false

        nameFilters: "Csv files(*.csv)"
        onAccepted: {
            yvr.loadDevicesExcel(filedialog.fileUrl)
            mainWin.hideSubWin()
            acceptImport()
        }
    }
}
