#include "ResourceListModel.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFile>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QTime>
#include "plugins/JsonFileParse.h"
#include "Settings.h"

void ResourceListModel::load()
{
    beginResetModel();
    m_data.clear();
    Settings::Instance().getShowFileInfos(m_data);
    endResetModel();
}

void ResourceListModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

int ResourceListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return  m_data.size();
}

void ResourceListModel::publish(int id)
{
    int pos = -1;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].id == id) {
            pos = i;
            break;
        }
    }

    m_data[pos].prePlayTime = QDateTime::currentDateTime().toString("yyyyMMdd hh:mm");

    auto start_index = createIndex(pos, 4);
    auto end_index = createIndex(pos, 4);
    dataChanged(start_index, end_index);
}

void ResourceListModel::stoppublish(int id)
{
    int pos = -1;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].id == id) {
            pos = i;
            break;
        }
    }

    m_data[pos].prePlayTime.clear();

    auto start_index = createIndex(pos, 4);
    auto end_index = createIndex(pos, 4);
    dataChanged(start_index, end_index);
}

bool ResourceListModel::getHasPublish()
{
    for (int i = 0; i < m_data.size(); ++i) {
        if (!m_data[i].prePlayTime.isEmpty())
            return true;
    }

    return false;
}

QVariant ResourceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if(row < m_data.size() || row >= 0)
    {
        switch (role) {
        case Qt::UserRole:
            return m_data[row].showName;
        case Qt::UserRole + 1:
            if(m_data[row].poster.contains("qrc:/res") || m_data[row].poster.contains("file:///"))
                return m_data[row].poster;
            return m_data[row].poster.isEmpty() ? "qrc:/res/image/yvr_defualtposter.png" : "file:///" + m_data[row].poster;
        case Qt::UserRole + 2:
        {
            auto type = m_data[row].type;

            return (type > 3 && type < 7 )|| type == 0 ?  "2D" : "3D";
        }
        case Qt::UserRole + 3:
        {
            auto type = m_data[row].type;
            return type > 3;
        }
        case Qt::UserRole + 4:
        {
            return m_data[row].prePlayTime;
        }
        case Qt::UserRole + 5:
        {
            return m_data[row].id;
        }
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ResourceListModel::roleNames() const
{
    return
    {
        {Qt::UserRole, "showName"},
        {Qt::UserRole + 1, "poster"},
        {Qt::UserRole + 2, "type23D"},
        {Qt::UserRole + 3, "typeImageVideo"},
        {Qt::UserRole + 4, "playTime"},
        {Qt::UserRole + 5, "fileIndex"},
    };
}

ResourceListModel::ResourceListModel(QObject *parent)
    :QAbstractListModel(parent)
{
}


