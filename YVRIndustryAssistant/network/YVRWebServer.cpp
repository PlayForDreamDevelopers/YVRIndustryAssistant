#include "YVRWebServer.h"
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>
#include <QJsonDocument>
#include "./../Settings.h"
#include <QJsonObject>
#include <QDebug>

YVRWebServer::YVRWebServer(quint16 port, QObject *parent) : QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("YVRServer"),
                                            QWebSocketServer::NonSecureMode,
                                            this))
{
    if(m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &YVRWebServer::onNewConnection);
    }else
    {
        qInfo("WebSocketServer Start fail");
    }
}

void YVRWebServer::sendCommand(QByteArray &data, QString sn)
{

    if(sn.isEmpty())
    {
       for(auto device :m_clients)
       {

           if(device != nullptr)
           {
              qInfo()<< QString("%1: %2").arg(sn).arg(QString(data));
              device->sendTextMessage(data);
           }
       }

    }else
    {

        if(m_clients.contains(sn) && m_clients[sn] != nullptr)
        {
            qInfo()<< QString("%1: %2").arg(sn).arg(QString(data));
            m_clients[sn]->sendTextMessage(data);
        }
    }
}

void YVRWebServer::close()
{
    qInfo("close YVRWebServer");
    for(auto device :m_clients)
    {
        if(device != nullptr)
           device->close();
    }

    m_pWebSocketServer->close();
}

void YVRWebServer::onNewConnection()
{
    auto pSocket = m_pWebSocketServer->nextPendingConnection();
    //QTextStream(stdout) << getIdentifier(pSocket) << " connected!\n";
    pSocket->setParent(this);

    qInfo() << "new connnect";

    connect(pSocket, &QWebSocket::textMessageReceived,
            this, &YVRWebServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected,
            this, &YVRWebServer::socketDisconnected);

    QObject::connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                       this,  SLOT(socketError(QAbstractSocket::SocketError)));
}

void YVRWebServer::processMessage(const QString &message)
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());

    QJsonParseError josnError;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &josnError);


    if(doc.isEmpty() || (josnError.error != QJsonParseError::NoError))
    {
        qInfo() << "msg parse error: "<< message;
        return;
    }

    QJsonObject reply = doc.object();
    VRMsgType msgType = static_cast<VRMsgType>(reply["type"].toInt(9999));

    if(reply.contains("data"))
    {
        QJsonObject data = reply["data"].toObject();

        QString sn = data["sn"].toString();

        if(m_clients.contains(sn))
        {
            if(pSender != m_clients[sn])
            {
                if(m_clients[sn] != nullptr)
                {
                    m_clients[sn]->deleteLater();
                }

                m_clients[sn]= pSender;
                int id = Settings::Instance().getDeviceId(sn);
                bool type = Settings::Instance().getControl();
                QString res = "{\"data\":{\"id\":%1,\"controlled\":%2},\"type\":2004}}";
                res = res.arg(id).arg(type? "true" : "false");
                pSender->sendTextMessage(res);
            }
        }else
        {
            m_clients[sn]= pSender;

            int id = Settings::Instance().getDeviceId(sn);
            bool type = Settings::Instance().getControl();
            QString res = "{\"data\":{\"id\":%1,\"controlled\":%2},\"type\":2004}}";
            res = res.arg(id).arg(type? "true" : "false");
            pSender->sendTextMessage(res);
        }

        handleMsg(msgType, data);
    }

    if(reply.contains("code") && reply["code"].toInt() != 0)
    {
        qInfo() << "error msg reply: "<< message;
    }
}

void YVRWebServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    //QTextStream(stdout) << getIdentifier(pClient) << " disconnected!\n";
    if (pClient)
    {
        QString sn;
        for (auto &item : m_clients.toStdMap()) {
            if (pClient == item.second)
            {
                sn = item.first;
                qInfo() << "disconnnect: " << sn;
                m_clients[sn] = nullptr;
                emit deviceStateChange(sn, true, false, false, 2);
                break;
            }
        }

        pClient->deleteLater();
    }
}

void YVRWebServer::socketError(QAbstractSocket::SocketError error)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    qInfo() << "socketError:" << error;

    if (pClient && error == QAbstractSocket::RemoteHostClosedError)
    {
        for (auto &item : m_clients.toStdMap()) {
            if (pClient == item.second)
            {
                QString sn = item.first;
                qInfo() << "disconnnect: " << sn;
                m_clients[sn] = nullptr;
                pClient->close();
                pClient->deleteLater();
                emit deviceStateChange(sn, true, false, false, 2);
                break;
            }
        }
    }
}


void YVRWebServer::handleMsg(VRMsgType type, QJsonObject &data)
{
    QString sn = data["sn"].toString();
    //qInfo()<< data;
    switch (type) {
    case VRMsgType::OnLine:
    case VRMsgType::HeartBeat:
    {
        emit deviceInfoChange(sn, data);
    }
        break;
    case VRMsgType::DeviceStatus:
    case VRMsgType::DeviceInformation:
    {

    }
        break;
    case VRMsgType::RequestDeviceStatus:
    {

    }
        break;
    case VRMsgType::ReportError:
    {

    }
        break;
    case VRMsgType::StartPlay:
        break;
    case VRMsgType::StopPlay:
        break;
    default:
        break;
    }
}
