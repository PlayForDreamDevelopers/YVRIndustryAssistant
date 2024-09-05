import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.2

Rectangle {
    property string showText: qsTr("确定")
    property bool selected: false
    property string showIcon: "qrc:/res/image/yvr_select.png"
    signal clickSignal()
    radius: 8
    width: 64
    height: 64
    color: clickArea.containsMouse || selected ? "#2E2E2E" : "transparent"

    ColumnLayout
    {
        spacing: 0
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 6
            Layout.preferredHeight: 32
            Layout.preferredWidth: 32
            source: showIcon
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: showText
            font.pixelSize: 14
            color: "#FFFFFF"
        }
    }




    MouseArea{
        id: clickArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: clickSignal()
        hoverEnabled: true
    }
}
