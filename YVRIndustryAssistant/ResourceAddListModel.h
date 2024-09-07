#ifndef ResourceAddListModel_H
#define ResourceAddListModel_H

#include <QAbstractListModel>
#include <QObject>
#include "DefineData.h"

class ResourceAddListModel:  public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString logo READ getLogo WRITE setLogo NOTIFY logoChange)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChange)
    Q_PROPERTY(bool init READ getInit NOTIFY nameChange)
    Q_PROPERTY(QString content READ getContent WRITE setContent NOTIFY contentChange)
    Q_PROPERTY(QStringList videoGroups READ getVideoGroups NOTIFY videoGroupsChange)

public:
    ResourceAddListModel(QObject *parent = nullptr);

    void load();

    Q_INVOKABLE void deletebyIndex(int index);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void add(QString showName, QString desc, QString file, QString poster, int type, int groupIndex);
    Q_INVOKABLE void modify(int index, QString showName, QString desc, QString file, QString poster, int type, int groupIndex);
    Q_INVOKABLE void modifyGroup(int index, QString group);
    Q_INVOKABLE int addOrDelGroup(int index, bool add = false);
    Q_INVOKABLE void showGroup(int index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    void clear();
    bool getInit();
    QString getLogo() const;
    void setLogo(QString path);
    QString getName() const;
    void setName(QString name);
    QString getContent() const;
    void setContent(QString content);
    QStringList getVideoGroups();

    void save();

signals:
     void logoChange();
     void nameChange();
     void contentChange();
     void videoGroupsChange();

protected:
    void copyLogo(QString desPath, bool local);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    QString fileMd5(QString sourceFilePath, qint64 &size);
private:
    QList<FileInfo>* m_data;
    QVector<int> m_groupIndexToIndex;
    QString m_appLocalDataLocation;
    QString m_localFileLocation;
    QString m_logo;
    QString m_name;
    QString m_content;
    int m_curGroupIndex;
    QStringList m_videoGroups;
};

#endif // ResourceAddListModel_H
