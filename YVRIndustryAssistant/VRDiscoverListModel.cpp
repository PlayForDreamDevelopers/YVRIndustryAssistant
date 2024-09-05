#include "VRDiscoverListModel.h"

#include <QTime>

VRDiscoverListModel::VRDiscoverListModel(QObject *parent) : QAbstractListModel(parent)
{
     //startTimer(2000);
}

int VRDiscoverListModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

QVariant VRDiscoverListModel::data(const QModelIndex &index, int role) const
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
            return m_data[row].group;
        case Qt::UserRole + 3:
            return m_data[row].hasSelect;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> VRDiscoverListModel::roleNames() const
{
    return
    {
        {Qt::UserRole, "vrSN"},
        {Qt::UserRole + 1, "vrID"},
        {Qt::UserRole + 2, "vrGroup"},
        {Qt::UserRole + 3, "vrSelect"},
    };
}

void VRDiscoverListModel::deviceChange(QString sn, bool offline, int index)
{
    if(m_data.size() > 10)
        return;

    if(offline)
    {
        if(!m_deviceSn.contains(sn))
            return;

        int pos = -1;
        for (int i = 0; i < m_data.size(); ++i) {
            if (m_data[i].sn == sn) {
                pos = i;
                break;
            }
        }

        beginRemoveRows(QModelIndex(), pos, pos);
        m_data.removeAt(pos);
        m_deviceSn.remove(sn);
        endRemoveRows();
    }else
    {
        if(m_deviceSn.contains(sn))
            return;

        beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
        YVRDiscoverDevice device;
        device.sn = sn;
        device.group = "默认";
        device.id = QString("%1").arg(index, 3, 10, QLatin1Char('0'));
        m_data.append(device);
        m_deviceSn.insert(sn);
        endInsertRows();
    }

    countChange();
}

void VRDiscoverListModel::select(QString sn, bool selected)
{
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].sn == sn) {
           m_data[i].hasSelect = selected;
           break;
        }
    }
}

void VRDiscoverListModel::modifyDevice(QString sn, QString id, QString group)
{
    int row = 0;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].sn == sn) {
           m_data[i].id = id;
           m_data[i].group = group;
           row = i;
           break;
        }
    }

    QModelIndex topleft = createIndex(row, 2);
    QModelIndex bottomRight = createIndex(row,3);
    dataChanged(topleft, bottomRight);
}

void VRDiscoverListModel::clear(bool all)
{
    beginResetModel();

    if(all)
    {
        m_data.clear();
        m_deviceSn.clear();
    }
    else
    {
        QList<YVRDiscoverDevice> _data;
        for (int i = 0; i < m_data.size(); ++i) {
            if (!m_data[i].hasSelect) {
                _data.append(m_data[i]);
            }else
            {
                m_deviceSn.remove(m_data[i].sn);
            }
        }

        m_data = _data;
    }

    endResetModel();
    countChange();
}

int VRDiscoverListModel::getCount()
{
    return m_data.size();
}

QList<YVRDiscoverDevice> VRDiscoverListModel::getAllDevices(bool all) const
{
    if(all)
    {
        return  m_data;
    }else
    {
        QList<YVRDiscoverDevice> _data;
        for (int i = 0; i < m_data.size(); ++i) {
            if (m_data[i].hasSelect) {
                _data.append(m_data[i]);
            }
        }

        return _data;
    }
}

void VRDiscoverListModel::timerEvent(QTimerEvent *event)
{
    deviceChange(QTime::currentTime().toString("qiwuiqeuqq_ss"), false, m_data.size() + 1);
}



