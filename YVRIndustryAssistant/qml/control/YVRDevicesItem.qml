import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.2

Rectangle {
    signal clickSignal()

    property string vrItemId: "001"
    property string vrItemGroup: "默认"
    property bool online: false
    property string vrItemSn: "PA7L10MGG6040024W"
    property var deviceStatus: ["qrc:/res/image/yvr_idle.png","qrc:/res/image/yvr_control.png"]
    property int power: 20
    property bool showScreen: false
    property bool selected: false
    property bool selectAll: false
    property bool selectedMode: false

    width: 358
    height: 213
    color: "#33343D"
    radius: 8

    ColumnLayout {
        spacing: 0
        width:parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        RowLayout {
            Layout.topMargin: 24
            Layout.leftMargin: 24
            Layout.preferredHeight: 33
            height: 33
            width: 313
            spacing: 12

            Text {
                Layout.preferredWidth: 60
                font.pixelSize: 28
                font.bold: true
                color: "#D8D8D8"
                text: vrItemId
                font.letterSpacing: 2
            }

            Image {
                Layout.preferredHeight: 14
                Layout.preferredWidth: 14
                source: online ? "qrc:/res/image/yvr_online.png": "qrc:/res/image/yvr_offline.png"
            }

            Rectangle{
                visible: online
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                width: 1
                height: 20
                color: "#7F7F7F"
            }

            Repeater{
                model: deviceStatus
                delegate: Image {
                    Layout.preferredHeight: 14
                    Layout.preferredWidth: 14
                    source: modelData
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Rectangle{
                visible: online
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                width: 1
                height: 20
                color: "#7F7F7F"
            }


            Rectangle
            {
                visible: online
                Layout.rightMargin: 20
                width: 30
                height: 16
                border.width: 2
                border.color: "#D9D9D9"
                color: "#000000"
                radius: 4

                Rectangle
                {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 3
                    width: power * 24 / 100
                    height: 10
                    color: "#26D941"
                }

                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 8
                    font.bold: true
                    color: "#FFFFFF"
                    text: power
                }

                Rectangle{
                    width: 2
                    height: 5
                    color: "#D9D9D9"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.right
                }
            }

        }

        Text {
            Layout.topMargin: 18
            Layout.leftMargin: 24
            text: qsTr("SN: ") + vrItemSn
            font.pixelSize: 16
            color: "#9C9C9C"
        }

        RowLayout{
            Layout.leftMargin: 24
            Layout.topMargin: 62

            Rectangle{
                width: 20
                height: 20
                visible: selectedMode
                Layout.leftMargin: 10
                color: "transparent"
                radius: 4
                border.width: 2
                border.color: "#B2B2B5"

                Image{
                    id: selectImage
                    anchors.fill: parent
                    visible: selected || selectAll
                    source: "qrc:/res/image/yvr_select.png"
                }

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        yvr.vrListModel.select(vrSN, !selected)
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }


            Rectangle
            {
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                color: "#4C4C4C"
                radius: 32
                visible: false

                Image {
                    anchors.fill: parent
                    source: "qrc:/res/image/yvr_preview_btn.png"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                        }
                    }
                }
            }

            Image {
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.rightMargin: 20
                source: "qrc:/res/image/yvr_more_btn.png"
                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        clickSignal()
                    }
                }
            }
        }
    }
}
