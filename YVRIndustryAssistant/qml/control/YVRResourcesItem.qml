import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.2

Rectangle{
    width: 203
    height: 130
    anchors {
        horizontalCenter: parent.horizontalCenter;
        verticalCenter: parent.verticalCenter
    }
    radius: 10
    color: mouseArea.containsMouse ? "#606171" : "transparent"

    signal clickSignal()

    MouseArea{
        id: mouseArea
        cursorShape: Qt.PointingHandCursor
        anchors.fill: parent
        hoverEnabled: true

        onClicked: clickSignal()
    }

    ColumnLayout{
        anchors.centerIn: parent
        Image {
            Layout.preferredHeight: 118
            Layout.preferredWidth: 191
            source: poster
            fillMode: Image.PreserveAspectCrop
        }
    }

    ColumnLayout{
        spacing: 10
        visible: hasItem
        Text {
            Layout.leftMargin: 12
            Layout.topMargin: 12
            color: "#FFFFFF"
            text: showName
            font.bold: true
            Layout.maximumWidth: 180
            elide: Text.ElideRight
            font.pixelSize: 14
        }

        RowLayout{
            spacing: 6
            Layout.leftMargin: 12

            YVRButton {
                width: 36
                height: 20
                radius: 6
                color: "#F69422"
                showText: type23D
                textPixel: 12
            }

            YVRButton {
                width: 36
                height: 20
                radius: 6
                color: "#3476FB"
                showText: typeImageVideo ?  qsTr("视频") : qsTr("图像")
                textPixel: 12
            }
        }
    }
}
