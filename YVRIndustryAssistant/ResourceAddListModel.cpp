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
#include "Settings.h"

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

void ResourceAddListModel::load()
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
        m_logo = logo.filePath();

    m_name = JsonFileParse::ins()->getName();
    m_content = JsonFileParse::ins()->getContent();
}

void ResourceAddListModel::add(QString showName, QString desc, QString file, QString poster, int type, int groupIndex)
{
    FileInfo info;
    info.showName = showName;
    info.describe = desc;
    info.realFile = file.contains("file:///") ? file: "";
    info.realPoster = poster.contains("file:///") ? poster: "";
    info.type = static_cast<YVRResourceType>(type);

    QStringList groups;
    if(groupIndex >= 0)
    {
        groups.push_back(m_videoGroups[groupIndex]);
    }

    info.groups = groups;

    if(groupIndex != m_curGroupIndex)
    {
        auto &_data = Settings::Instance().getFileInfos(groupIndex);
        _data.insert(_data.size() -1, info);
        return;
    }

    auto index = m_data->size();
    qint64 size = 0;
    info.md5 = fileMd5(info.realFile, size);
    info.size = size;
    beginInsertRows(QModelIndex(), index - 1, index - 1);
    info.index = index - 1;
    m_data->insert(index - 1, info);
    endInsertRows();
}

void ResourceAddListModel::modify(int index, QString showName, QString desc, QString file, QString poster, int type, int groupIndex)
{
    FileInfo info;
    info.showName = showName;
    info.describe = desc;
    info.type = (YVRResourceType)type;
    info.realFile = file.contains("file:///") ? file: "";
    info.realPoster = poster.contains("file:///") ? poster: "";

    QStringList groups;
    if(groupIndex >= 0)
    {
        groups.push_back(m_videoGroups[groupIndex]);
    }

    info.groups = groups;

    qint64 size = 0;
    info.md5 = fileMd5(info.realFile, size);
    info.size = size;
    (*m_data)[index] = info;
    auto start_index = createIndex(index, 0);
    auto end_index = createIndex(index, 0);
    dataChanged(start_index, end_index);
}

void ResourceAddListModel::deletebyIndex(int index)
{
    int pos = -1;
    for (int i = 0; i < m_data->size(); ++i) {
        if ((*m_data)[i].index == index) {
            pos = i;
            break;
        }
    }

    beginRemoveRows(QModelIndex(), pos, pos);
    m_data->removeAt(pos);
    endRemoveRows();
}

void ResourceAddListModel::move(int from, int to)
{
    if(from == to)
        return;

    m_data->move(from, to);


//    for (int i = 0; i < m_data->size(); ++i) {
//        qInfo() << (*m_data)[i].showName;
//    }

//    auto start_index = createIndex(from, 0);
//    auto end_index = createIndex(to, 10);
//    dataChanged(start_index, end_index);
}

bool ResourceAddListModel::getInit()
{
    return  !m_name.isEmpty();
}

void ResourceAddListModel::clear()
{
    m_logo.clear();
    m_content.clear();
    m_name.clear();
    m_videoGroups.clear();
    m_videoGroups.append("默认");
    contentChange();
    logoChange();
    videoGroupsChange();
    nameChange();

    JsonFileParse::ins()->setLogo("");
    JsonFileParse::ins()->updateGroups(m_videoGroups, m_name, m_content);
}

void ResourceAddListModel::modifyGroup(int index, QString group)
{
    m_videoGroups[index] = group;
    videoGroupsChange();
    JsonFileParse::ins()->updateGroups(m_videoGroups, m_name, m_content);
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
        Settings::Instance().removeGroup(index);
    }

    videoGroupsChange();
    JsonFileParse::ins()->updateGroups(m_videoGroups, m_name, m_content);

    return m_videoGroups.size() - 1;
}

void ResourceAddListModel::showGroup(int index)
{
    m_curGroupIndex = index;
    beginResetModel();
    m_data = &Settings::Instance().getFileInfos(index);
    endResetModel();
}

void ResourceAddListModel::update()
{
    auto start_index = createIndex(0, 0);
    auto end_index = createIndex(m_data->size() -1, 10);
    dataChanged(start_index, end_index);
}

int ResourceAddListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return  m_data->size();
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

void ResourceAddListModel::save()
{
    JsonFileParse::ins()->updateGroups(m_videoGroups, m_name, m_content);
    Settings::Instance().deploymentLocal(m_localFileLocation);
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
    JsonFileParse::ins()->setLogo(path);
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

    QList<FileInfo> & tmp = *m_data;

    if(row < m_data->size() || row >= 0)
    {
        switch (role) {
        case Qt::UserRole:
            return tmp[row].realFile == ""  ? "" :  "file:///" + tmp[row].realFile;
        case Qt::UserRole + 1:
            return tmp[row].showName;
        case Qt::UserRole + 2:
            return tmp[row].describe;
        case Qt::UserRole + 3:
        {
            QString poster = tmp[row].realPoster;
            if(poster.contains("qrc:/res") || poster.contains("file:///"))
                return poster;

            return poster == "" ? "qrc:/res/image/yvr_defualtposter.png" :  "file:///" + tmp[row].realPoster;
        }
        case Qt::UserRole + 4:
        {
            auto type = tmp[row].type;

            return (type > 3 && type < 7 )|| type == 0 ?  "2D" : "3D";
        }
        case Qt::UserRole + 5:
        {
            auto type = tmp[row].type;
            return type > 3;
        }
        case Qt::UserRole + 6:
        {
            auto type = tmp[row].type;
            return type != UndeifneFile;
        }
        case Qt::UserRole + 7 :
        {
            int type = (int)tmp[row].type;
            return type;
        }
        case Qt::UserRole + 8 :
        {
            auto size = tmp[row].size;
            return transitionMem(size);
        }
        case Qt::UserRole + 9 :
        {
            return tmp[row].index;
        }
        case Qt::UserRole + 10 :
        {
            auto & groups = tmp[row].groups;

            for(int i = 0; i < m_videoGroups.size();++i)
            {
                if(groups.contains(m_videoGroups[i]))
                {
                    return  i;
                }
            }

            return -1;
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
        {Qt::UserRole + 9, "fileIndex"},
        {Qt::UserRole + 10, "groupIndex"}
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
    load();
}


