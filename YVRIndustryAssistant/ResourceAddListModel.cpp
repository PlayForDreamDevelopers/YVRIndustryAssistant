#include "ResourceAddListModel.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFile>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QTime>
#include <QDebug>
#include "plugins/JsonFileParse.h"
#include "plugins/utils.h"

QString transitionMem(qint64 byte)
{
    byte = byte / (1 << 10);

    if(byte < (1 << 10))
    {
        return QString::number(byte) + "KB";
    }
    else if(byte < (1 << 20))
    {
        double kb = byte *1.0 / (1 << 10);
        return QString::number(kb, 'f', 2) + "MB";
    }
    else if(byte < (1 << 30))
    {
        double mb = byte *1.0 / (1 << 20);
        return QString::number(mb, 'f', 2 ) + "GB";
    }
    else
    {
        double gb = byte *1.0 / (1 << 30);
        return QString::number(gb, 'f', 2) + "TB";
    }
}

void ResourceAddListModel::load(const QJsonArray &data)
{
    beginResetModel();
    m_data.clear();
    for(QJsonValue item : data)
    {
        auto itemObj = item.toObject();

        QString path = QString("file:///%1/res/").arg(m_appLocalDataLocation);
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

        for(auto group : itemObj["groups"].toArray())
        {
            auto name = group.toString();
            if(!info.groups.contains(name))
                info.groups.append(name);
        }

        m_data.append(info);
    }


    m_name = JsonFileParse::ins()->getName();
    m_content = JsonFileParse::ins()->getContent();

    FileInfo info;
    info.type = YVRResourceType::UndeifneFile;
    info.realPoster = "qrc:/res/image/yvr_import_resource.png";
    m_data.append(info);
    endResetModel();
}

void ResourceAddListModel::add(QString showName, QString desc, QString file, QString poster, int type, QList<bool> select)
{
    FileInfo info;
    info.showName = showName;
    info.describe = desc;
    info.realFile = file.contains("file:///") ? file: "";
    info.realPoster = poster.contains("file:///") ? poster: "";
    info.type = static_cast<YVRResourceType>(type);

    QStringList groups;
    for(int i = 0; i < m_videoGroups.size();++i)
    {
        if(select[i])
        {
            groups.push_back(m_videoGroups[i]);
        }
    }

    info.groups = groups;

    auto index = m_data.size();
    qint64 size = 0;
    info.md5 = fileMd5(info.realFile, size);
    info.size = size;
    beginInsertRows(QModelIndex(), index - 1, index - 1);
    info.index = index - 1;
    m_data.insert(index - 1, info);
    endInsertRows();
}

void ResourceAddListModel::modify(int index, QString showName, QString desc, QString file, QString poster, int type, QList<bool> select)
{
    FileInfo info;
    info.showName = showName;
    info.describe = desc;
    info.realFile = file.contains("file:///") ? file: "";
    info.realPoster = poster.contains("file:///") ? poster: "";

    QStringList groups;
    for(int i = 0; i < m_videoGroups.size();++i)
    {
        if(select[i])
        {
            groups.push_back(m_videoGroups[i]);
        }
    }

    info.groups = groups;

    qint64 size = 0;
    info.md5 = fileMd5(info.realFile, size);
    info.size = size;
    m_data[index] = info;
    auto start_index = createIndex(index, 0);
    auto end_index = createIndex(index, 0);
    dataChanged(start_index, end_index);
}

void ResourceAddListModel::deletebyIndex(int index)
{
    int pos = -1;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].index == index) {
            pos = i;
            break;
        }
    }

    beginRemoveRows(QModelIndex(), pos, pos);
    m_data.removeAt(pos);
    endRemoveRows();
}

void ResourceAddListModel::move(int from, int to)
{
    if(from == to)
        return;

    m_data.move(from, to);
//    auto start_index = createIndex(from, 0);
//    auto end_index = createIndex(to, 0);
//    dataChanged(start_index, end_index);
}

bool ResourceAddListModel::getInit()
{
    return  !m_name.isEmpty();
}

void ResourceAddListModel::fileArray(QJsonArray & arr, QString desPath)
{
    QDir dir(desPath);
    if(dir.exists())
    {
        dir.removeRecursively();
    }

    dir.mkpath(desPath);

    copyLogo(desPath, false);

    int index = 1000;
    for(int i = 0; i< m_data.size(); ++i)
    {

        auto& info = m_data[i];
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

        QString fileName = (m_data[i].type > 3 ?"video": "image") + QString::number(index);

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
        obj["index"] = i;
        obj["type"] = static_cast<int>(info.type);
        obj["describe"] = info.describe;
        obj["showName"] = info.showName;
        obj["groups"] = QJsonArray::fromStringList(info.groups);
        info.index = i;

        arr.append(obj);
        index++;
    }

    JsonFileParse::ins()->updateGroups(m_videoGroups, m_name, m_content);
}

void ResourceAddListModel::fileArrayLocal(QJsonArray & arr, bool copy)
{
    JsonFileParse::ins()->setLocalFile(m_localFileLocation);

    QDir dir(m_localFileLocation);
    if(!dir.exists())
    {
        dir.mkpath(m_localFileLocation);
    }

    copyLogo(m_localFileLocation, true);

    for(int i = 0; i< m_data.size(); ++i)
    {

        auto &info = m_data[i];
        if(info.type == UndeifneFile)
            continue;

        if(copy)
        {
            QString path = info.realFile;

            if(path.contains("%"))
            {
                path = QUrl::fromPercentEncoding(path.remove("file:///").toUtf8());
            }else{
                path.remove("file:///");
            }


            QFileInfo file(path);

            auto desFile = QString("%1/%2").arg(m_localFileLocation).arg(file.fileName());
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
            auto desPoster= QString("%1/%2").arg(m_localFileLocation).arg(file.fileName());

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

        arr.append(obj);
    }
}

FileInfo ResourceAddListModel::getbyIndex(int index)
{
    return  m_data[index];
}

void ResourceAddListModel::clear()
{
    m_logo.clear();
    m_content.clear();
    m_name.clear();
    m_videoGroups.clear();
    contentChange();
    logoChange();
    videoGroupsChange();
    nameChange();

    JsonFileParse::ins()->updateGroups(m_videoGroups, m_name, m_content);
    JsonFileParse::ins()->updateFile(QJsonArray());
    JsonFileParse::ins()->updateLocal();

    load(QJsonArray());
}

void ResourceAddListModel::modifyGroup(int index, QString group)
{
    m_videoGroups[index] = group;
    videoGroupsChange();
}


int ResourceAddListModel::addOrDelGroup(int index, bool add)
{
    if(add)
    {
        QString group = "新增";
        while (m_videoGroups.contains(group))
        {
            group += "1";
        }

        m_videoGroups.append(group);

    }else
    {
        m_videoGroups.removeAt(index);
    }

    videoGroupsChange();

    return m_videoGroups.size() - 1;
}

QList<bool> ResourceAddListModel::getGroupSelect(int index)
{
    QList<bool> ans;

    int pos = -1;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].index == index) {
            pos = i;
            break;
        }
    }

    auto & groups = m_data[pos].groups;

    for(int i = 0; i < m_videoGroups.size();++i)
    {
        if(groups.contains(m_videoGroups[i]))
        {
            ans.push_back(true);
        }else
        {
            ans.push_back(false);
        }
    }

    return ans;
}

int ResourceAddListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return  m_data.size();
}

QString ResourceAddListModel::getLogo() const
{
    if(m_logo.isEmpty())
        return m_logo;

    return "file:///" + m_logo;
}

QStringList ResourceAddListModel::getVideoGroups()
{
    return  m_videoGroups;
}

void ResourceAddListModel::save(bool copy)
{
    JsonFileParse::ins()->updateGroups(m_videoGroups, m_name, m_content);

    QJsonArray  arr;
    fileArrayLocal(arr, copy);
    JsonFileParse::ins()->updateFile(arr);
    JsonFileParse::ins()->updateLocal();
}

void ResourceAddListModel::copyLogo(QString desPath, bool local)
{
    if(!m_logo.isEmpty())
    {

        QString logo = m_logo;

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

        if(local)
            JsonFileParse::ins()->setLogo(desFile);
        else
            JsonFileParse::ins()->setLogo(file.fileName());
    }else
    {
        JsonFileParse::ins()->setLogo("");
    }
}

void ResourceAddListModel::setLogo(QString path)
{

    if(path.contains("file:///"))
       path = path.remove("file:///");

    m_logo = path;
    logoChange();
}

QString ResourceAddListModel::getName() const
{
    return m_name;
}

void ResourceAddListModel::setName(QString name)
{
    m_name = name;
    nameChange();
}

QString ResourceAddListModel::getContent() const
{
    return m_content;
}

void ResourceAddListModel::setContent(QString content)
{
    m_content = content;
    contentChange();
}

QVariant ResourceAddListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if(row < m_data.size() || row >= 0)
    {
        switch (role) {
        case Qt::UserRole:
            return m_data[row].realFile == ""  ? "" :  "file:///" + m_data[row].realFile;
        case Qt::UserRole + 1:
            return m_data[row].showName;
        case Qt::UserRole + 2:
            return m_data[row].describe;
        case Qt::UserRole + 3:
        {
            QString poster = m_data[row].realPoster;
            if(poster.contains("qrc:/res") || poster.contains("file:///"))
                return poster;

            return poster == "" ? "qrc:/res/image/yvr_defualtposter.png" :  "file:///" + m_data[row].realPoster;
        }
        case Qt::UserRole + 4:
        {
            auto type = m_data[row].type;

            return (type > 3 && type < 7 )|| type == 0 ?  "2D" : "3D";
        }
        case Qt::UserRole + 5:
        {
            auto type = m_data[row].type;
            return type > 3;
        }
        case Qt::UserRole + 6:
        {
            auto type = m_data[row].type;
            return type != UndeifneFile;
        }
        case Qt::UserRole + 7 :
        {
            int type = (int)m_data[row].type;
            return type;
        }
        case Qt::UserRole + 8 :
        {
            auto size = m_data[row].size;
            return transitionMem(size);
        }
        case Qt::UserRole + 9 :
        {
            return m_data[row].index;
        }

        }
    }

    return QVariant();
}

QHash<int, QByteArray> ResourceAddListModel::roleNames() const
{
    return
    {
        {Qt::UserRole, "file"},
        {Qt::UserRole + 1, "showName"},
        {Qt::UserRole + 2, "describe"},
        {Qt::UserRole + 3, "poster"},
        {Qt::UserRole + 4, "type23D"},
        {Qt::UserRole + 5, "typeImageVideo"},
        {Qt::UserRole + 6, "hasItem"},
        {Qt::UserRole + 7, "typeIndex"},
        {Qt::UserRole + 8, "fileSize"},
        {Qt::UserRole + 9, "fileIndex"}
    };
}

QString ResourceAddListModel:: fileMd5(QString sourceFilePath, qint64 &size)
{

    if(sourceFilePath.contains("%"))
    {
         sourceFilePath = QUrl::fromPercentEncoding(sourceFilePath.toUtf8());
    }

    QFile sourceFile(sourceFilePath.remove("file:///"));
    qint64 fileSize = sourceFile.size();
    size = fileSize;

    QCryptographicHash::hash(QString::number(fileSize).toUtf8(), QCryptographicHash::Md5);

 // 文件MD5
//    const qint64 bufferSize = 10240;

//    if (sourceFile.open(QIODevice::ReadOnly)) {
//        char buffer[bufferSize];
//        int bytesRead;
//        int readSize = qMin(fileSize, bufferSize);

//        QCryptographicHash hash(QCryptographicHash::Md5);

//        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
//            fileSize -= bytesRead;
//            hash.addData(buffer, bytesRead);
//            readSize = qMin(fileSize, bufferSize);
//        }

//        sourceFile.close();
//        return QString(hash.result().toHex());
//    }

  return QString(QCryptographicHash::hash(QString::number(fileSize).toUtf8(), QCryptographicHash::Md5).toHex());
}

ResourceAddListModel::ResourceAddListModel(QObject *parent)
    :QAbstractListModel(parent)
{

    if(JsonFileParse::ins()->getLocalFile().isEmpty())
    {
        m_localFileLocation =  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                + "/YVRIndustryTemp";

        QDir path("D:/");
        if(path.exists() && !IsRemovableDrive(L"D:/"))
        {
            m_localFileLocation = "D:/YVRIndustryTemp";
        }

    }else
    {
        m_localFileLocation = JsonFileParse::ins()->getLocalFile();
    }


    m_appLocalDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    for(auto item : JsonFileParse::ins()->videoGroups())
    {
        m_videoGroups.append(item.toString());
    }

    if(m_videoGroups.size() == 0)
    {
        m_videoGroups.append("默认");
    }

    QFileInfo logo(JsonFileParse::ins()->getLogo());

    if(logo.exists() && logo.isFile())
        m_logo = logo.path();
}


