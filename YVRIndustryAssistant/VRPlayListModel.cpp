#include "VRPlayListModel.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include "plugins/JsonFileParse.h"

VRPlayListModel::VRPlayListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void VRPlayListModel::load(QList<YVRPlayDevice> &data, int count)
{
    m_selectCount = count;
    selectCountChange();
    beginResetModel();
    m_data = data;
    endResetModel();
}

int VRPlayListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
           return 0;

     return m_data.size();
}

QVariant VRPlayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if(row < m_data.size() || row >= 0)
    {
        switch (role) {
        case Qt::UserRole:
            return m_data[row].sn;
        case Qt::UserRole + 1:
            return m_data[row].id;
        case Qt::UserRole + 2:
            return m_data[row].status;
        case Qt::UserRole + 3:
            return m_data[row].hasSelect;
        case Qt::UserRole + 4:
            return m_data[row].canSelect;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> VRPlayListModel::roleNames() const
{
    return
    {
        {Qt::UserRole, "vrSN"},
        {Qt::UserRole + 1, "vrID"},
        {Qt::UserRole + 2, "vrState"},
        {Qt::UserRole + 3, "vrSelected"},
        {Qt::UserRole + 4, "vrCanSelected"},
    };
}

void VRPlayListModel::select(int index, bool selected)
{
    if(!m_data[index].canSelect)
        return;

    if(selected)
        m_selectCount++;
    else
        m_selectCount--;

    selectCountChange();

    m_data[index].hasSelect = selected;
    QModelIndex topleft = createIndex(index, 3);
    QModelIndex bottomRight = createIndex(index,3);
    dataChanged(topleft, bottomRight);
}

QStringList VRPlayListModel::deviceSNs(bool selectAll)
{
    QStringList sns;
    for(auto &item : m_data)
    {
        if(item.canSelect)
        {
            if(selectAll || item.hasSelect)
            {
                sns.append(item.sn);
            }
        }
    }

    return  sns;
}

QString VRPlayListModel::getSelectCount()
{
    return QString("选择设备(%1)").arg(m_selectCount);
}

