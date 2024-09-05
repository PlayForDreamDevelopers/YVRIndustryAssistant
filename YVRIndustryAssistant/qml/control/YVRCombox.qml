import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.1

Rectangle {
    id: control
    property string curText: qsTr("全部分组")
    property int currentIndex: -1
    property var controlModel: []
    width: 560
    height: 54
    color: "#33343D"
    radius: 8

    signal selectItem(var data)


    Text {
        id: combox
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.verticalCenter: parent.verticalCenter
        text: curText
        font.pixelSize: 20
        color: "#FFFFFF"
    }

    Image {
        anchors.right: parent.right
        anchors.rightMargin: 18
        anchors.verticalCenter: parent.verticalCenter
        width: 24
        height: 24
        source: "qrc:/res/image/yvr_arrow_down.png"
    }


    Menu{
        id: menu
        width: 560
        padding: 12
        y: control.height + 2
        background: Rectangle{
            anchors.fill: parent
            color: "#33343D"
            radius: 8
        }

        Repeater{
            model: controlModel
            delegate: MenuItem {
                    width: 536
                    height: 34
                    background: Rectangle{
                        anchors.centerIn: parent
                        radius: 4
                        anchors.fill: parent
                        color: cellArea.containsMouse ? "#177BE6" : "transparent"
                    }

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.verticalCenter: parent.verticalCenter
                        text: modelData
                        font.family: utils.yvr_font
                        color: "#FFFFFF"
                        font.pixelSize: 20
                    }

                    MouseArea{
                        id: cellArea
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onClicked: {
                            curText = modelData
                            currentIndex = index
                            menu.close()
                        }
                    }
                }
        }
    }

//    Popup {
//        id: comboxItem
//        width: parent.width
//        padding: 12
//        y: control.height + 2
//        background: Rectangle{
//            anchors.fill: parent
//            color: "#33343D"
//            radius: 8
//        }

//        closePolicy: Popup.CloseOnEscape
//        ColumnLayout{
//            anchors.horizontalCenter: parent.horizontalCenter
//            Repeater{
//                model: controlModel
//                delegate: Rectangle{
//                    width: control.width - 24
//                    height: 34
//                    radius: 8
//                    color: btnArea.containsMouse ? "#40414F" : "transparent"
//                    Text {
//                        anchors.verticalCenter: parent.verticalCenter
//                        anchors.left: parent.left
//                        anchors.leftMargin: 18
//                        text: modelData
//                        font.family: utils.yvr_font
//                        color: "#FFFFFF"
//                        font.pixelSize: 20
//                    }


//                    MouseArea{
//                        id: btnArea
//                        anchors.fill: parent
//                        cursorShape: Qt.PointingHandCursor
//                        hoverEnabled: true
//                        onClicked: {
//                            curText = modelData
//                            currentIndex = index
//                            comboxItem.close()
//                            selectItem(modelData)
//                        }
//                    }
//                }
//            }
//        }
//    }

    MouseArea{
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            if(menu.visible)
                menu.close()
             else
                menu.open()
        }
    }

}
