#include "openvpnwrapper.h"
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QNetworkInterface>
#include "utilities.h"
OpenVPNWrapper::OpenVPNWrapper(QObject *parent) : QObject(parent)
{
        proc  = new QProcess(this);
        connect(proc,SIGNAL(finished(int)),this,SLOT(onProcessExited(int)));
        connect(proc,SIGNAL(started()),this,SLOT(onStarted()));
        OpenVPNWrapper::wrapper = this ;
}
OpenVPNWrapper* OpenVPNWrapper::wrapper;
void OpenVPNWrapper::StartOpenVPN()
{


    QString Command = "\"" + QCoreApplication::applicationDirPath() ;
    if(QSysInfo::currentCpuArchitecture() == "x86_64"){
        Command += "/OpenVPN/x64/openvpn.exe\"";
    }else{
        Command += "/OpenVPN/x86/openvpn.exe\"";
    }
    Command += QString(" --config \"%1\" --auth-nocache --auth-user-pass \"%2\" --management 127.0.0.1 %3").arg(
                QCoreApplication::applicationDirPath() +"/profile.ovpn",
                QCoreApplication::applicationDirPath() +"/cred",
                QString::number(m_port)
                );
    if(QSysInfo::windowsVersion() == QSysInfo::WV_XP){
        Utilities::ExecuteCmdHidden(Command);
        emit OpenVPNStarted();
    }else{
        proc->start(Command);
        proc->waitForFinished(-1);
    }

}
bool OpenVPNWrapper::isDriverInstalled(){
    QProcess command;
    command.setProcessChannelMode(QProcess::MergedChannels);
    QString wmi = "cmd.exe /c wmic.exe nic get name";
    command.start(wmi);
    command.waitForFinished(-1);
    QString output = QString(command.readAll());
    if(output.contains(QString("TAP-Windows Adapter V9")))
        return true;
    else
        return false;
}
bool OpenVPNWrapper::InstallDrivers(){
    QString DriverPath(QCoreApplication::applicationDirPath() + "/OpenVPN/Drivers");
    QString DevCon(QCoreApplication::applicationDirPath() + "/OpenVPN/Drivers");
    if (QSysInfo::currentCpuArchitecture() == QString("x86_64")){
        DriverPath += QString("/x64/driver.inf") ;
        DevCon += QString("/x64/devcon64.exe") ;
    }else{
        DriverPath += QString("/x86/driver.inf") ;
        DevCon += QString("/x86/devcon86.exe") ;
    }
    QProcess command ;
    QString cmd = "\"%1\" install \"%2\" tap0901";
    cmd = cmd.arg(DevCon,DriverPath);
    cmd = QDir::cleanPath(cmd);
    command.start(cmd);
    command.setProcessChannelMode(QProcess::MergedChannels);
    command.waitForFinished(-1);
    QString output = QString(command.readAll());
    qDebug() << output;
    if(output.contains(QString("Drivers installed successfully."))){
        return true;
    }else{
        return false;
    }
}
void OpenVPNWrapper::DeleteDriversForWinXP(){
    QString DriverPath(QCoreApplication::applicationDirPath() + "/OpenVPN/Drivers");
    QString DevCon(QCoreApplication::applicationDirPath() + "/OpenVPN/Drivers");
    if (QSysInfo::currentCpuArchitecture() == QString("x86_64")){
        DriverPath += QString("/x64/driver.inf") ;
        DevCon += QString("/x64/devcon64.exe") ;
    }else{
        DriverPath += QString("/x86/driver.inf") ;
        DevCon += QString("/x86/devcon86.exe") ;
    }
    QProcess command ;
    command.start(QString("\"%1\" remove tap0901").arg(DevCon));
    command.waitForFinished();
}
bool OpenVPNWrapper::InstallDriversForXp(){
    QString DriverPath(QCoreApplication::applicationDirPath() + "/OpenVPN/Drivers");
    QString DevCon(QCoreApplication::applicationDirPath() + "/OpenVPN/Drivers");
    if (QSysInfo::currentCpuArchitecture() == QString("x86_64")){
        DriverPath += QString("/x64/driver.inf") ;
        DevCon += QString("/x64/devcon64.exe") ;
    }else{
        DriverPath += QString("/x86/driver.inf") ;
        DevCon += QString("/x86/devcon86.exe") ;
    }
    DeleteDriversForWinXP();
    QProcess command ;
    QString cmd = "\"%1\" install \"%2\" tap0901";
    cmd = cmd.arg(DevCon,DriverPath);
    cmd = QDir::cleanPath(cmd);
    command.start(cmd);
    command.setProcessChannelMode(QProcess::MergedChannels);
    command.waitForFinished(-1);
    QString output = QString(command.readAll());
    qDebug() << output;
    if(output.contains(QString("Drivers installed successfully."))){
        return true;
    }else{
        return false;
    }}
void OpenVPNWrapper::CheckDrivers()
{

    if(QSysInfo::windowsVersion() == QSysInfo::WV_XP){

        if(OpenVPNWrapper::InstallDriversForXp()){
            if(wrapper)
                emit wrapper->OnDriversReady();
        }else{
            if(wrapper)
                emit wrapper->OnDriversFaild();
        }
    }
    else{

        if(!OpenVPNWrapper::isDriverInstalled()){
            if(OpenVPNWrapper::InstallDrivers()){
                if(wrapper)
                    emit wrapper->OnDriversReady();
            }else{
                if(wrapper)
                    emit wrapper->OnDriversFaild();
            }
        }else{
            if(wrapper)
                emit wrapper->OnDriversReady();
        }
    }

}
void OpenVPNWrapper::StopOpenVPN()
{
    QProcess prc;
    prc.start("taskkill -im openvpn.exe /f /t");
    prc.waitForFinished();

}
void OpenVPNWrapper::onProcessExited(int)
{
    emit OpenVPNExited();
}

void OpenVPNWrapper::onStarted()
{
    emit OpenVPNStarted();
}
