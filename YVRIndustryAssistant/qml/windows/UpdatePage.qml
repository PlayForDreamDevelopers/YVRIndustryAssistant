import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQml 2.0

Rectangle {

    width: 420
    height: contentArea.height
    color: "transparent"
    property string note: ""
    property bool isForce: false

    Rectangle{
        anchors.fill: parent
        radius: 12
        color: utils.yvr_btnbackgroundcolor
        ColumnLayout {
            id:  contentArea
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Text {
                Layout.topMargin: 30
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("自动更新")
                font.family: utils.yvr_font
                color: "#CCCCCD"
                font.pixelSize: 36
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 4
                text: qsTr("您的YVR串流工具需安装如下更新才能使用")
                font.family: utils.yvr_font
                color: "#FF909092"
                font.pixelSize: 16
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 10
                Layout.preferredWidth: 340
                text: note
                font.family: utils.yvr_font
                wrapMode: Text.WrapAnywhere
                color: "#F3F3F3"
                font.pixelSize: 18
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 10
                text: qsTr("此版本为强更，取消后将关闭程序")
                font.family: utils.yvr_font
                color: "red"
                font.pixelSize: 18
                visible: isForce
            }



            RowLayout{
                Layout.topMargin: 16
                Layout.bottomMargin: 18
                Layout.alignment: Qt.AlignHCenter
                spacing: 16

                Rectangle{
                    width: 162
                    height: 54
                    color: "#FF177BE6"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: qsTr("立即安装")
                        font.family: utils.yvr_font
                        color: "#FFFFFF"
                        font.bold: true
                        font.pixelSize: 20
                    }

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {

                            if(yvrupdate.setup())
                            {
                                yvr.shutdownADB()
                                mainWin.hideSubWin()
                                mainWin.close()
                            }
                            else
                            {
                               mainWin.showToast(qsTr("安装失败请重试！"))
                            }
                        }
                    }
                }

                Rectangle{
                    width: 162
                    height: 54
                    color: "#FF292A33"
                    radius: 8
                    Text {
                        font.bold: true
                        anchors.centerIn: parent
                        text: qsTr("取消")
                        font.family: utils.yvr_font
                        color: "#FFFFFF"
                        font.pixelSize: 20
                    }

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            if(isForce)
                            {
                                mainWin.hideSubWin()
                                mainWin.close()
                            }else
                            {
                                mainWin.hideSubWin()
                            }
                        }
                    }
                }
            }
       }
    }
}
