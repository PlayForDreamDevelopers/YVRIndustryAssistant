#ifndef VRMONITOR_H
#define VRMONITOR_H

#include <QObject>
#include <QMap>



QT_FORWARD_DECLARE_CLASS(VRListModel)
QT_FORWARD_DECLARE_CLASS(VRDiscoverListModel)
QT_FORWARD_DECLARE_CLASS(YVRWebServer)

class VRMonitor : public QObject
{
    Q_OBJECT
public:
    explicit VRMonitor(QObject *parent = nullptr);
    ~ VRMonitor();

    void stopPlay(int index);
    void startPlay(int index, QJsonObject data, bool selectAll);

    void uncontrol();
    void control();
    void modiftyDevice(QString sn, int id);

    void startAutonomous(QString sn);
    void rebootP(bool all);
    void rebootN(bool all);
    void rebootP(QString sn);
    void rebootN(QString sn);

    QVariant vrList() const;
    QVariant vrDiscoverList() const;

    void startWebsocket(int port);
    void refresh();

    void loadDevices(QString path);
    void addDiscoverDeice(bool all);

    QStringList snList();
    void clear();

    void save();

private:
    void sendMsg(QJsonObject &obj, QString sn) const;

public slots:
    void statesChange(QString sn,  bool offline, bool onuse, bool controled, int playStatus);
    void infoChange(QString sn,  QJsonObject& data);

private:
    VRListModel* m_pVRList = nullptr;
    VRDiscoverListModel* m_pVRListDiscover = nullptr;
    YVRWebServer* m_pWebServer = nullptr;
    bool m_control;
};

#endif // VRMONITOR_H
