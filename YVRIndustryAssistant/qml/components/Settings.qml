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
            text: qsTr("设置")
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

        Rectangle{
            width: 609
            height: 142
            color: "#45454B"
            radius: 16
            Layout.topMargin: 51
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 44
                anchors.top: parent.top
                anchors.topMargin: 44
                text: qsTr("受控模式")
                font.pixelSize: 20
                font.bold: true
                color: "#DBDBDB"
            }

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 44
                anchors.top: parent.top
                anchors.topMargin: 76
                text: qsTr("开启后，所有的VR设备将处于受控状态。")
                font.pixelSize: 14
                color: "#DBDBDB"
            }

            Rectangle{
                anchors.right: parent.right
                anchors.rightMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                width: 40
                height: 20
                radius: 20
                color:  yvr.control ? "#3476FB" : "#000000"

                Rectangle{
                    width: 16
                    height: 16
                    radius: 16
                    anchors.left: parent.left
                    anchors.leftMargin: yvr.control ? 20 : 4
                    anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        yvr.control = !yvr.control
                    }
                }
            }
        }
  }
}
