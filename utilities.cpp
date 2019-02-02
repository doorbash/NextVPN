#include "utilities.h"
#include <QProcess>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSysInfo>
#include <QtXml>
#include <QTcpSocket>
Utilities::Utilities(QObject *parent) : QObject(parent)
{

}

void Utilities::StartStunnel(const QString& Conf){
    QString path = QCoreApplication::applicationDirPath() ;
    path += "/Stunnel/stunnel.conf";
    QFile confFile(path);
    confFile.open(QFile::WriteOnly);
    confFile.write(Conf.toUtf8());
    confFile.close();
    QString cmd("%1 %2");
    cmd = cmd.arg(QCoreApplication::applicationDirPath() +"/Stunnel/stunnel.exe",path);
    QProcess command ;
    //command.startDetached(cmd);
    ExecuteCmdHidden(cmd);
}
void Utilities::StartZebedee(const QString& Server,int localPort,int remotePort){
    QString ZebedeePath = QCoreApplication::applicationDirPath();
    ZebedeePath += "/TunnelPlus/zebedee.exe";
    QString Command = QString("%1 -U %2:%3:%4");
    Command = Command.arg(ZebedeePath,QString::number(localPort),Server,QString::number(remotePort));
    QProcess command ;
    //command.startDetached(Command);
    ExecuteCmdHidden(Command);
}
void Utilities::KillZebedee(){
    QString Command = QString("taskkill -im zebedee.exe /f /t");
    QProcess command ;
    command.start(Command);
    command.waitForFinished();
    qDebug() << command.readAll();
}
void Utilities::KillStunnel(){
    QString Command = QString("taskkill -im stunnel.exe /f /t");
    QProcess command ;
    command.start(Command);
    command.waitForFinished();
    qDebug() << command.readAll();
}
void Utilities::KillProxifier(){
    QString Command = QString("taskkill -im proxifier.exe /f /t");
    QProcess command ;
    command.start(Command);
    command.waitForFinished();
    qDebug() << command.readAll();
}

void Utilities::StartStunnel(const QString &Server, int localPort, int remotePort)
{
    QFile conf(QString(":/stunnel.conf"));
    if(!conf.exists())
        qDebug() << "FILE DOESN'T EXISTS";
    conf.open(QFile::ReadOnly);
    QString config = QString(conf.readAll());
    config = config.arg(QCoreApplication::applicationDirPath() + "/cert.crt",QCoreApplication::applicationDirPath() + "/key.key",QString::number(localPort),Server,QString::number(remotePort));
    qDebug() << config ;
    StartStunnel(config);
}
void Utilities::ConfigureProxifierProfile(int localPort,const QString& username,const QString& password){
    QString ProfilePath = QCoreApplication::applicationDirPath();
    ProfilePath += "/Proxifier/Profiles/Default.ppx";
    QDomDocument document ;
    QFile file(ProfilePath);
    if(file.open(QFile::ReadOnly)){
        document.setContent(&file);
        file.close();
        QDomElement root = document.firstChildElement(QString("ProxifierProfile"));
        QDomNode ProxyList = root.elementsByTagName(QString("ProxyList")).at(0);
        QDomElement Proxy = ProxyList.firstChild().toElement();

        QDomElement newPort = document.createElement(QString("Port")) ;
        QDomText newPortValue = document.createTextNode(QString::number(localPort));
        newPort.appendChild(newPortValue);

        QDomElement newUsername = document.createElement(QString("Username")) ;
        QDomText newUsernameValue = document.createTextNode(username);
        newUsername.appendChild(newUsernameValue);

        QDomElement newPassword = document.createElement(QString("Password")) ;
        QDomText newPasswordValue = document.createTextNode(password);
        newPassword.appendChild(newPasswordValue);

        QDomElement Authentication = Proxy.elementsByTagName(QString("Authentication")).at(0).toElement();

        Proxy.replaceChild(newPort,Proxy.elementsByTagName(QString("Port")).at(0).toElement());
        Authentication.replaceChild(newUsername,Authentication.elementsByTagName(QString("Username")).at(0).toElement());
        Authentication.replaceChild(newPassword,Authentication.elementsByTagName(QString("Password")).at(0).toElement());


        QString debug = document.toString();
        file.open(QFile::WriteOnly);
        file.write(debug.toUtf8());
        file.close();

    }

}
void Utilities::StartProxifier()
{
    Utilities::KillProxifier();
    QProcess command ;
    command.setProcessChannelMode(QProcess::MergedChannels);
    QString ProxifierPath = QCoreApplication::applicationDirPath();
    ProxifierPath += "/Proxifier/Proxifier.exe";
    ProxifierPath = QDir::cleanPath(ProxifierPath) ;
   // command.startDetached(ProxifierPath);
    ExecuteCmdHidden(ProxifierPath);
}
void Utilities::StartOpenVPN(const QString& udpProfile,const QString& tcpProfile,const QString& server, const QString& username , const QString& password, bool isTCP){
    QString Profile = isTCP ? tcpProfile : udpProfile;
    Profile.replace(QString("$server"),server);
    QFile PFile(QCoreApplication::applicationDirPath() + "/profile.ovpn");
    PFile.open(QFile::WriteOnly);
    PFile.write(Profile.toUtf8());
    PFile.close();
    QString UP = QString("%1\r\n%2").arg(username,password);
    QFile UPFILE(QCoreApplication::applicationDirPath() + "/cred");
    UPFILE.open(QFile::WriteOnly);
    UPFILE.write(UP.toUtf8());
    UPFILE.close();
}
void Utilities::StopOpenVPN(){
    QProcess prc;
    prc.execute("Taskkill.exe /im openvpn.exe /f /t");
}
int Utilities::FindOpenPort(int min , int max){
    for (int i = min ; i < max ; i++){
        QTcpSocket socket;
        socket.connectToHost(QString("127.0.0.1"),i);
        socket.waitForConnected(500);
        if(socket.state() != QAbstractSocket::ConnectedState)
            return i;
    }
    return -1;
}
QString Utilities::base64_encode(QString string){
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}
QString Utilities::base64_decode(QString string){
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

QList<Utilities::Server> Utilities::GetServer(QJsonObject MainObject, Proto Service)
{
    QList<Utilities::Server> list;
    QString sname = "" ;
    switch(Service){
    case Utilities::stunnel :
    case Utilities::sslvpn :
        sname = "Stunnel" ;
        break;
    case Utilities::zebedee :
        sname = "TunnelPlus";
        break;
    case Utilities::openvpn:
        sname = "OpenVPN";
        break;
    case Utilities::pptp:
        sname = "PPTP";
        break;
    case Utilities::sstp:
        sname = "SSTP";
        break;
    case Utilities::cisco:
        sname = "Cisco";
        break;
    default :
        sname = "Stunnel" ;
        break;
    }
    QJsonArray ServerList = MainObject.value(sname).toArray();
    foreach (const QJsonValue & value, ServerList) {
        QJsonObject obj = value.toObject();
        Server srv ;
        srv.type = Service ;
        if(Service == Utilities::stunnel || Service == Utilities::zebedee || Service == Utilities::pptp || Service == Utilities::sstp || Service == Utilities::sslvpn ){
            srv.country = obj["Country"].toString();
            srv.server = obj["Server"].toString();
            srv.port = QString::number(obj["Port"].toInt());
        }else if (Service == Utilities::openvpn){
            srv.server = obj["Server"].toString();
            if(obj["Type"].toString() == "UDP")
                srv.tcp = false;
            else
                srv.tcp = true;
            srv.country = obj["Country"].toString() + " [" + QString( srv.tcp == true ? "TCP" : "UDP") +"]";

        }
        list.append(srv);
    }

    return list ;
}
void Utilities::setProxy(const QString& proxy){
    QProcess proc ;
    proc.start(QString("reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" /v ProxyEnable /t REG_DWORD /d 1 /f"));
    proc.waitForFinished();
    qDebug() << proc.readAll();
    proc.start(QString("reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" /v ProxyServer /t REG_SZ /d %1 /f").arg(proxy));
    proc.waitForFinished();
    qDebug() << proc.readAll();
}
void Utilities::unsetProxy(){
    QProcess proc ;
    proc.start(QString("reg.exe add \"HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" /v ProxyEnable /t REG_DWORD /d 0 /f"));
    proc.waitForFinished();
    qDebug() << proc.readAll();
}

//////////////////////////////////////

BOOL Utilities::ExecuteCmdHidden   ( const QString &cmd  )
{
    wchar_t* pass;
    pass = (wchar_t*) malloc (sizeof(wchar_t)*cmd.length()+1);
    cmd.toWCharArray(pass);
    pass[cmd.length()]=0; // Null terminate the string
    STARTUPINFO         si;
    PROCESS_INFORMATION pi;

    BOOL                bRes;

    DWORD               dwCode  =   0;

    ZeroMemory  (   &si,    sizeof  (   STARTUPINFO));

    si.cb           =   sizeof  (   STARTUPINFO);
    si.dwFlags      =   STARTF_USESHOWWINDOW;
    si.wShowWindow  =   SW_HIDE;

    bRes    =   CreateProcess   (   NULL,
                                    pass,
                                    NULL,
                                    NULL,
                                    TRUE,
                                    NORMAL_PRIORITY_CLASS,
                                    NULL,
                                    NULL,
                                    &si,
                                    &pi
                                    );

    CloseHandle (   pi.hProcess);
    CloseHandle (   pi.hThread);

    return  (   bRes);
}

//////////////////////////////////////
