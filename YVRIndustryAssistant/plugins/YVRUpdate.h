#ifndef YVRUPDATE_H
#define YVRUPDATE_H

#include <QNetworkAccessManager>
#include <QObject>

class YVRUpdate : public QObject
{
    Q_OBJECT
public:
    explicit YVRUpdate(QObject *parent = nullptr);

    Q_INVOKABLE bool setup();
    Q_INVOKABLE QString version() const;

private:
    bool saveToDisk(const QString &filename, QIODevice *data);
    QString saveFileName(const QUrl &url);
    bool isHttpRedirect(QNetworkReply *reply);
    void check();
    void download(QString des);
    bool compareVersion(QString cur, QString romote);

signals:
    void updateSignal(QString note, bool isForce);
    void downExeFinish();

private:
    QNetworkAccessManager* m_phttpReq = nullptr;
    QString m_exeUrl;
    QString m_note;
    QString m_path;
    bool m_isForce;
    bool m_isInit = false;
    bool m_hasSetup = false;
};

#endif // YVRUPDATE_H
