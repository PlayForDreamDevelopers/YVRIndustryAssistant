#include "YVRUpdate.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslConfiguration>
#include <QHttpMultiPart>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>
#include <qfile.h>
#include <QFileInfo>
#include <QNetworkReply>
#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QGuiApplication>
#include "version.h"

#define YVRURLTEST "https://apitest.yvrdream.com"
#define YVRURL "https://api.yvrdream.com"


YVRUpdate::YVRUpdate(QObject *parent) : QObject(parent)
{
    m_phttpReq = new QNetworkAccessManager(this);
    check();
}

void YVRUpdate::check()
{
    QNetworkRequest requestInfo;
    QSslConfiguration conf = requestInfo.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    requestInfo.setSslConfiguration(conf);
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain;charset=UTF-8");

    requestInfo.setUrl(QString(YVRRelease ? YVRURL : YVRURLTEST) + "/vrmcsys/appstore/getAlvrVerInfo");

    auto reply =  m_phttpReq->post(requestInfo, "{}");
    connect(reply, &QIODevice::readyRead, this, [=](){

        if(reply->error() != QNetworkReply::NetworkError::NoError)
        {
            //todo: 获取版本文件异常处理
            reply->deleteLater();
            return;
        }

        QJsonParseError parseError;

        QJsonDocument doc= QJsonDocument::fromJson(reply->readAll(),&parseError);

        if(parseError.error != QJsonParseError::NoError)
        {
            qInfo()<< "json parseError";

            reply->deleteLater();
            return;
        }

        QJsonObject obj= doc.object();


        if(obj.contains("errCode") && obj["errCode"].toInt() == 0)
        {
            if(obj.contains("data"))
            {
                auto res = obj["data"].toObject();
                auto version = res["version"].toString();

                if(compareVersion(PFDMVersion, version))
                {
                    qInfo()<< "start update";
                    m_isForce = res["forced"].toInt() == 1;
                    m_note = res["note"].toString().replace("/n", "<br>");
                    m_exeUrl = res["url"].toString();

                    QString filename = saveFileName(m_exeUrl);
                    auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/download/";

                    m_path = path + filename;
                    if (QFile::exists(m_path))
                    {
                        emit updateSignal(m_note, m_isForce);
                    }else
                    {
                        QDir dir;
                        if(!dir.exists(path))
                        {
                            dir.mkdir(path);
                        }

                        download(m_path);
                    }
                }
            }
        }else
        {
            qInfo()<< "check version: " << obj;
        }

       reply->deleteLater();
    });
}

void YVRUpdate::download(QString des)
{
    QNetworkRequest requestInfo;
    QSslConfiguration conf = requestInfo.sslConfiguration();


    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    requestInfo.setSslConfiguration(conf);
    requestInfo.setUrl(QUrl(m_exeUrl));
    auto reply = m_phttpReq->get(requestInfo);
    connect(reply, &QNetworkReply::finished,
            this, [=]{
        QUrl url = reply->url();
        if (reply->error()) {
                qInfo() << QString("Download of %s failed: %s\n").arg(url.toEncoded().constData(), reply->errorString());
        } else {
            if (isHttpRedirect(reply)) {
              qInfo() << QString("Request was redirected.\n");
            } else {
                if (saveToDisk(des, reply)) {
                    emit updateSignal(m_note, m_isForce);
                }
            }
        }

       emit downExeFinish();
    });
}

bool YVRUpdate::compareVersion(QString cur, QString romote)
{
    auto curlist_ = cur.split('_');
    QStringList curlist;
    if(curlist_.size() == 3)
    {
        curlist = curlist_[1].split('.');
        curlist.push_back(curlist_[2]);
    }else
    {
        curlist = cur.split('.');
    }


    auto remotelist = romote.split('.');
    int len = curlist.size() > remotelist.size() ? remotelist.size() : curlist.size();

    if(len < 3)
        return false;

    for(int i = 0; i < len; i++)
    {
        if(curlist[i].toInt() < remotelist[i].toInt())
            return true;

        if(curlist[i].toInt() > remotelist[i].toInt())
            return false;
    }

    return false;
}

QString YVRUpdate::version() const
{
    if(YVRRelease)
    {
        QString ver = "v%1.%2.%3";
        auto verlist = QString(PFDMVersion).split(".");
        if(verlist.size() > 3)
        {
            return ver.arg(verlist[0]).arg(verlist[1]).arg(verlist[2]);
        }
        return "v1.0.0";

    }else
    {
        return  "v" + QString(PFDMVersion);
    }
}

bool YVRUpdate::setup()
{
    if(m_hasSetup)
        return false;

    m_hasSetup = true;

    if(QFile::exists(m_path))
    {
        QProcess::startDetached("\"" + m_path + "\"");
        return true;
    }

    return false;
}

bool YVRUpdate::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

QString YVRUpdate::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "yvrhelep.exe";

    return basename;
}

bool YVRUpdate::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

