import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQuick.Dialogs 1.3
import "../control"


Rectangle {

    width: 680
    height: mainspace.height + 102
    color: "transparent"

    property bool isVideo: true
    property bool isPlayControl: true
    property string file: isVideo ? qsTr("请上传视频文件") : qsTr("请上传图片文件")
    property string poster: isVideo ? qsTr("请上传视频海报") : qsTr("请上传图片海报")
    property string  showNameText: "111"
    property string  fileDescText: "222"
    property var selectIndex: 0
    property int type: 0
    property int index: 0
    property int pos: 0

    Rectangle{
        anchors.fill: parent
        color: "#22242C"
        radius: 16
        RowLayout{
            width: parent.width
            height: 42
            id: titleBar
            Text {
                Layout.leftMargin: 268
                Layout.topMargin: 20
                text: qsTr("编辑资源")
                font.bold: true
                color: "#FFFFFF"
                font.pixelSize: 24
            }

            Item {
                Layout.fillWidth: true
            }

            Image{
                Layout.rightMargin: 35
                width:24
                height: 24
                source: "qrc:/res/image/yvr_close.png"
                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        mainWin.hideSubWin()
                        subWindow.source = ""
                    }
                }
            }
        }

        ColumnLayout{
            id: mainspace
            anchors.top: titleBar.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 12

            Rectangle{
                width: 560
                height: 44
                color: "#33343D"
                radius: 8
                RowLayout{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    spacing: 8
                    Repeater{
                        model: yvr.resourcesListModelAdd.videoGroups
                        delegate: Rectangle
                        {
                            width: 56
                            height: 26
                            color:selectIndex === index? "#3476FB" :"#555763"
                            radius: 4
                            Text {
                                anchors.centerIn: parent
                                width: 46
                                color: "#C4C4C4"
                                font.pixelSize: 12
                                text: modelData
                                elide: Text.ElideRight
                                horizontalAlignment: Text.AlignHCenter
                            }

                            MouseArea
                            {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    //selectIndex = index
                                    mainWin.showToast(qsTr("编辑不支持分组修改"))
                                }
                            }
                        }
                    }

                }
            }

            YVRTextInput{
                id: showName
                height: 44
                defaultText: showNameText
                tipText: qsTr("请输入资源名称")
            }

            YVRTextInput{
                id: fileDesc
                height: 88
                defaultText: fileDescText
                tipText: qsTr("请输入资源介绍")
            }

            ComboBox {
                id: control
                Layout.preferredWidth: 560
                Layout.preferredHeight: 46

                model:isVideo ?  ["2D", "180°[2D]", "360°[2D]", "3D", "180°[3D]","360°[3D]"]
                              : ["2D", qsTr("立方体投影"),qsTr("360°全景"),qsTr("180°全景")]

                currentIndex: {
                   if(type > 5)
                   {
                        return type - 3;
                   }
                   return type;
                }

                delegate: ItemDelegate {
                    width: 560
                    height: 30
                    background: Rectangle{
                        color: parent.highlighted ?  "#40414F" : "#33343D"
                        radius: 8
                        anchors.fill: parent
                    }

                    contentItem: Text {
                        text: modelData
                        color: parent.highlighted ? "#FFFFFF" : "#99FFFFFF"
                        font.pixelSize: 16
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: control.highlightedIndex === index
                }

                indicator:Image {
                        anchors.right: parent.right
                        anchors.rightMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        width: 24
                        height: 24
                        source: "qrc:/res/image/yvr_arrow_down.png"
                    }

                contentItem: Text {
                    leftPadding: 16
                    rightPadding: control.indicator.width + control.spacing

                    text: control.displayText
                    font: control.font
                    color: "#FFFFFF"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 560
                    implicitHeight: 54
                    color: "#33343D"
                    radius: 8
                }

                popup: Popup {
                    y: control.height + 5
                    width: control.width
                    implicitHeight: contentItem.implicitHeight
                    padding: 1

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight + 2
                        model: control.popup.visible ? control.delegateModel : null
                        currentIndex: control.highlightedIndex

                        ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        color: "#33343D"
                        radius: 8
                    }
                }
            }

            ComboBox {
                id: select3D
                visible: isVideo && control.currentIndex > 2
                Layout.preferredWidth: 560
                Layout.preferredHeight: 46
                currentIndex: type > 5 ? 0 : 1
                model: [qsTr("3D左右"), qsTr("3D上下")]

                delegate: ItemDelegate {
                    width: 560
                    height: 30
                    background: Rectangle{
                        color: parent.highlighted ?  "#40414F" : "#33343D"
                        radius: 8
                        anchors.fill: parent
                    }

                    contentItem: Text {
                        text: modelData
                        color: parent.highlighted ? "#FFFFFF" : "#99FFFFFF"
                        font.pixelSize: 16
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: select3D.highlightedIndex === index
                }

                indicator:Image {
                        anchors.right: parent.right
                        anchors.rightMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        width: 24
                        height: 24
                        source: "qrc:/res/image/yvr_arrow_down.png"
                    }

                contentItem: Text {
                    leftPadding: 16
                    rightPadding: select3D.indicator.width + select3D.spacing

                    text: select3D.displayText
                    font: select3D.font
                    color: "#FFFFFF"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 560
                    implicitHeight: 54
                    color: "#33343D"
                    radius: 8
                }

                popup: Popup {
                    y: select3D.height + 5
                    width: select3D.width
                    implicitHeight: contentItem.implicitHeight
                    padding: 1

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight + 2
                        model: select3D.popup.visible ? select3D.delegateModel : null
                        currentIndex: select3D.highlightedIndex

                        ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        color: "#33343D"
                        radius: 8
                    }
                }
            }

            Rectangle{
                radius: 8
                width: 560
                height: 46
                color: "#33343D"
                Layout.alignment: Qt.AlignHCenter

                RowLayout{
                    spacing: 20
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        leftPadding:16
                        Layout.fillWidth: true
                        text: file
                        font.pixelSize: 20
                        elide: Text.ElideRight
                        color: "#99FFFFFF"
                    }

                    Rectangle {
                        Layout.rightMargin: 16
                        width: 120
                        height: 30
                        color: "#606171"
                        radius: 6
                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 14
                            color: "#D8D8D8"
                            text: qsTr("上传文件")
                        }

                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                filedialog.nameFilters = "Video files(*.mkv *.mp4 *.3gp *.avi *.flv *.m4v *.mov *.mpeg *.mpg *.webm *.wmv)"
                                filedialog.isPoster = false
                                filedialog.open()
                            }
                        }
                    }
                }
            }

            Rectangle{
                radius: 8
                width: 560
                height: 54
                color: "#33343D"
                Layout.alignment: Qt.AlignHCenter

                RowLayout{
                    spacing: 20
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        leftPadding:16
                        Layout.fillWidth: true
                        text: poster
                        font.pixelSize: 20
                        elide: Text.ElideRight
                        color: "#99FFFFFF"
                    }

                    Rectangle{
                        Layout.rightMargin: 16
                        width: 120
                        height: 30
                        color: "#606171"
                        radius: 6
                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 14
                            color: "#D8D8D8"
                            text: qsTr("上传海报")
                        }

                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                filedialog.nameFilters = "Image files(*.png *.jpg *.jpeg)"
                                filedialog.isPoster = true
                                filedialog.open()
                            }
                        }
                    }
                }
            }

            YVRButton{
                Layout.topMargin: 14
                width: 276
                Layout.alignment: Qt.AlignHCenter
                onClickSignal: {

                    var type = control.currentIndex

                    if(isVideo)
                    {
                        if(type > 3 && select3D.currentIndex === 0)
                        {
                            type = type + 7
                        }else
                        {
                            type = type + 4
                        }
                    }

                    yvr.resourcesListModelAdd.modify(index, showName.inputText, fileDesc.inputText,
                                        file, poster, type, selectIndex)
                    mainWin.hideSubWin()
                    subWindow.source = ""
                }
            }
        }
    }

    FileDialog{
        id: filedialog
        property bool  isPoster: false
        selectExisting: true
        title: "YVR"
        //folder: yvr.defaultPath()
        selectFolder: false
        selectMultiple: false
        nameFilters: ["Video files(*.mkv *.mp4 *.3gp *.avi *.flv *.m4v *.mov *.mpeg *.mpg *.webm *.wmv)"]
        onAccepted: {
            if(isPoster)
                poster = filedialog.fileUrl
            else
               file = filedialog.fileUrl
        }
    }
}
