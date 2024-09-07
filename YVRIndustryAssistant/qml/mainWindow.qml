import QtQuick 2.14
import QtShark.Window 1.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.2
import QtQml 2.0
import YVRManager 1.0
import "./windows"
import "./components"
import "./control"


FramelessWindow {

    property int main_width: 1280
    property int main_height: 720
    property int scaleSize: 1
    property int pageIndex: YVRManagerCLS.DeviceManager
    id: mainWin
    //property int preWidth: 1280
    property bool hasDevices: yvr.curSN !== ""
    visible: true
    title: qsTr("玩出梦想行业助手")
    width: main_width * scaleSize
    height: main_height * scaleSize
    color: "transparent"
    titleBar: titleBar

    Rectangle{
        scale: scaleSize
        width: 1280
        height: 720
        anchors.centerIn: parent
        color: utils.yvr_backgroundcolor

        ColumnLayout{
            anchors.fill: parent
            spacing: 0
            RowLayout {
                id: titleBar
                Layout.preferredHeight: 40
                Layout.leftMargin: 30

                Item {
                    Layout.fillWidth: true
                }


                Image {
                    width: 24
                    height: 24
                    source: "qrc:/res/image/yvr_min.png"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            mainWin.showMinimized()
                        }
                    }
                }

                Image {
                    Layout.rightMargin: 20
                    Layout.leftMargin: 12
                    width: 24
                    height: 24
                    source: "qrc:/res/image/yvr_close.png"
                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            mainWin.close()
                        }
                    }
                }


            }

            RowLayout{
                Layout.preferredWidth: parent.width
                Layout.fillHeight:  true
                spacing: 0

                ColumnLayout {
                    Layout.preferredWidth: 92
                    Layout.fillHeight:  true
                    spacing: 12

                    Image {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight:  44
                        Layout.preferredWidth: 44
                        source: "qrc:/res/image/yvr_logo.png"
                    }

                    Rectangle{
                        width: 64
                        height: 1
                        color: "#2E3037"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    YVRImageButton{
                        Layout.alignment: Qt.AlignHCenter
                        showText: qsTr("设备管理")
                        selected: pageIndex === YVRManagerCLS.DeviceManager
                        showIcon: "qrc:/res/image/yvr_device_manager.png"
                        onClickSignal: showPage(YVRManagerCLS.DeviceManager)

                    }

                    YVRImageButton{
                        Layout.alignment: Qt.AlignHCenter
                        showText: qsTr("播控")
                        selected: pageIndex === YVRManagerCLS.PlayControl
                        showIcon: "qrc:/res/image/yvr_play_tab.png"
                        onClickSignal: showPage(YVRManagerCLS.PlayControl)
                    }

                    YVRImageButton{
                        visible: false
                        Layout.alignment: Qt.AlignHCenter
                        showText: qsTr("无人值守")
                        selected: pageIndex === YVRManagerCLS.Autonomous
                        showIcon: "qrc:/res/image/yvr_no_one.png"
                        onClickSignal: showPage(YVRManagerCLS.Autonomous)
                    }

                    Item {
                        Layout.fillHeight: true
                    }

                    YVRImageButton{
                        Layout.alignment: Qt.AlignHCenter
                        showText: qsTr("方案管理")
                        selected: pageIndex === YVRManagerCLS.PlanManager
                        showIcon: "qrc:/res/image/yvr_plan_manager.png"
                        onClickSignal: showPage(YVRManagerCLS.PlanManager)
                    }

                    YVRImageButton{
                        Layout.bottomMargin: 44
                        Layout.alignment: Qt.AlignHCenter
                        showText: qsTr("设置")
                        selected: pageIndex === YVRManagerCLS.ProgramSettings
                        showIcon: "qrc:/res/image/yvr_setting.png"
                        onClickSignal: showPage(YVRManagerCLS.ProgramSettings)
                    }
                }

                Rectangle
                {
                    color: utils.yvr_itemdowncolor
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Loader{
                        id: pageLoader
                        anchors.fill: parent
                    }
                }

            }
        }

        Component.onCompleted: {
            showPage(YVRManagerCLS.DeviceManager)
            mainWin.x = 320
            mainWin.y = 160
        }
    }

    Rectangle{
        visible: subWindow.source.toString() !== "" &&  subWindow.item.visible
        anchors.fill: parent
        color: "#1A1B21"

        opacity: 0.3

        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            propagateComposedEvents: false
        }
    }

    Loader {
        id: subWindow
        anchors.centerIn: parent
        visible: false
        scale: scaleSize

        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            propagateComposedEvents: false
        }
    }

    Utils{
        id: utils
    }

    FramelessWindow{
        id: toast
        flags: Qt.WindowStaysOnTopHint
        needShadow: false
        color: "transparent"
        Rectangle{
            anchors.fill: parent
            radius: 8
            color: "#000000"
        }

        Text {
            x:12
            y:8
            id: toastText
            color: "#FFFFFF"
            onTextChanged: {
                toast.width = toastText.width + 24
                toast.height = toastText.height + 16
            }
        }

        Timer{
            id: timer
            interval: 2000
            onTriggered: {
                toast.visible = false
            }
        }

        onVisibleChanged: {
            if(toast.visible)
            {
                timer.start()
            }
        }
    }

    onCloseAll: {
        close()
    }

    onWidthChanged: {

//        if(width === main_width)
//        {
//            preWidth = main_width
//            return
//        }

//        if(width !== preWidth && width !== main_width)
//        {
//            preWidth = width
//            yvr.mainWinsizeChange()
//        }
    }

    Connections{
        target: yvr

        // @disable-check M16
        onShowToast:{
            showToast(toast)
        }

        // @disable-check M16
        onChangeMainwinSize:{
            mainWin.width = main_width
            mainWin.height = main_height
        }
    }

    function showTips(icon, tips)
    {
        tipsIcon.source = icon
        tipsText.text = tips
        tipsrect.visible = true
    }

    function showToast(str)
    {
        toastText.text = str
        toast.x = mainWin.x + main_width/2 - toast.width /2
        toast.y = mainWin.y + 400
        toast.show()
    }

    function showSubWin()
    {
//        subWindow.item.x = mainWin.x + main_width/2 - subWindow.item.width/2
//        subWindow.item.y = mainWin.y + main_height/2 - subWindow.item.height/2
        subWindow.visible = true
    }

    function hideSubWin()
    {
        subWindow.visible = false
    }

    function showPage(page)
    {
        switch(page){
        case YVRManagerCLS.DeviceManager:
            if(yvr.vrListModel.rowCount() === 0)
                pageLoader.source = "qrc:/qml/components/DeviceEmptyPage.qml"
            else
                pageLoader.source = "qrc:/qml/components/DeviceManagePage.qml"
            break;
        case YVRManagerCLS.PlayControl:
            pageLoader.source = "qrc:/qml/components/ResourcesPage.qml"
            break;
        case YVRManagerCLS.Autonomous:
            pageLoader.source = "qrc:/qml/components/AutonomousPage.qml"
            break;
        case YVRManagerCLS.PlanManager:
            pageLoader.source = "qrc:/qml/components/HomePage.qml"
            break;
        case YVRManagerCLS.DeploymentPlan:
            pageLoader.source = "qrc:/qml/components/DeploymentPlan.qml"
            break;
        case YVRManagerCLS.ProgramSettings:
            pageLoader.source = "qrc:/qml/components/Settings.qml"
            break;
        }

        pageIndex = page;
    }
}
