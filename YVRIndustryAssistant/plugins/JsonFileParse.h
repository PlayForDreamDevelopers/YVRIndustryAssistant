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


    void deploymentLocal(QJsonArray fileList);
    void deployment(QJsonArray fileList, QString path, QString logo);

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
    void updateGroups(QStringList groups, QString name, QString content);
    void copy(QString sourcePoster, QString sourceFile,  QString desPath, QString fileName);

    QJsonArray localFileList(int index);
    QJsonArray & vrList();
    QJsonArray & groups();
    QJsonArray & videoGroups();

private:
   JsonFileParse();
   QJsonArray m_localFileList;
   QJsonArray m_vrList;
   QJsonArray m_groups;
   QJsonArray m_videoGroups;

   QJsonObject m_baseInfo;
   QString m_appLocalDataLocation;
};

#endif // JSONFILEPARSE_H
