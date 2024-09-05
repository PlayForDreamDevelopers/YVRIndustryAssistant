#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QObject>

typedef void (QObject::*RequestReplayHandle)(QJsonObject&);

class HttpManager : public QObject
{
    Q_OBJECT



public:
    static HttpManager& ins();
    void login(QString phone);
    void refreshToken();
    void uploadfile(QString path, QString fileName);
    void report(QString eventId,  QJsonObject param = QJsonObject());
    void doReportMsg(bool fromTime = false);
    void timerEvent(QTimerEvent *event) override;

private:
    explicit HttpManager(QObject *parent = nullptr);
    void handleReport(QNetworkReply *reply,  QJsonArray arr);

private:
    QNetworkAccessManager* m_phttpReq = nullptr;

    QList<QJsonObject> m_reportlist;
    QMutex m_mutex;
};

#endif // HTTPMANAGER_H
