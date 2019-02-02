#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <windows.h>

class Utilities : public QObject
{
    Q_OBJECT
public:
    enum Proto{
        stunnel,
        zebedee,
        openvpn,
        pptp,
        sstp,
        sslvpn,
        cisco
    };

    struct Server{
        QString server ;
        QString port ;
        Proto type ;
        bool tcp = true ;
        QString country ;
    };
    explicit Utilities(QObject *parent = 0);
    static void StartProxifier();
    static bool isDriverInstalled();
    static void StartZebedee(const QString &Server, int localPort, int remotePort);
    static void StartStunnel(const QString &Server, int localPort,int remotePort);
    static bool InstallDrivers();
    static void ConfigureProxifierProfile(int localPort, const QString &username, const QString &password);
    static int FindOpenPort(int min, int max);
    static void StartOpenVPN(const QString& udpProfile,const QString& tcpProfile,const QString& server, const QString& username , const QString& password, bool isTCP);
    static void KillZebedee();
    static void KillProxifier();
    static void KillStunnel();
    static QList<Utilities::Server> GetServer(QJsonObject MainObject, Proto Service);
    static QString base64_encode(QString string);
    static QString base64_decode(QString string);
    static WINBOOL ExecuteCmdHidden(const QString &cmd);
    static void setProxy(const QString &proxy);
    static void unsetProxy();

   static void StopOpenVPN();
private:
    static void StartStunnel(const QString &Conf);
signals:

public slots:
protected:
};

#endif // UTILITIES_H
