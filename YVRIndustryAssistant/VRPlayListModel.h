#ifndef VRPlayListModel_H
#define VRPlayListModel_H

#include <QAbstractListModel>
#include "DefineData.h"




class VRPlayListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString selectCount READ getSelectCount NOTIFY selectCountChange)

public:
    explicit VRPlayListModel(QObject *parent = nullptr);

    void load(QList<YVRPlayDevice>& data, int count, int disable);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void select(int index, bool selected);
    Q_INVOKABLE int getDisable();


    QStringList deviceSNs(bool selectAll);
    QString getSelectCount();

 signals:
    void selectCountChange();

private:
    QList<YVRPlayDevice> m_data;
    int m_selectCount;
    int m_disable;
};

#endif // VRPlayListModel_H
