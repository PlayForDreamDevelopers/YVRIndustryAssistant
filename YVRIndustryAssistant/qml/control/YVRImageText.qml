import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.2

Item {
    property string showText: qsTr("在线")
    property string showIcon: "qrc:/res/image/yvr_offline.png"
    width: content.width
    height: 20

    RowLayout{
        id: content
        Image {
            Layout.preferredHeight: 14
            Layout.preferredWidth: 14
            source: showIcon
        }

        Text {
            font.pixelSize: 12
            color: "#FFFFFF"
            text: showText
        }
    }
}
