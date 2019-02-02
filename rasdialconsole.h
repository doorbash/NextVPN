#ifndef RASDIALCONSOLE_H
#define RASDIALCONSOLE_H

#include <QObject>
#include <QProcess>
class RasdialConsole : public QObject
{
    Q_OBJECT
public:
    explicit RasdialConsole(QObject *parent = 0);
    enum ConnectionState{
        Connecting ,
        Authenticating ,
        Connected,
        Disconnected,
        RegisteringOnNetwork,
        InvalidUsernamePassword,
        Error,
        BeginDialed
    };
    enum ConnectionType{
        pptp,
        sstp
    };
    static void KillVPN();

signals:
    void onUpdateStatus(int state);
public slots:
    void setSettings(const QString &u,const QString &p , const QString &s , ConnectionType t);
    void StartVPN();
private slots:
    void ReadyRead();
private:
    QString username,password,server;
    ConnectionType type ;
    QProcess * proc;
    QString Buffer ;
};

#endif // RASDIALCONSOLE_H
