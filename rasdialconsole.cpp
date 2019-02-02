#include "rasdialconsole.h"
#include <QCoreApplication>
#include <QDebug>
RasdialConsole::RasdialConsole(QObject *parent) : QObject(parent)
{
    proc = new QProcess(this);
    proc->setProcessChannelMode(QProcess::MergedChannels);
    connect(proc,SIGNAL(readyRead()),this,SLOT(ReadyRead()));
}

void RasdialConsole::setSettings(const QString &u, const QString &p, const QString &s, RasdialConsole::ConnectionType t)
{
    type = t;
    server = s;
    username = u ;
    password = p ;
}

void RasdialConsole::StartVPN()
{
    QString command("rasdial.exe NextVPN %1 %2 /PHONE:%3 /PHONEBOOK:\"%4\"");
    if(type == pptp){
        command = command.arg(username,password,server,QCoreApplication::applicationDirPath() + "/pptp.pbk");
    }else {
        command = command.arg(username,password,server,QCoreApplication::applicationDirPath() + "/sstp.pbk");
    }
    qDebug().noquote() << command;
    proc->start(command);
    proc->waitForFinished();
}

void RasdialConsole::KillVPN()
{
    QProcess prc;
    prc.setProcessChannelMode(QProcess::MergedChannels);
    prc.start(QString("taskkill -im rasdial.exe /f"));
    prc.waitForFinished();
    prc.start(QString("Rasdial.exe NextVPN /DISCONNECT"));
    prc.waitForFinished(-1);
    qDebug() << prc.readAll();
}
void RasdialConsole::ReadyRead()
{
    Buffer = QString::fromLatin1(proc->readAll());
    qDebug() << Buffer;
    if (Buffer.contains("Successfully connected to") || Buffer.contains("You are already connected")){
        emit onUpdateStatus(Connected);
    }else if (Buffer.contains("Verifying username and password")){
        emit onUpdateStatus(Authenticating);
    }else if (Buffer.contains("Registering your computer on the network")){
        emit onUpdateStatus(RegisteringOnNetwork);
    }else if (Buffer.contains(QString("user name and password combination you provided is not recognized")) || Buffer.contains(QString("691"))){
        emit onUpdateStatus(InvalidUsernamePassword);
    }else if (Buffer.contains(QString("error 756"))){
        emit onUpdateStatus(BeginDialed);
    }else if(Buffer.contains("Connecting to")){
        emit onUpdateStatus(Connecting); }
    else if (Buffer.contains("error")){
        emit onUpdateStatus(Error);
    }
}
