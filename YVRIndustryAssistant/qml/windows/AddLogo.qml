import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQuick.Dialogs 1.3
import "../control"


Rectangle {
    width: 301
    height: 326
    visible: true
    color: "transparent"
    property string file: ""
    property var selectItem: [false, false, false, false, false]

    Rectangle{
        anchors.fill: parent
        color: "#22242C"
        radius: 16

        Image{
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            width:24
            height: 24
            source: "qrc:/res/image/yvr_close.png"
            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: mainWin.hideSubWin()
            }
        }



        ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Text {
                Layout.topMargin: 34
                Layout.alignment: Qt.AlignHCenter
                text: yvr.resourcesListModelAdd.logo === "" ? qsTr("添加 LOGO"): qsTr("更改 LOGO")
                font.bold: true
                color: "#FFFFFF"
                font.pixelSize: 24
            }


            Image {
                Layout.topMargin: 34
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 84
                Layout.preferredWidth: 227
                source: file === "" ?  yvr.resourcesListModelAdd.logo === "" ? "qrc:/res/image/yvr_logo_add.png" : yvr.resourcesListModelAdd.logo :file

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        filedialog.open()
                    }
                }
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 12
                text: qsTr("请上传322*126px，png/jpg格式LOGO")
                font.pixelSize: 10
                font.bold: true
                color: "#DBDBDB"
            }

            YVRButton{
                Layout.topMargin: 40
                width: 220
                height: 36
                showText: qsTr("上传")
                Layout.alignment: Qt.AlignHCenter
                onClickSignal: {

                    yvr.resourcesListModelAdd.logo = file
                    mainWin.hideSubWin()
                }
            }
        }
    }

    FileDialog{
        id: filedialog
        selectExisting: true
        title: "PFDM"
        //folder: yvr.defaultPath()
        selectFolder: false
        selectMultiple: false
        nameFilters: ["Image files(*.jpg *.png)"]
        onAccepted: {
            file = filedialog.fileUrl
        }
    }
}
