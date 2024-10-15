#include "YVRManager.h"
#include <QJsonArray>
#include <QDebug>
#include <QJsonObject>
#include <QTimer>
#include <QVariant>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QThread>
#include "VRMonitor.h"
#include "Settings.h"
#include "version.h"
#include "plugins/JsonFileParse.h"
#include "ResourceListModel.h"
#include "ResourceAddListModel.h"
#include "plugins/utils.h"
#include  "network/ConnectManager.h"

//address ws://127.0.0.1:50208
const int YVRWebServerPort = 50208;

YVRManager::YVRManager(QObject *parent) : QObject(parent)
{
    qInfo("app satrt");
    m_monitor = new VRMonitor(this);
    m_UDPBroadCast = new ConnectManager(this);
    m_UDPBroadCast->startUDPBroadCast(YVRWebServerPort - 1);
    m_monitor->startWebsocket(YVRWebServerPort);

    auto task = [=](){ Settings::Instance().load();};

    std::thread t(task);
    t.detach();

    //startTimer(2000);
}

YVRManager::~YVRManager()
{
   delete m_UDPBroadCast;
   m_monitor->save();

   if(m_resourcesListAdd != nullptr)
       m_resourcesListAdd->save();

    qInfo("app close");
}

void YVRManager::mainWinsizeChange()
{
    if(m_needSetMainWinSize)
        return;

     m_needSetMainWinSize = true;
     QTimer::singleShot(500, this, [=](){
         m_needSetMainWinSize = false;
         emit changeMainwinSize();
     });
}

void YVRManager::gotoFolder(QString path)
{
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

void YVRManager::deploymentPlan()
{
    m_fileSavePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/YVRIndustryAssistant";

    QDir path("D:/");
    if(path.exists() && !IsRemovableDrive(L"D:/"))
    {
        m_fileSavePath = "D:/YVRIndustryAssistant";
    }

    emit fileSavePathChange();


    QJsonArray fileArray;
    JsonFileParse::ins()->setPort(YVRWebServerPort);
    Settings::Instance().deployment(m_fileSavePath + "/config");

    gotoFolder(m_fileSavePath);

    auto task = [=](){
        m_monitor->save();
        m_resourcesListAdd->save();

        if( m_resourcesList != nullptr)
            m_resourcesList->load();
    };

    std::thread t(task);
    t.detach();
}

QString YVRManager::getVersion()
{
    return QString(PFDMVersion);
}


void YVRManager::loadDevicesExcel(QString path)
{
    m_monitor->loadDevices(path.remove("file:///"));
}

void YVRManager::setRefreshState(bool refresh)
{
    qInfo() << "refresh: " << refresh;
    m_realTimeFresh = refresh;
}

void YVRManager::rebootN(bool selectAll)
{
    m_monitor->rebootN(selectAll);
}

void YVRManager::rebootP(bool selectAll)
{
    m_monitor->rebootP(selectAll);
}

void YVRManager::rebootN(QString sn)
{
    m_monitor->rebootN(sn);
}

void YVRManager::rebootP(QString sn)
{
    m_monitor->rebootP(sn);
}

void YVRManager::modiftyDevice(QString sn, QString id)
{
    m_monitor->modiftyDevice(sn, id.toInt());
}

void YVRManager::addDiscoverDeice(bool selectAll)
{
    m_monitor->addDiscoverDeice(selectAll);
}


void YVRManager::pause(int index)
{
    m_monitor->stopPlay(index);
    m_resourcesList->stoppublish(index);
    Settings::Instance().publishInfo(false, index);
}


void YVRManager::play(int index, bool loopPlay,  bool selectAll)
{
    QJsonObject obj;

    obj["index"] = index;
    obj["loopPlay"] = loopPlay;
    m_monitor->startPlay(index, std::move(obj), selectAll);

    m_resourcesList->publish(index);
    Settings::Instance().publishInfo(true, index);
}

void YVRManager::clearPlan()
{
    m_resourcesListAdd->clear();
    Settings::Instance().clear();

    if(m_resourcesList != nullptr)
    {
        m_resourcesList->load();
    }

    m_resourcesListAdd->showGroup(0);
}

void YVRManager::showDeviceExampleExcel()
{
    auto res = QGuiApplication::applicationDirPath() + "/example/";
    QDir dir;
    if(!dir.exists(res))
    {
        dir.mkdir(res);
        QFile file(res+ "example.csv");
        file.open(QIODevice::Truncate |QIODevice::WriteOnly);
        file.write("D2HD232328D9000813\r\nD2HD232328D9000813\r\nZTR49R160019");
        file.close();
    }

    gotoFolder(res);
}

bool YVRManager::verfityId(QString sn, QString id)
{
    return Settings::Instance().verifyId(sn, id);
}

bool YVRManager::firstPlayControl() const
{
    if(Settings::Instance().getFirstPlayControl())
    {
        Settings::Instance().setFirstPlayControl(false);
        return true;
    }

    return false;
}

bool YVRManager::firstAddDevice() const
{
    if(Settings::Instance().getFirstAddDevice())
    {
        Settings::Instance().setFirstAddDevice(false);
        return true;
    }

    return false;
}

bool YVRManager::hasPulished()
{
    if(m_resourcesList == nullptr)
        return false;
    return m_resourcesList->getHasPublish();
}

QVariant YVRManager::resourcesList()
{
    if(m_resourcesListAdd == nullptr)
    {
        m_resourcesListAdd = new ResourceAddListModel(this);
        m_resourcesListAdd->showGroup(0);
    }

    if(m_resourcesList == nullptr)
    {
        m_resourcesList = new ResourceListModel(this);
        m_resourcesList->load();
    }

    return QVariant::fromValue(m_resourcesList);
}

QVariant YVRManager::resourcesListAdd()
{
    if(m_resourcesListAdd == nullptr)
    {
        m_resourcesListAdd = new ResourceAddListModel(this);
        m_resourcesListAdd->showGroup(0);
    }

    return QVariant::fromValue(m_resourcesListAdd);
}

QVariant YVRManager::resourcesListAuto()
{
    if(m_resourcesListAuto == nullptr)
    {
        m_resourcesListAuto = new ResourceAddListModel(this);
        m_resourcesListAuto->showGroup(0);
    }

    return QVariant::fromValue(m_resourcesListAuto);
}

QVariant YVRManager::vrList() const
{
    return m_monitor->vrList();
}


QVariant YVRManager::vrDiscoverList() const
{
    return m_monitor->vrDiscoverList();
}

QString YVRManager::fileSavePath() const
{
    return m_fileSavePath;
}

bool YVRManager::getControl() const
{
    return  Settings::Instance().getControl();
}

void YVRManager::setControl(bool is)
{
    Settings::Instance().setControl(is);

    if(is)
        m_monitor->control();
    else {
        m_monitor->uncontrol();
    }

    controlChange();
}

void YVRManager::timerEvent(QTimerEvent *)
{
    if(m_realTimeFresh)
    {
        m_monitor->refresh();
    }
}
