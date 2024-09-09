#ifndef RESOURCELISTMODEL_H
#define RESOURCELISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "DefineData.h"

class ResourceListModel:  public QAbstractListModel
{
    Q_OBJECT

public:
    ResourceListModel(QObject *parent = nullptr);

    void load();

    void clear();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    void publish(int id);
    void stoppublish(int id);

    bool getHasPublish();

protected:

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;
private:
    QList<ShowFileInfo> m_data;
};

#endif // RESOURCELISTMODEL_H
