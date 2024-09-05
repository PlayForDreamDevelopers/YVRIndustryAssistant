import QtQuick 2.14
import QtQml 2.0

Rectangle {
    property string showText: qsTr("确定")
    property int textPixel: 20
    property string textColor: "#FFFFFF"

    signal clickSignal()
    radius: 8
    width: 172
    height: 54
    color: "#1E68FF"
    Text {
        anchors.centerIn: parent
        text: showText
        font.pixelSize: textPixel
        color: textColor
    }

    MouseArea{
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: clickSignal()
    }
}
