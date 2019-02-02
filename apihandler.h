#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QObject>
#include <QUrl>
class ApiHandler : public QObject
{
    Q_OBJECT
public:
    ApiHandler(QString URL, QObject *parent = 0);

    static QJsonDocument GetPage(const QString &URL);
signals:
    void OnDocRecieved(const QJsonDocument& Doc);
    void Finished();
public slots:
    void GetPageAsync() ;
private:
    QUrl url ;
};

#endif // APIHANDLER_H
