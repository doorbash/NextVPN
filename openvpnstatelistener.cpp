#include "openvpnstatelistener.h"

OpenVPNStateListener::OpenVPNStateListener(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SIGNAL(OnConnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(OnSocketData()));
    connect(socket,SIGNAL(disconnected()),this,SIGNAL(OnDisconnected()));
}

void OpenVPNStateListener::StartListening(int port)
{
    if(socket->state() == QTcpSocket::ConnectedState){
        socket->disconnectFromHost();
    }

    socket->connectToHost(QString("127.0.0.1"),port);
    socket->waitForConnected(3000);

    if(socket->state() != QTcpSocket::ConnectedState){
        emit OnError();
    }
}

void OpenVPNStateListener::StopListening()
{
    socket->disconnectFromHost();
}

void OpenVPNStateListener::WriteToSocket(const QString &data)
{
    socket->write(data.toUtf8());
    socket->waitForBytesWritten(2000);
    socket->flush();
}

void OpenVPNStateListener::OnSocketData()
{
    QString data = QString(socket->readAll());
    emit OnText(data);
}

