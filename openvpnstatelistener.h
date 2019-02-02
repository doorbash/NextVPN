#ifndef OPENVPNSTATELISTENER_H
#define OPENVPNSTATELISTENER_H

#include <QObject>
#include <QTcpSocket>
class OpenVPNStateListener : public QObject
{
    Q_OBJECT
public:
    explicit OpenVPNStateListener(QObject *parent = 0);

signals:
    void OnError();
    void OnText(const QString &msg);
    void OnConnected();
    void OnDisconnected();
public slots:
    void StartListening(int port);
    void StopListening();
    void WriteToSocket(const QString& data);
private slots:
    void OnSocketData();
private:
    QTcpSocket *socket;
};

#endif // OPENVPNSTATELISTENER_H
