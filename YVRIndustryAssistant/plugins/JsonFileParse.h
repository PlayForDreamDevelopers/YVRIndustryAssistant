#ifndef JSONFILEPARSE_H
#define JSONFILEPARSE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

class JsonFileParse
{
public:
    static JsonFileParse* ins();
    void loadFile();

    void updateFile(QJsonArray fileList);
    void updateDevice(QJsonArray& vrList, QStringList group);

    void setPort(int port);
    void setLogo(QString path);
    void setLocalFile(QString path);
    QString getLocalFile();
    QString getLogo();
    QString getName();
    QString getContent();
    void setPlayControl(bool  autoControl);
    bool getPlayControl();
    void deployment(QJsonArray fileList, QString path);
    void updateLocal();
    void updateGroups(QStringList groups, QString name, QString content);
    void copy(QString sourcePoster, QString sourceFile,  QString desPath, QString fileName);

    QJsonArray & fileList();
    QJsonArray & vrList();
    QJsonArray & groups();
    QJsonArray & videoGroups();

private:
   JsonFileParse();
   QJsonArray m_fileList;
   QJsonArray m_vrList;
   QJsonArray m_groups;
   QJsonArray m_videoGroups;

   QJsonObject m_baseInfo;
   QString m_appLocalDataLocation;
};

#endif // JSONFILEPARSE_H
