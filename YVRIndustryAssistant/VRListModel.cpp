#include "VRListModel.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include "plugins/JsonFileParse.h"
#include "VRPlayListModel.h"
#include "Settings.h"


VRListModel::VRListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    Init(JsonFileParse::ins()->vrList());
}

void VRListModel::Init(QJsonArray& snArr)
{
    auto groups = JsonFileParse::ins()->groups();


    for(auto item : groups)
    {
        m_groups.append(item.toString());
    }

    if(m_groups.size() == 0)
        m_groups.append("默认");

    auto list = m_dataMap.keys();

    for(auto item: snArr)
    {
        auto info = item.toObject();
        if(list.contains(info["sn"].toString()))
            continue;

        YVRDevice device;
        device.sn = info["sn"].toString();
        device.group = info["group"].toString();
        device.video = info["video"].toInt();
        device.id = QString("%1").arg(info["index"].toInt(), 3, 10, QLatin1Char('0'));
        m_dataMap[device.sn] = device;
    }

    refresh(true);
}

void VRListModel::add(QString sn, QString group, QString id)
{
    YVRDevice device;
    device.sn = sn;
    device.group = group;
    device.id = id;
    device.video = -1;
    m_dataMap[device.sn] = device;
}

int VRListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
           return 0;

     return m_data.size();
}

QVariant VRListModel::data(const QModelIndex &index, int role) const
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
        {
            QStringList images;

            if(m_data[row].offline)
                return images;

            if(m_data[row].controled)
            {
                images.append("qrc:/res/image/yvr_control.png");
            }
            else
            {
                images.append("qrc:/res/image/yvr_uncontrol.png");
            }

            if(m_data[row].onuse)
            {
                images.append("qrc:/res/image/yvr_onuse.png");
            }
            else
            {
                images.append("qrc:/res/image/yvr_idle.png");
            }

            auto res = m_data[row].playStatus;
            if(res == 0)
            {
                images.append("qrc:/res/image/yvr_play.png");
            }
            else if(res == 1)
            {
                images.append("qrc:/res/image/yvr_pause.png");
            }else
            {
                images.append("qrc:/res/image/yvr_play_error.png");
            }

            return images;
        }
        case Qt::UserRole + 2:
            return !m_data[row].offline;
        case Qt::UserRole + 3:
            return m_data[row].id;
        case Qt::UserRole + 4:
            return m_data[row].group;
        case Qt::UserRole + 5:
            return !m_data[row].offline;
        case Qt::UserRole + 6:
            return m_data[row].controled;
        case Qt::UserRole + 7:
            return m_data[row].hasSelect;
        case Qt::UserRole + 8:
            return m_data[row].battery;
        case Qt::UserRole + 9:
            return static_cast<int>(m_data[row].type);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> VRListModel::roleNames() const
{
    return
    {
        {Qt::UserRole, "vrSN"},
        {Qt::UserRole + 1, "vrState"},
        {Qt::UserRole + 2, "vrHasConnect"},
        {Qt::UserRole + 3, "vrID"},
        {Qt::UserRole + 4, "vrGroup"},
        {Qt::UserRole + 5, "vrEnable"},
        {Qt::UserRole + 6, "vrControl"},
        {Qt::UserRole + 7, "vrSelect"},
        {Qt::UserRole + 8, "vrBattery"},
        {Qt::UserRole + 9, "vrType"},
    };
}

void VRListModel::deleteDevices(bool all)
{
    if(all)
    {
        for(auto & item : m_dataMap)
        {
            Settings::Instance().removeDevices(item.sn);
        }

        clear();
    }else
    {
        QStringList list;
        for(auto & item : m_dataMap)
        {
            if(item.hasSelect)
            {
                if(!item.offline)
                {
                    m_onlineCount--;
                }

                list.append(item.sn);
            }
        }

        for(auto sn: list){
            m_dataMap.remove(sn);
            Settings::Instance().removeDevices(sn);
        }

        beginResetModel();
        m_data = m_dataMap.values();
        endResetModel();
    }

    onlineCountChange();
}

void VRListModel::deleteDevice(QString sn)
{
    int index = -1;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].sn == sn) {
            index = i;
            break;
        }
    }

    if(!m_data[index].offline)
    {
        m_onlineCount--;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_data.removeAt(index);
    m_dataMap.remove(sn);
    endRemoveRows();

    Settings::Instance().removeDevices(sn);

    onlineCountChange();
}

void VRListModel::modifyDevice(QString sn, QString id,  QString group)
{
    m_dataMap[sn].id = id;
    m_dataMap[sn].group = group;
    int row = 0;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].sn == sn) {
            m_data[i].id = id;
            m_data[i].group = group;
            row = i;
            break;
        }
    }

    Settings::Instance().modiftyDevices(sn, id.toInt());

    QModelIndex topleft = createIndex(row, 3);
    QModelIndex bottomRight = createIndex(row,4);
    dataChanged(topleft, bottomRight);
}

void VRListModel::modiftyGroup(bool add, QString group)
{
    if(add)
    {
        if(!m_groups.contains(group))
            m_groups.append(group);
    }else
    {
        if(m_groups.contains(group))
            m_groups.removeOne(group);
    }

    groupsChange();
}

void VRListModel::selectGroup(QString group)
{
    if(group == "全部分组")
    {
        refresh(true);
    }else
    {
       QList<YVRDevice> _data;
       for(auto & item : m_dataMap)
        {
            if(item.group == group)
            {
                _data.append(item);
            }
        }

       beginResetModel();
       m_data = _data;
       endResetModel();
    }
}

void VRListModel::select(QString sn, bool selected)
{
    m_dataMap[sn].hasSelect = selected;
    int row = 0;
    for (int i = 0; i < m_data.size(); ++i) {
        if (m_data[i].sn == sn) {
            m_data[i].hasSelect = selected;
            row = i;
            break;
        }
    }

    QModelIndex topleft = createIndex(row, 7);
    QModelIndex bottomRight = createIndex(row,7);
    dataChanged(topleft, bottomRight);
}

QVariant VRListModel::getPlayVRList(int index)
{
    if(m_pVRPlayList == nullptr)
        m_pVRPlayList = new VRPlayListModel(this);

    QList<YVRPlayDevice> data;
    int count = 0;
    int disableCount = 0;
    for(auto & item : m_dataMap)
    {

        YVRPlayDevice device;
        device.id = item.id;
        device.sn = item.sn;
        device.canSelect = (item.playStatus != 0 || item.video == index) && !item.offline;

        if(item.offline)
        {
            device.status = "离线";
        }else {

            if(item.playStatus == 0)
                device.status = "播放中";
            else
            {
                if(!item.controled)
                    device.status = "自由";
                else
                    device.status = "待机";
            }
        }

        device.hasSelect = item.video == index;



        if(!device.canSelect)
            disableCount++;
        else if(device.hasSelect)
            count++;

        data.append(device);
    }

    m_pVRPlayList->load(data, count, disableCount);

    return QVariant::fromValue(m_pVRPlayList);
}


void VRListModel::doSelect(QString sn, bool is)
{
    if(m_dataMap.contains(sn))
    {
        m_dataMap[sn].hasSelect = is;
    }
}

void VRListModel::updateVideo(QString sn, int index)
{
    m_dataMap[sn].video = index;
}

QStringList VRListModel::vrList() const
{
    return  m_dataMap.keys();
}

QStringList VRListModel::vrPlayingList(int index)
{
    QStringList sns;
    for(auto &item : m_data)
    {
        if(item.video == index)
        {
           sns.append(item.sn);
           item.video = -2;
        }
    }

    for(auto sn : sns)
    {
        m_dataMap[sn].video = -2;
    }

    return  sns;
}

QStringList VRListModel::vrPlayList( bool selectAll) const
{
    return m_pVRPlayList->deviceSNs(selectAll);
}

QStringList VRListModel::selectList(bool all)
{
    QStringList list;
    for(auto & item : m_dataMap)
    {
        if((all || item.hasSelect))
        {
            list.append(item.sn);
        }
    }

    return  list;
}

QString VRListModel::getOnlineAndAll() const
{
    return  QString("<span style='color: #1E68FF;'>%1</span>/%2").arg(m_onlineCount).arg(m_dataMap.size());
}

QStringList VRListModel::getGroups()
{
    return m_groups;
}

bool compare(const YVRDevice &a, const YVRDevice &b)
{
    return b.id.toInt() >  a.id.toInt();
}


void VRListModel::refresh(bool reset)
{
    if(reset)
    {
        beginResetModel();
        m_data = m_dataMap.values();
        qSort(m_data.begin(), m_data.end(), compare);
        endResetModel();

    }else
    {
        m_data = m_dataMap.values();
        qSort(m_data.begin(), m_data.end(), compare);
        QModelIndex topleft = createIndex(0,0);
        QModelIndex bottomRight = createIndex(m_dataMap.size() - 1,7);
        dataChanged(topleft, bottomRight);
    }
}

void VRListModel::clear()
{
    beginResetModel();
    m_dataMap.clear();
    m_data.clear();
    endResetModel();

    m_onlineCount = 0;
    emit onlineCountChange();
}

bool VRListModel::contains(QString sn)
{
    return m_dataMap.contains(sn);
}

void VRListModel::statesChange(QString sn,  bool offline, bool onuse, bool controled, int playStatus)
{
    if(m_dataMap.contains(sn))
    {
        if( m_dataMap[sn].offline != offline)
        {
             calOnlineCount(offline ? -1: 1);
        }

        m_dataMap[sn].offline = offline;
        m_dataMap[sn].onuse = onuse;
        m_dataMap[sn].playStatus = playStatus;
        m_dataMap[sn].controled = controled;

        for(int i = 0; i < m_data.size(); ++i)
        {
            if(m_data[i].sn == sn)
            {
                m_data[i].offline = offline;
                m_data[i].onuse = onuse;
                m_data[i].playStatus = playStatus;
                m_data[i].controled = controled;
                QModelIndex topleft = createIndex(i,0);
                QModelIndex bottomRight = createIndex(i,7);
                dataChanged(topleft, bottomRight);

                break;
            }
        }

    }else{
        qInfo()<< "unknown device: " << sn;
    }
}

void VRListModel::infoChange(QString sn, QJsonObject &data)
{
    if(m_dataMap.contains(sn))
    {
        bool offline = data["deviceStatus"].toInt(0) == 0;
        if( m_dataMap[sn].offline != offline)
        {
             calOnlineCount(offline ? -1: 1);
        }

        m_dataMap[sn].onuse = data["onuse"].toInt(0) == 1;
        m_dataMap[sn].playStatus = data["playStatus"].toInt();
        m_dataMap[sn].controled = data["controled"].toInt() == 1;
        m_dataMap[sn].battery = data["battery"].toInt();
        m_dataMap[sn].offline = offline;

        for(int i = 0; i < m_data.size(); ++i)
        {
            if(m_data[i].sn == sn)
            {
                m_data[i].offline = offline;
                m_data[i].onuse = m_dataMap[sn].onuse;
                m_data[i].playStatus = m_dataMap[sn].playStatus;
                m_data[i].controled = m_dataMap[sn].controled;
                m_data[i].battery = m_dataMap[sn].battery;

                QModelIndex topleft = createIndex(i,0);
                QModelIndex bottomRight = createIndex(i,7);
                dataChanged(topleft, bottomRight);

                break;
            }
        }

    }else{
        qInfo()<< "unknown device: " << sn;
    }
}

void VRListModel::calOnlineCount(int count)
{
    m_onlineCount += count;
    emit onlineCountChange();
}

void VRListModel::save()
{
    QJsonArray devices;
    for (auto item : m_dataMap.values())
    {
        QJsonObject device;
        device["sn"] = item.sn;
        device["group"] = item.group;
        device["index"] = item.id.toInt();
        device["video"] = item.video;
        devices.append(device);
    }

    JsonFileParse::ins()->updateDevice(devices, m_groups);
}

