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
    m_monitor = new VRMonitor(this);
    m_UDPBroadCast = new ConnectManager(this);
    m_UDPBroadCast->startUDPBroadCast(YVRWebServerPort - 1);
    m_monitor->startWebsocket(YVRWebServerPort);

    m_resourcesListAdd = new ResourceAddListModel(this);
    // m_resourcesListAdd->load(JsonFileParse::ins()->fileList());
    auto task = [=](){ m_resourcesListAdd->load(JsonFileParse::ins()->fileList());};

    std::thread t(task);
    t.detach();

    //startTimer(2000);
}

YVRManager::~YVRManager()
{
    delete m_UDPBroadCast;

   if(m_resourcesListAdd != nullptr)
       m_resourcesListAdd->save(true);

   m_monitor->save();
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
    m_resourcesListAdd->fileArray(fileArray, m_fileSavePath + "/config");
    JsonFileParse::ins()->setPort(YVRWebServerPort);
    JsonFileParse::ins()->deployment(fileArray, m_fileSavePath + "/config");

    gotoFolder(m_fileSavePath);

    auto task = [=](){
        m_monitor->save();
        m_resourcesListAdd->save(true);
        if( m_resourcesList != nullptr)
            m_resourcesList->load(JsonFileParse::ins()->fileList());
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

void YVRManager::addDiscoverDeice(bool selectAll)
{
    m_monitor->addDiscoverDeice(selectAll);
}


void YVRManager::pause(int index)
{
    m_monitor->stopPlay(index);
    m_resourcesList->stoppublish(index);
}


void YVRManager::play(int index, bool loopPlay,  bool selectAll)
{
    QJsonObject obj;

    obj["index"] = index;
    obj["loopPlay"] = loopPlay;
    m_monitor->startPlay(index, std::move(obj), selectAll);

    m_resourcesList->publish(index);
}

void YVRManager::clearPlan()
{
    m_resourcesListAdd->clear();

    if(m_resourcesList)
    {
        m_resourcesList->clear();
    }

    JsonFileParse::ins()->updateFile(QJsonArray());
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
        file.write("1,D2HD232328D9000813\r\n2,D2HD232328D9000813\r\n3,ZTR49R160019");
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

QVariant YVRManager::resourcesList()
{

    if(m_resourcesList == nullptr)
    {
        m_resourcesList = new ResourceListModel(this);
        m_resourcesList->load(JsonFileParse::ins()->fileList());
    }

    return QVariant::fromValue(m_resourcesList);
}

QVariant YVRManager::resourcesListAdd()
{
    return QVariant::fromValue(m_resourcesListAdd);
}

QVariant YVRManager::resourcesListAuto()
{
    if(m_resourcesListAuto == nullptr)
    {
        m_resourcesListAuto = new ResourceAddListModel(this);
        m_resourcesListAuto->load(JsonFileParse::ins()->fileList());
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
