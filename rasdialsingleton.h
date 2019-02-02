#ifndef RASDIALSINGLETON_H
#define RASDIALSINGLETON_H

#include <QObject>
#include <ras.h>
class RasDialSingleton : public QObject
{
    Q_OBJECT
    #define CELEMS(x) ((sizeof(x))/(sizeof(x[0])))
    #define VS_SstpOnly 5   // Only SSTP is attempted
public:
    RasDialSingleton(QObject *parent = 0);
    ~RasDialSingleton();
    enum ConnectionState{
        Connecting ,
        Authenticating ,
        Registering ,
        Connected,
        Disconnected,
        RegisteringOnNetwork,
        Retrying,
        Error,
        AuthError
    };
    static void WINAPI RasDialFunc(UINT unMsg,RASCONNSTATE rasconnstate,DWORD dwError );
    enum vpn_type
    {
        pptp,
        l2tp_psk,
        l2tp_cert,
        ikev2_eap,
        ikev2_cert,
        sstp
    };
    bool connectVpn(const QString& RasEntry, const QString& Username, const QString& Password);
    void createvpn(const QString& EntryName, const QString&Server, const QString &Username, const QString& Password, const QString &PSK, vpn_type type);
    void deleteEntry(const QString &EntryName);
    void hangUp(const QString &EntryName);
signals:
    void onUpdateStatus(int state);
    void onError(const QString &err);
public slots:

private :
    HRASCONN hRasConn = NULL;
    static RasDialSingleton *RASDIAL ;
    static void UpdateStatus(ConnectionState state);
    static void UpdateError(const QString &err);

};

#endif // RASDIALSINGLETON_H
