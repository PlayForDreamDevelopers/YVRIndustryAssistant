#include "Settings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDebug>

Settings::Settings(QObject *parent): QObject(parent)
{
    QString path =  QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    QFile file(QString(path + "/setting.ins"));
    if(!file.exists())
    {
        return;
    }

    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc=QJsonDocument::fromJson(data, &parseError);

    if(parseError.error != QJsonParseError::NoError)
    {
        qInfo()<< "config file parseError";
        file.close();
        return;
    }

    QJsonObject obj = doc.object();

    m_firstAddDevice = obj["firstAddDevice"].toBool(true);
    m_firstPalyControl = obj["firstPalyControl"].toBool(true);
    m_control = obj["control"].toBool(true);

    file.close();
}


Settings &Settings::Instance()
{
    static Settings ins(nullptr);
    return ins;
}

void Settings::setFirstPlayControl(bool is)
{
    m_firstPalyControl = is;
    save();
}

bool Settings::getFirstPlayControl()
{
    return  m_firstPalyControl;
}

void Settings::setFirstAddDevice(bool is)
{
    m_firstAddDevice = is;
    save();
}

bool Settings::getFirstAddDevice()
{
    return m_firstAddDevice;
}

void Settings::setControl(bool is)
{
    m_control = is;
    save();
}

bool Settings::getControl()
{
    return m_control;
}

int Settings::getDeviceId(QString sn)
{
    if(m_allIndex.contains(sn))
        return m_allIndex[sn];

    int index = 1;
    auto indexList = m_allIndex.values();
    while (indexList.contains(index)) {
        index++;
    }

    m_allIndex[sn] = index;
    return index;
}

bool Settings::containsDevice(QString sn)
{
    return m_allIndex.contains(sn);
}

void Settings::removeDevices(QString sn)
{
    m_allIndex.remove(sn);
}

void Settings::modiftyDevices(QString sn, int id)
{
    m_allIndex[sn] = id;
}

bool Settings::verifyId(QString sn, QString id)
{
    if(m_allIndex[sn] == id.toInt(0))
        return true;

    if(id.length() > 3)
        return false;

    if(m_allIndex.contains(id))
    {
        return  false;
    }

    m_allIndex[sn] = id.toInt(0);

    return true;
}

void Settings::save()
{
    QString path =  QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    QFile remoteFile(QString(path + "/setting.ins"));
    QJsonDocument doc;
    QJsonObject obj;
    obj["firstAddDevice"] = m_firstAddDevice;
    obj["firstPalyControl"] = m_firstPalyControl;
    obj["control"] = m_control;

    doc.setObject(obj);
    remoteFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    remoteFile.write(doc.toJson());
    remoteFile.close();
}
