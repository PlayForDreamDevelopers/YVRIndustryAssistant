#include "JsonFileParse.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QStandardPaths>
#include <QUuid>
#include <version.h>
#include <QFileInfo>

JsonFileParse::JsonFileParse()
{
    m_appLocalDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    loadFile();
}

JsonFileParse * JsonFileParse::ins()
{
    static JsonFileParse instace;
    return  &instace;
}

void JsonFileParse::loadFile()
{
    QFile file(QString(m_appLocalDataLocation + "/config.dat"));
    if(file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if(parseError.error != QJsonParseError::NoError)
        {
            qInfo()<< "config file parseError";
            file.close();
        }else
        {
            QJsonObject obj = doc.object();
            file.close();

            m_fileList = obj["list"].toArray();
            m_videoGroups = obj["groups"].toArray();
            obj.remove("list");
            obj.remove("groups");

            m_baseInfo = std::move(obj);
        }

    }


    file.setFileName(QString(m_appLocalDataLocation + "/configForDevices.dat"));
    if(file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if(parseError.error != QJsonParseError::NoError)
        {
            qInfo()<< "config file parseError";
            file.close();
            return;
        }

        QJsonObject obj = doc.object();
        file.close();

        m_vrList = obj["vrList"].toArray();
        m_groups = obj["groups"].toArray();
    }
}

void JsonFileParse::updateFile(QJsonArray fileList)
{
    m_fileList = std::move(fileList);

//    QJsonDocument doc;
//    QJsonObject remote = m_baseInfo;
//    remote["list"] = m_fileList;
//    remote["groups"] = m_videoGroups;
//    QFile remoteFile(QString(m_appLocalDataLocation + "/config.dat"));

//    doc.setObject(remote);
//    remoteFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
//    remoteFile.write(doc.toJson());
//    remoteFile.close();
}

void JsonFileParse::updateDevice(QJsonArray& vrList, QStringList groups)
{
    m_vrList = std::move(vrList);
    m_groups = QJsonArray::fromStringList(groups);

    QJsonObject local;
    local["vrList"] = m_vrList;
    local["groups"] = m_groups;

    QJsonDocument doc;

    QFile file(QString(m_appLocalDataLocation + "/configForDevices.dat"));

    doc.setObject(local);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {

        file.write(doc.toJson());
        file.close();
    }
}

void JsonFileParse::setPort(int port)
{
    m_baseInfo["port"] = port;
}

void JsonFileParse::setLogo(QString path)
{
    m_baseInfo["logo"] = path;
}

void JsonFileParse::setLocalFile(QString path)
{
    m_baseInfo["localPath"] = path;
}

QString JsonFileParse::getLocalFile()
{
    return m_baseInfo["localPath"].toString();
}

QString JsonFileParse::getLogo()
{
    if(m_baseInfo.contains("logo"))
        return  m_baseInfo["logo"].toString();
    else
        return QString();
}

QString JsonFileParse::getName()
{
    if(m_baseInfo.contains("planName"))
        return  m_baseInfo["planName"].toString();
    else
        return QString();
}

QString JsonFileParse::getContent()
{
    if(m_baseInfo.contains("planContent"))
        return  m_baseInfo["planContent"].toString();
    else
        return QString();
}

void JsonFileParse::setPlayControl(bool autoControl)
{
     m_baseInfo["autoControl"] = autoControl;
}

bool JsonFileParse::getPlayControl()
{
    return m_baseInfo["autoControl"].toBool();
}

void JsonFileParse::deployment(QJsonArray fileList, QString path)
{
    QString uid = QUuid::createUuid().toString();
    m_baseInfo["UUID"] = uid;
    m_baseInfo["pcVersion"] = PFDMVersion;
    QJsonDocument doc;

    QJsonObject remote;
    remote["UUID"] = uid;
    remote["pcVersion"] = PFDMVersion;
    remote["logo"] = m_baseInfo["logo"].toString();
    remote["port"] = m_baseInfo["port"];
    remote["groups"] = m_videoGroups;
    remote["list"] = std::move(fileList);

    QFile remoteFile(QString(path + "/config.dat"));

    doc.setObject(remote);
    remoteFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    remoteFile.write(doc.toJson());
    remoteFile.close();
}

void JsonFileParse::updateLocal()
{
    QJsonDocument doc;

    QJsonObject local = m_baseInfo;
    local["list"] = m_fileList;
    local["groups"] = m_videoGroups;

    QFile file(QString(m_appLocalDataLocation + "/config.dat"));

    doc.setObject(local);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {

        file.write(doc.toJson());
        file.close();
    }
}

void JsonFileParse::updateGroups(QStringList groups, QString name, QString content)
{
    m_videoGroups = QJsonArray::fromStringList(groups);
    m_baseInfo["planName"] = name;
    m_baseInfo["planContent"] = content;
}

QJsonArray &JsonFileParse::fileList()
{
    return m_fileList;
}

QJsonArray &JsonFileParse::vrList()
{
    return m_vrList;
}

QJsonArray &JsonFileParse::groups()
{
    return m_groups;
}

QJsonArray &JsonFileParse::videoGroups()
{
    return m_videoGroups;
}

