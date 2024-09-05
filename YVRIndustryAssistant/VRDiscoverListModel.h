#ifndef VRDISCOVERLISTMODEL_H
#define VRDISCOVERLISTMODEL_H

#include <QAbstractListModel>
#include <QSet>

struct YVRDiscoverDevice{
    QString sn;     // 设备SN
    QString id;     // 设备号
    QString group;
    bool hasSelect = false; // 是否连接
};


class VRDiscoverListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ getCount NOTIFY countChange)
public:
    explicit VRDiscoverListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void deviceChange(QString sn, bool offline, int index = -1);

    Q_INVOKABLE void select(QString sn, bool selected);
    Q_INVOKABLE void modifyDevice(QString sn, QString id, QString group);

    int getCount();
    QList<YVRDiscoverDevice> getAllDevices(bool all) const ;
    void clear(bool all);


protected:
    void timerEvent(QTimerEvent *event) override;

signals:
    void countChange();

private:
    QList<YVRDiscoverDevice> m_data;
    QSet<QString> m_deviceSn;
};

#endif // VRDISCOVERLISTMODEL_H
