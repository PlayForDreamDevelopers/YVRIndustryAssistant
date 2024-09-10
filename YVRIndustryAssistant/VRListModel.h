#ifndef VRLISTMODEL_H
#define VRLISTMODEL_H

#include <QAbstractListModel>
#include "DefineData.h"



struct YVRDevice{
    QString sn;     // 设备SN
    QString id;     // 设备号
    QString group;     // 设备号
    int video = -2;     // 绑定视频
    bool offline = true; // 设备状态
    bool onuse = false;
    bool controled = false;
    int playStatus = 1;
    DeviceType type  = DeviceType::YVR_D2;
    bool hasSelect = false; // 是否连接
    int battery = 50;
//    int left = 0;
//    int right = 0;
    //QWebSocket* socket; // 连接句柄
};
QT_FORWARD_DECLARE_CLASS(VRPlayListModel)

class VRListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString onlineAndAll READ getOnlineAndAll NOTIFY onlineCountChange)
    Q_PROPERTY(QStringList groups READ getGroups NOTIFY groupsChange)

public:
    explicit VRListModel(QObject *parent = nullptr);

    void add(QString sn, QString group, QString id);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void deleteDevices(bool all);

    Q_INVOKABLE void deleteDevice(QString sn);

    Q_INVOKABLE void modifyDevice(QString sn, QString id, QString group);

    Q_INVOKABLE void modiftyGroup(bool add, QString group);

    Q_INVOKABLE void selectGroup(QString group);

    Q_INVOKABLE void select(QString sn, bool selected);

    Q_INVOKABLE QVariant getPlayVRList(int index);

    void doSelect(QString sn , bool is);

    void updateVideo(QString sn, int index);

    QStringList vrList() const;

    QStringList vrPlayingList(int index);

    QStringList vrPlayList( bool selectAll) const;

    QStringList selectList(bool all);
    
    QString getOnlineAndAll() const;

    QStringList getGroups();

    void save();
    void refresh(bool reset= false);
    void clear();
    bool contains(QString sn);

signals:
    void onlineCountChange();
    void groupsChange();

public slots:
   void statesChange(QString sn,  bool offline, bool onuse, bool controled, int playStatus);
   void infoChange(QString sn,  QJsonObject& data);

private:
   void calOnlineCount(int count);
   void Init(QJsonArray& snArr);

private:
    QMap<QString, YVRDevice> m_dataMap;
    QList<YVRDevice> m_data;
    int m_onlineCount = 0;
    QStringList m_groups;

    VRPlayListModel* m_pVRPlayList = nullptr;
};

#endif // VRLISTMODEL_H
