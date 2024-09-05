#ifndef CONNECTMANAGER_H
#define CONNECTMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QProcess>


class ConnectManager: public QObject
{

    Q_OBJECT
public:
    ConnectManager(QObject* parent = nullptr);

    void startUDPBroadCast(int port);
    void test();

    void send(QByteArray msg);

    void SetWhenConnectCloseBroadCast();
    

signals:
    void networkCategoryError();

protected:
    void timerEvent(QTimerEvent *event) override;

    void on_readoutput();
    void testNetworkCategory();

private:
    QUdpSocket* m_udpSocket;
    QJsonObject m_appData;

    QUdpSocket m_socket;
    QHostAddress m_group;

    QProcess* m_powerShellCmd;
};

#endif // CONNECTMANAGER_H
