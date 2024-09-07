import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtShark.Window 1.0
import QtQuick.Dialogs 1.3
import "../control"


Rectangle {
    id: root
    width: 800
    height: mainspace.height + 82
    visible: true
    color: "transparent"
    property bool isVideo: true
    property bool isPlayControl: true
    property bool isVideoOnly: false
    property string file: qsTr("请上传文件")
    property string poster: qsTr("请上传封面")
    property var selectIndex: 0

    Rectangle{
        anchors.fill: parent
        color: "#22242C"
        radius: 16
        RowLayout{
            width: parent.width
            height: 42
            id: titleBar

            Text {
                Layout.leftMargin: 328
                Layout.topMargin: 20
                text: qsTr("添加资源")
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
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 12
            RowLayout{
                spacing: 40
                Layout.topMargin: 30
                Item {
                    width: 75
                    height: 40
                    Layout.topMargin: 0
                    ColumnLayout{
                        Text {
                            text: qsTr("添加视频")
                            font.pixelSize: 18
                            color: isVideo ?  "#FFFFFF" : "#99FFFFFF"
                        }

                        Rectangle{
                            width: 40
                            height: 4
                            radius: 4
                            color: "#1E68FF"
                            Layout.alignment: Qt.AlignHCenter
                            visible: isVideo
                        }

                    }

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            isVideo = true
                        }
                    }
                }

                Item {
                    visible: !isVideoOnly
                    width: 75
                    height: 40
                    property bool seected: false
                    ColumnLayout{
                        Text {
                            text: qsTr("添加图片")
                            font.pixelSize: 18
                            color: isVideo ?   "#99FFFFFF": "#FFFFFF"
                        }

                        Rectangle{
                            width: 40
                            height: 4
                            radius: 4
                            color: "#1E68FF"
                            Layout.alignment: Qt.AlignHCenter
                            visible: !isVideo
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            isVideo = false
                        }
                    }
                }
            }

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
                            color: selectIndex == index ? "#3476FB" :"#555763"
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
                                    selectIndex = index
                                }
                            }
                        }
                    }

                }
            }

            YVRTextInput{
                id: showName
                height: 44
                tipText: qsTr("请输入资源名称")
            }

            YVRTextInput{
                id: fileDesc
                height: 88
                tipText: qsTr("请输入资源介绍")
            }


            YVRCombox{
                id: control
                currentIndex: 0
                curText: qsTr("2D")
                controlModel: isVideo ? ["2D", "180°[2D]", "360°[2D]", "3D", "180°[3D]","360°[3D]"]
                               : ["2D", qsTr("立方体投影"),qsTr("360°全景"),qsTr("180°全景")]

                onSelectItem: root.update()
            }


            YVRCombox{

                id: select3D
                visible: isVideo && control.currentIndex > 2
                currentIndex: 0
                curText: qsTr("3D左右")
                controlModel: [qsTr("3D左右"), qsTr("3D上下")]
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
                                if(isVideo)
                                {
                                    filedialog.nameFilters = "Video files(*.mkv *.mp4 *.3gp *.avi *.flv *.m4v *.mov *.mpeg *.mpg *.webm *.wmv)"
                                }else
                                {

                                    filedialog.nameFilters = "Image files(*.png *.jpg *.jpeg)"
                                }

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
                height: 44
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
                height: 44
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


                    yvr.resourcesListModelAdd.add(showName.inputText, fileDesc.inputText,
                                     file, poster, type, selectIndex)

                    file = qsTr("请上传文件")
                    poster = qsTr("请上传封面")
                    fileDesc.clearInput()
                    showName.clearInput()
                    control.currentIndex = 0
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
        nameFilters: ["Image files(*.png *.jpg *.jpeg)", "Video files(*.mkv *.mp4 *.3gp *.avi *.flv *.m4v *.mov *.mpeg *.mpg *.webm *.wmv)"]
        onAccepted: {
            if(isPoster)
                poster = filedialog.fileUrl
            else
                file = filedialog.fileUrl
        }
    }
}
