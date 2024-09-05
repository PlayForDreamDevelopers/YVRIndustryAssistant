import QtQuick 2.14
import QtQml 2.0

Rectangle {
    id: control
    property string tipText: qsTr("请输入")
    property string inputText: input.text
    property string defaultText: ""
    property bool inputEdit: true
    property int textPixel: 20
    radius: 8
    width: 560
    height: 54
    color: "#33343D"
    Text {
        anchors.fill: parent
        visible: !input.activeFocus  && input.text === ""
        elide: Text.ElideRight
        text: tipText
        leftPadding:16
        topPadding: 12
        font.pixelSize: textPixel
        color: "#99FFFFFF"
    }

    TextInput{
        id: input
        enabled: inputEdit
        anchors.fill: parent
        wrapMode: Text.WrapAnywhere
        leftPadding:16
        rightPadding: 16
        topPadding: 12
        font.pixelSize: textPixel
        color: "#FFFFFF"
        selectByMouse: true
        text: defaultText
    }

    function clearInput(){
        input.clear()
    }
}
