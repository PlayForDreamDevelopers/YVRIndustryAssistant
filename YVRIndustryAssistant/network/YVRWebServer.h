#ifndef YVRWEBSERVER_H
#define YVRWEBSERVER_H

#include <QObject>
#include <QMap>
#include <QAbstractSocket>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)


enum  VRMsgType{

  // VR端
  HeartBeat = 1000, //心跳包
  DeviceInformation = 1001, // 设备信息
  RequestDeviceStatus = 1002, // 请求设备展示状态
  ReportError = 1003, // 报告错误

  // PC端
  OnLine = 2000, // 在线检测
  DeviceStatus = 2001, // 获取设备状态
  StartPlay = 2002, // 开始播放Stop
  StopPlay = 2003, // 停止播放
  Controlled = 2004,  // 解除受控
  PlayListChange = 2005, // 播放列表改变
  AutonomousChange = 2006, // 无人值守配置更改
  StartAutonomous = 2007, // 无人值守开启
  RebootN = 2008, // 重启
  RebootP = 2009, // 关机

  // 未定义
  UndefineMsg = 9999

};

class YVRWebServer : public QObject
{
    Q_OBJECT
public:
    explicit YVRWebServer(quint16 port, QObject *parent = nullptr);

    void sendCommand(QByteArray &data, QString sn = "");
    void close();

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError error);

signals:
    void deviceStateChange(QString sn, bool offline, bool onuse, bool controled, int playStatus);
    void deviceInfoChange(QString sn, QJsonObject& data);

private:
    void handleMsg(VRMsgType type, QJsonObject& data);

private:
    QMap<QString, QWebSocket*> m_clients;
    QWebSocketServer *m_pWebSocketServer;
};

#endif // YVRWEBSERVER_H
