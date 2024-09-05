#include "HttpManager.h"

#include <QDir>
#include <QEventloop>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include "sysmgr/SystemInfo.h"
#include "version.h"

//#include "QtGui/private/qzipwriter_p.h"


const QString loaclurl = YVRRelease ?  "https://api.yvrdream.com" : "https://apitest.yvrdream.com/";

HttpManager &HttpManager::ins()
{
    static HttpManager instace;
    return instace;
}

void HttpManager::uploadfile(QString path, QString fileName)
{
    QHttpPart textpart;
    textpart.setHeader(QNetworkRequest::ContentDispositionHeader,QString("form-data;name=\"file\";filename=\".json\""));
    textpart.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data;");

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QFile *file  = new QFile(path, multiPart);
    file->open(QIODevice::ReadOnly);
    textpart.setBodyDevice(file);

    QHttpPart textpart1;
    textpart1.setHeader(QNetworkRequest::ContentDispositionHeader,"form-data;name=\"fileName\"");
    textpart1.setBody(fileName.toUtf8());


   QNetworkRequest requestInfo;
   QSslConfiguration conf = requestInfo.sslConfiguration();
   conf.setPeerVerifyMode(QSslSocket::VerifyNone);
   conf.setProtocol(QSsl::TlsV1SslV3);
   requestInfo.setSslConfiguration(conf);

   requestInfo.setUrl(QUrl(loaclurl + "/vrmcsys/fileserver/StreamAssistant/upload"));


    multiPart->append(textpart);
    multiPart->append(textpart1);
    QNetworkReply* reply = m_phttpReq->post(requestInfo, multiPart);

     if(reply){
         connect(reply, &QNetworkReply::finished, this, [=]{
             QJsonParseError parseError;
             QJsonDocument doc= QJsonDocument::fromJson(reply->readAll(),&parseError);

             if(parseError.error != QJsonParseError::NoError)
             {
                 qInfo()<< reply->url() << " json parse fail";
                 reply->deleteLater();
                 return;
             }

             QJsonObject result = doc.object();
             if(result["errCode"].toInt() != 0)
             {
                 qInfo() << result["errMsg"].toString() << ":" << reply->url();
             }

             reply->deleteLater();
         });
     }
}



void HttpManager::report(QString eventId, QJsonObject param)
{
    QJsonObject requestbody;
    requestbody["timestamp"] = QDateTime::currentDateTime().toMSecsSinceEpoch();
    requestbody["defaultData"] = SystemInfo::ins()->defaultParam();

    requestbody["eventId"] = eventId;
    requestbody["extraData"] = param;
    m_mutex.lock();
    m_reportlist.append(requestbody);
    m_mutex.unlock();
}

void HttpManager::doReportMsg(bool fromTime)
{
    QJsonDocument doc;
    QJsonArray arr;
    if(m_reportlist.isEmpty())
        return;

    //qInfo() << m_reportlist;
    m_mutex.lock();
    for(auto item: m_reportlist)
    {
        arr.append(item);
    }
    m_reportlist.clear();
    m_mutex.unlock();

    doc.setArray(arr);

    QNetworkRequest requestInfo;
    QSslConfiguration conf = requestInfo.sslConfiguration();

    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader,"application/json;charset=UTF-8");
    requestInfo.setSslConfiguration(conf);
    requestInfo.setUrl(QUrl( loaclurl + "/vrmcsys/datacenter/streaming_data_upload"));
    auto reply = m_phttpReq->post(requestInfo, doc.toJson(QJsonDocument::Compact));

    if(fromTime)
    {
        connect(reply, &QNetworkReply::finished, this, [=]{
            handleReport(reply, arr);
        });
    }
    else
    {
        QEventLoop loop;
        QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
        loop.exec();
        handleReport(reply, arr);
    }
}

void HttpManager::timerEvent(QTimerEvent *)
{
    doReportMsg(true);
}


HttpManager::HttpManager(QObject *parent) : QObject(parent)
{
    m_phttpReq = new QNetworkAccessManager(this);
    startTimer(10 * 60 * 1000);
}

void HttpManager::handleReport(QNetworkReply *reply, QJsonArray arr)
{
    QJsonParseError parseError;
    QJsonDocument doc= QJsonDocument::fromJson(reply->readAll(),&parseError);

    if(parseError.error != QJsonParseError::NoError)
    {
        qCritical() << "error evnetid: "  << arr << " json parse fail";
        reply->deleteLater();
        return;
    }

    QJsonObject result = doc.object();
    if(result["errCode"].toInt() != 0)
    {
        qCritical() << "report error: " <<result << ":" << arr;
    }

    reply->deleteLater();
}
