#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMap>
#include <QObject>

class Settings: public QObject
{
public:
    static Settings& Instance();

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

    void save();
private:
    Settings(QObject* parent);
    bool m_firstPalyControl = true;
    bool m_firstAddDevice = true;
    bool m_control = true;
    QMap<QString, int> m_allIndex;
};

#endif // SETTINGS_H
