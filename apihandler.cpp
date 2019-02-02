#include "apihandler.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
ApiHandler::ApiHandler(QString URL, QObject *parent):QObject(parent)
{
    url = QUrl(URL);
}

void ApiHandler::GetPageAsync()
{
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));

    loop.exec();

    if (reply->error() == QNetworkReply::NoError){
        QByteArray bts = reply->readAll();
        QString str(bts);

        QJsonParseError *err = new QJsonParseError();
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(),err);
        if(err->error == QJsonParseError::NoError)
            emit OnDocRecieved(doc);
        else
            emit OnDocRecieved(QJsonDocument());


        emit Finished();
        delete reply ;
    }else{
        delete reply;
        emit OnDocRecieved(QJsonDocument());
        emit Finished();
    }
}
QJsonDocument ApiHandler::GetPage(const QString& URL)
{
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(URL)));

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));

    loop.exec();

    if (reply->error() == QNetworkReply::NoError){
        QByteArray bts = reply->readAll();
        QJsonParseError *err = new QJsonParseError();
        QJsonDocument doc = QJsonDocument::fromJson(bts,err);
        delete reply;
        if(err->error == QJsonParseError::NoError)
            return doc;
        else
            return QJsonDocument();

    }else{
        delete reply;
        return QJsonDocument();
    }
}
