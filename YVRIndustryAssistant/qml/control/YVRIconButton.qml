import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.2

Rectangle {
    property string showText: qsTr("确定")
    property string showIcon: "qrc:/res/image/yvr_delete.png"
    signal clickSignal()
    radius: 4
    width: 80
    height: 34
    color: "#33343D"

    RowLayout
    {
        spacing: 8
        anchors.centerIn: parent
        Image {
            Layout.preferredHeight: 20
            Layout.preferredWidth: 20
            source: showIcon
        }

        Text {
            text: showText
            font.pixelSize: 14
            color: "#FFFFFF"
        }
    }




    MouseArea{
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: clickSignal()
    }
}
