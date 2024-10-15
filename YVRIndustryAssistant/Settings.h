#ifndef SETTINGS_H
#define SETTINGS_H

#include <QJsonArray>
#include <QMap>
#include <QObject>
#include "DefineData.h"

class Settings: public QObject
{
public:
    static Settings& Instance();
    void load();

    void setFirstPlayControl(bool is);
    bool getFirstPlayControl();

    void setFirstAddDevice(bool is);
    bool getFirstAddDevice();

    void setControl(bool is);
    bool getControl();

    int getDeviceId(QString sn);
    bool containsDevice(QString sn);
    void removeDevices(QString sn);
    void modiftyDevices(QString sn, int id);
    bool verifyId(QString sn, QString id);

    QList<FileInfo> & getFileInfos(int index);
    void removeGroup(int index);
    void getShowFileInfos(QList<ShowFileInfo> & showinfos);

    void deployment(QString desPath);
    void deploymentLocal(QString desPath);

    void publishInfo(bool start, int index);

    void save();
    void clear();

private:
    Settings(QObject* parent);

    bool m_firstPalyControl = true;
    bool m_firstAddDevice = true;
    bool m_control = false;
    QJsonArray m_preTimesJson;
    QMap<QString, int> m_allIndex;
    QMap<int, QList<FileInfo>> m_fileInfos;
    QStringList m_preTimes;
};

#endif // SETTINGS_H
