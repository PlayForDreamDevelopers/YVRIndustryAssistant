#include "ConnectManager.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkDatagram>
#include <QTcpSocket>
#include <QByteArray>
#include <QUdpSocket>
#include <QString>
#include "version.h"

#define SOFT_NAME "PFDMPlayControlServer"

struct NetworkInfo {
    int interfaceIndex;
    QString networkCategory;
    QString ipv4Connectivity;
};

//Disconnected：设备没有连接到任何网络。
//NoTraffic：设备连接到网络，但没有检测到IPv4流量。
//SubNet：设备只能在本地子网内通信，无法与外部网络通信。
//LocalNetwork：设备可以与本地网络中的其他设备通信，但无法访问互联网。
//Internet：设备可以访问互联网。

NetworkInfo parseSingleNetworkInfo(const QStringList &lines, int &index) {
    NetworkInfo info;

    for (; index < lines.size(); ++index) {
        QString line = lines[index].trimmed();

        QStringList parts = line.split(':');
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            if (key == "InterfaceIndex") {
                info.interfaceIndex = value.toInt();
            } else if (key == "NetworkCategory") {
                info.networkCategory = value;
            } else if (key == "IPv4Connectivity") {
                info.ipv4Connectivity = value;
                break;
            }
        }
    }
    return info;
}

ConnectManager::ConnectManager(QObject *parent):  QObject(parent)
{
    //test();
    //startTimer(1000);

    m_powerShellCmd = new QProcess(this);
    connect(m_powerShellCmd , &QProcess::readyReadStandardOutput , this , &ConnectManager::on_readoutput);
    testNetworkCategory();
}

void ConnectManager::startUDPBroadCast(int port)
{
    m_udpSocket = new QUdpSocket();
    m_udpSocket->bind(QHostAddress::Broadcast, port);

    m_appData.insert("name",SOFT_NAME);
    m_appData.insert("version",PFDMVersion);
    m_appData.insert("maichineName", QDir::home().dirName());
    startTimer(1000);
}

void ConnectManager::test()
{

    quint16 port = 9943; // 任意选择的端口号
    m_group.setAddress("239.255.255.250");

    // 绑定到所有网络接口上的指定端口
    if (!m_socket.bind(QHostAddress::AnyIPv4, port)) {
        qDebug() << "Failed to bind to port:" << m_socket.errorString();
        return;
    }

    // 加入广播组
    if (!m_socket.joinMulticastGroup(m_group)) {
        qDebug() << "Failed to join multicast group:" << m_socket.errorString();
        return;
    }
}

void ConnectManager::timerEvent(QTimerEvent *)
{
    QJsonDocument jsonDocument(m_appData);
    QNetworkDatagram datagram(jsonDocument.toJson(QJsonDocument::Compact), QHostAddress::Broadcast, 9943); // 广播数据到所有主机
    m_udpSocket->writeDatagram(datagram);

//    if(socket.pendingDatagramSize() < 1)
//        return;

//    QByteArray datagram;
//    datagram.resize(socket.pendingDatagramSize());

//    QHostAddress sender;
//    quint16 senderPort;

//    // 读取数据报
//    socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);


//    // 输出接收到的数据
//    qDebug() << "Received datagram:" << datagram.length()
    //             << "from" << sender.toString() << ":" << senderPort;
}

void ConnectManager::on_readoutput()
{
    QString msg = QString::fromLocal8Bit(m_powerShellCmd->readAllStandardOutput().data());
    //qInfo() << "powershellcmd res:" << msg;

    QList<NetworkInfo> infos;
    QStringList lines = msg.split("\r\n");
    int index = 0;

    int len = lines.size() - (lines.size() % 3);

    while (index < len) {

        NetworkInfo info = parseSingleNetworkInfo(lines, index);
        infos.append(info);
        ++index;
    }

    for(auto &info: infos)
    {
        if(info.networkCategory == "Public")
        {
            if(info.ipv4Connectivity == "Internet" || info.ipv4Connectivity == "LocalNetwork" ||info.ipv4Connectivity == "SubNet")
            {
                emit networkCategoryError();
                break;
            }
        }
    }
}

void ConnectManager::testNetworkCategory()
{
     m_powerShellCmd->start("powershell", QStringList()<< "/c"<< "Get-NetConnectionProfile | findstr /c:InterfaceIndex /c:NetworkCategory /c:IPv4Connectivity");
}
