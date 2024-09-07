#include "Settings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QUrl>
#include "plugins/JsonFileParse.h"

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
    m_control = obj["control"].toBool(false);

    file.close();
}


Settings &Settings::Instance()
{
    static Settings ins(nullptr);
    return ins;
}

void Settings::load()
{
    auto list = JsonFileParse::ins()->videoGroups();

    for(int i = 0; i < list.size(); ++i)
    {
        QJsonArray data = JsonFileParse::ins()->localFileList(i);

        QList<FileInfo> _data;

        for(QJsonValue item : data)
        {
            auto itemObj = item.toObject();
            FileInfo info;
            info.path = itemObj["path"].toString();
            info.file = itemObj["fileName"].toString();
            info.poster = itemObj["poster"].toString();

            info.realFile = itemObj["realFile"].toString();
            info.realPoster = itemObj["realPoster"].toString();

            info.md5 = itemObj["MD5"].toString();
            info.type = static_cast<YVRResourceType>(itemObj["type"].toInt());
            info.index = itemObj["index"].toInt();
            info.showName = itemObj["showName"].toString();
            info.describe = itemObj["describe"].toString();

            info.groups.clear();
            info.groups.append(list[i].toString());

            _data.append(info);
        }


        FileInfo info;
        info.type = YVRResourceType::UndeifneFile;
        info.realPoster = "qrc:/res/image/yvr_import_resource.png";
        _data.append(info);
        m_fileInfos[i] = std::move(_data);
    }
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

    if(m_allIndex.values().contains(id.toInt(0)))
    {
        return  false;
    }

    m_allIndex[sn] = id.toInt(0);

    return true;
}

QList<FileInfo> &Settings::getFileInfos(int index)
{
    if(!m_fileInfos.contains(index))
    {
        auto & res = m_fileInfos[index];
        FileInfo info;
        info.type = YVRResourceType::UndeifneFile;
        info.realPoster = "qrc:/res/image/yvr_import_resource.png";
        res.append(info);
        return res;
    }

    return m_fileInfos[index];
}

void Settings::removeGroup(int index)
{
    if(m_fileInfos.contains(index))
    {
        m_fileInfos[0].append(m_fileInfos[index]);
        m_fileInfos.remove(index);
    }
}

void Settings::getShowFileInfos(QList<ShowFileInfo> &showinfos)
{

    for(auto &item : m_fileInfos)
    {
        for(auto & info : item)
        {

            if(info.type != YVRResourceType::UndeifneFile)
            {
                ShowFileInfo showInfo;
                showInfo.poster = info.realPoster;
                showInfo.realFile = info.realFile;
                showInfo.id = info.index;
                showInfo.type = info.type;
                showInfo.showName = info.showName;
                showinfos.append(showInfo);
            }
        }
    }
}

void Settings::deployment(QString desPath)
{
    QDir dir(desPath);
    if(dir.exists())
    {
        dir.removeRecursively();
    }

    dir.mkpath(desPath);
    QJsonArray arr;

    QString logo = JsonFileParse::ins()->getLogo();

    if(!logo.isEmpty())
    {
        if(logo.contains("%"))
        {
            logo = QUrl::fromPercentEncoding(logo.remove("file:///").toUtf8());
        }

        QFileInfo file(logo);
        auto desFile = QString("%1/%2").arg(desPath).arg(file.fileName());
        if(file.exists() && !QFile::exists(desFile))
        {
            QFile::copy(logo, desFile);
        }
        logo = file.fileName();

    }

    int index = 1000;

    for(auto &item : m_fileInfos)
    {
        QList<FileInfo> & _data = item;
        for(int i = 0; i< _data.size(); ++i)
        {

            auto& info = _data[i];
            if(info.type == UndeifneFile)
                continue;

            QString path = info.realFile;

            if(path.contains("%"))
            {
                path = QUrl::fromPercentEncoding(path.remove("file:///").toUtf8());
            }else{
                path.remove("file:///");
            }

            QJsonObject obj;
            QFileInfo file(path);

            QString fileName = (_data[i].type > 3 ?"video": "image") + QString::number(index);

            if(!dir.exists(desPath + "/" +fileName))
            {
                dir.mkpath(desPath + "/" +  fileName);
            }

            if(file.exists())
            {
                obj["fileName"]=  QString("%1.%2").arg(fileName).arg(file.suffix());
                auto desFile = QString("%1/%2/%2.%3").arg(desPath).arg(fileName).arg(file.suffix());
                QFile::copy(path, desFile);
            }else
            {
                obj["fileName"] = "";
            }

            QString poster = info.realPoster;


            if(poster.contains("%"))
            {
                poster = QUrl::fromPercentEncoding(poster.remove("file:///").toUtf8());
            }else
            {
                poster.remove("file:///");
            }

            file.setFile(poster);
            if(file.exists())
            {
                obj["poster"] = QString("%1_poster.%2").arg(fileName).arg(file.suffix());
                auto desPoster= QString("%1/%2/%2_poster.%3").arg(desPath).arg(fileName).arg(file.suffix());
                QFile::copy(poster, desPoster);
            }else {
                obj["poster"] = "";
            }


            obj["MD5"]= info.md5;
            obj["path"] = fileName + "/";
            obj["index"] = index % 1000;
            obj["type"] = static_cast<int>(info.type);
            obj["describe"] = info.describe;
            obj["showName"] = info.showName;
            obj["groups"] = QJsonArray::fromStringList(info.groups);
            info.index = index % 1000;

            arr.append(obj);
            index++;
        }
    }

    JsonFileParse::ins()->deployment(arr, desPath, logo);
}

void Settings::deploymentLocal(QString desPath)
{
    JsonFileParse::ins()->setLocalFile(desPath);

    QDir dir(desPath);
    if(!dir.exists())
    {
        dir.mkpath(desPath);
    }

    QString logo = JsonFileParse::ins()->getLogo();

    if(!logo.isEmpty())
    {
        if(logo.contains("%"))
        {
            logo = QUrl::fromPercentEncoding(logo.remove("file:///").toUtf8());
        }

        QFileInfo file(logo);
        auto desFile = QString("%1/%2").arg(desPath).arg(file.fileName());
        if(file.exists() && !QFile::exists(desFile))
        {
            QFile::copy(logo, desFile);
        }

        JsonFileParse::ins()->setLogo(desFile);
    }


    QJsonArray files;
    for(auto &item : m_fileInfos)
    {
        QJsonArray itemArr;
        for(int i = 0; i< item.size(); ++i)
        {

            auto &info = item[i];
            if(info.type == UndeifneFile)
                continue;

            if(true)
            {
                QString path = info.realFile;

                if(path.contains("%"))
                {
                    path = QUrl::fromPercentEncoding(path.remove("file:///").toUtf8());
                }else{
                    path.remove("file:///");
                }


                QFileInfo file(path);

                auto desFile = QString("%1/%2").arg(desPath).arg(file.fileName());
                if(file.exists())
                {
                    if(!QFile::exists(desFile))
                        QFile::copy(path, desFile);
                    info.realFile = desFile;
                }


                QString poster = info.realPoster;


                if(poster.contains("%"))
                {
                    poster = QUrl::fromPercentEncoding(poster.remove("file:///").toUtf8());
                }else
                {
                    poster.remove("file:///");
                }

                file.setFile(poster);
                auto desPoster= QString("%1/%2").arg(desPath).arg(file.fileName());

                if(file.exists())
                {
                    if(!QFile::exists(desPoster))
                        QFile::copy(poster, desPoster);

                    info.realPoster = desPoster;
                }
            }

            QJsonObject obj;

            obj["MD5"]= info.md5;
            obj["index"] = info.index;
            obj["type"] = static_cast<int>(info.type);
            obj["describe"] = info.describe;
            obj["showName"] = info.showName;
            obj["groups"] = QJsonArray::fromStringList(info.groups);
            obj["realPoster"] = info.realPoster;
            obj["realFile"] = info.realFile;

            itemArr.append(obj);
        }

        files.append(itemArr);
    }

    JsonFileParse::ins()->deploymentLocal(files);
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

void Settings::clear()
{
    m_fileInfos.clear();
}
