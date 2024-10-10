#include "VRMonitor.h"
#include "VRListModel.h"
#include "VRDiscoverListModel.h"
#include "network/YVRWebServer.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "plugins/JsonFileParse.h"
#include "Settings.h"
#include <QJsonArray>

VRMonitor::VRMonitor(QObject *parent) : QObject(parent)
{
    m_pVRList = new VRListModel(this);
    m_pVRListDiscover = new VRDiscoverListModel(this);

    for(auto item: JsonFileParse::ins()->vrList())
    {
        auto sn = item.toObject()["sn"].toString();

        Settings::Instance().modiftyDevices(sn, item.toObject()["index"].toInt());
    }
}

VRMonitor::~VRMonitor()
{
    qInfo("close VRMonitor");
    m_pWebServer->close();
}

void VRMonitor::stopPlay(int index)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::StopPlay;

    auto res = m_pVRList->vrPlayingList(index);

    for(auto item : res)
    {
        sendMsg(obj, item);
    }
}

void VRMonitor::startPlay(int index, QJsonObject data,  bool selectAll)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::StartPlay;
    obj["data"] = data;

    auto res = m_pVRList->vrPlayList(selectAll);

    for(auto item : res)
    {
        m_pVRList->updateVideo(item, index);
        sendMsg(obj, item);
    }

    m_pVRList->refresh(true);
}

void VRMonitor::uncontrol()
{
    QJsonObject obj;
    obj["type"] = VRMsgType::Controlled;

    QJsonObject data;
    data["controlled"] = false;
    obj["data"] = data;
    
    auto list = m_pVRList->vrList();

    for(auto item : list)
    {
        data["id"] = Settings::Instance().getDeviceId(item);
        obj["data"] = data;
        sendMsg(obj, item);
    }
}

void VRMonitor::control()
{
    QJsonObject obj;
    obj["type"] = VRMsgType::Controlled;

    QJsonObject data;
    data["controlled"] = true;


    auto list = m_pVRList->vrList();

    for(auto item : list)
    {
        data["id"] = Settings::Instance().getDeviceId(item);
        obj["data"] = data;
        sendMsg(obj, item);
    }
}

void VRMonitor::modiftyDevice(QString sn, int id)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::Controlled;

    QJsonObject data;
    data["controlled"] = Settings::Instance().getControl();

    data["id"] =  id;
    obj["data"] = data;
    sendMsg(obj, sn);
}

void VRMonitor::startAutonomous(QString sn)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::StartAutonomous;
    
    sendMsg(obj, sn);
}

void VRMonitor::rebootP(bool all)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::RebootP;


    auto list = m_pVRList->selectList(all);
    for(auto item: list)
    {
        sendMsg(obj, item);
    }

}

void VRMonitor::rebootN(bool all)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::RebootN;

     auto list = m_pVRList->selectList(all);
    for(auto item: list)
    {
        sendMsg(obj, item);
    }
}

void VRMonitor::rebootP(QString sn)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::RebootP;
    sendMsg(obj, sn);
}

void VRMonitor::rebootN(QString sn)
{
    QJsonObject obj;
    obj["type"] = VRMsgType::RebootN;
    sendMsg(obj, sn);
}

QVariant VRMonitor::vrList() const
{
    return  QVariant::fromValue(m_pVRList);
}

QVariant VRMonitor::vrDiscoverList() const
{
     return  QVariant::fromValue(m_pVRListDiscover);
}

void VRMonitor::startWebsocket(int port)
{
    if(m_pWebServer == nullptr)
    {
        m_pWebServer = new YVRWebServer(port, this);
        connect(m_pWebServer, &YVRWebServer::deviceStateChange, this, &VRMonitor::statesChange);
        connect(m_pWebServer, &YVRWebServer::deviceInfoChange, this, &VRMonitor::infoChange);
    }
}

void VRMonitor::refresh()
{
    m_pVRList->refresh();
}

void VRMonitor::loadDevices(QString path)
{
      QFile file(path);
      if(file.open(QIODevice::ReadWrite))
      {
        auto data = QString::fromUtf8(file.readAll());
        auto lines =  data.split("\r\n");

        for(auto line: lines)
        {
            auto sns = line.split(',');
            for(auto sn : sns)
            {
                if(sn.contains("ZTR") || sn.contains("ZTS") ||sn.contains("D2H"))
                {
                    sn.remove("[^A-Z0-9]");

                    if(sn.size() <11)
                        continue;

                    if(!m_pVRList->contains(sn))
                    {
                        int id = Settings::Instance().getDeviceId(sn);
                        m_pVRList->add(sn, "默认",QString("%1").arg(id, 3, 10, QLatin1Char('0')));
                  }
              }
           }
        }
      }

      m_pVRListDiscover->clear(true);
      m_pVRList->refresh(true);
      m_pVRList->save();
}

void VRMonitor::addDiscoverDeice(bool all)
{
    for(YVRDiscoverDevice& item : m_pVRListDiscover->getAllDevices(all))
    {
          m_pVRList->add(item.sn, item.group, item.id);
    }

    m_pVRListDiscover->clear(all);
    m_pVRList->refresh(true);
    m_pVRList->save();
}

void VRMonitor::sendMsg(QJsonObject &obj, QString sn) const
{
    if(m_pWebServer != nullptr)
    {
        QByteArray  data =  QJsonDocument(obj).toJson(QJsonDocument::Compact);
        m_pWebServer->sendCommand(data, sn);
    }
}

void VRMonitor::save()
{
    m_pVRList->save();
}

void VRMonitor::statesChange(QString sn, bool offline, bool onuse, bool controled, int playStatus)
{
    if(m_pVRList->contains(sn))
    {
        m_pVRList->statesChange(sn, offline, onuse, controled, playStatus);
        return;
    }

    m_pVRListDiscover->deviceChange(sn, offline, Settings::Instance().getDeviceId(sn));
}

void VRMonitor::infoChange(QString sn, QJsonObject &data)
{
    if(m_pVRList->contains(sn))
    {
        m_pVRList->infoChange(sn, data);
        return;
    }

    auto offline = data["deviceStatus"].toInt(0) == 0;

    m_pVRListDiscover->deviceChange(sn, offline, Settings::Instance().getDeviceId(sn));
}

