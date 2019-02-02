#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"
#include <QCloseEvent>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QThread>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QSettings>
#include <QDateTime>
#include <QLocale>
#include "alertdialog.h"
#include <QtConcurrent/QtConcurrent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    setupStartupUi();
}
void MainWindow::setupStartupUi(){
    rasConsole = new RasdialConsole();
    rasThread = new QThread(this);
    VPNNAME = QString("NextVPN");
    OpenVPNThread = new QThread(this);
    ApiThread = new QThread(this);
    menu = new QMenu(this);
    listener = new OpenVPNStateListener(this);
    ui->cmb_protos->addItems(QStringList() << "Cisco AnyConnect" << "SSL Proxy" << "Tunnel+" << "SSTP" << "OpenVPN" << "SSL VPN" << "VPN+");

    ic_notconnected = new QIcon(":/notconnected.png");
    ic_connected = new QIcon(":/connected.png");
    ic_connecting = new QIcon(":/connecting.png");

    action_cancel = new QAction("Cancel",this);
    action_cancel->setEnabled(false);
    action_connect = new QAction("Connect",this);
    action_disconnect = new QAction("Disconnect",this);
    action_disconnect->setEnabled(false);
    action_exit = new QAction("Exit",this);
    action_show = new QAction("Show",this);

    menu->addAction(action_connect);
    menu->addAction(action_cancel);
    menu->addAction(action_disconnect);
    menu->addAction(action_show);
    menu->addAction(action_exit);


    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(*ic_notconnected);
    trayIcon->setToolTip(QString("NextVPN: Not connected."));
    trayIcon->show();
    trayIcon->setContextMenu(menu);


    apiset = new ApiHandler(SRV_URL);
    apiset->moveToThread(ApiThread);

    owrapper = new OpenVPNWrapper();
    owrapper->moveToThread(OpenVPNThread);

    rasConsole->moveToThread(rasThread);

    ui->connected_group->setVisible(false);
    ui->btnBack->setVisible(false);
    ui->setting_group->setVisible(false);
    ui->btnCancel->setVisible(false);
    ui->btnBack->setVisible(false);
    ui->cmbServers->setEnabled(false);
    ui->cmbServers->addItem("Loading...");
    ui->btnCancel->setVisible(false);
    ui->btnDisconnect->setVisible(false);
    SelectedProto = Utilities::stunnel;
    SelectedProtocol = "Stunnel";

    ui->btnConnect->setEnabled(false);
    ShowBytesInOut(false);

    ui->btnWebsite->setStyleSheet(QString("#btnWebsite { background-color: transparent; }"));

    connect(rasConsole,&RasdialConsole::onUpdateStatus,this,&MainWindow::onRasUpdate);
    connect(rasThread,SIGNAL(started()),rasConsole,SLOT(StartVPN()));

    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_TrayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(action_cancel,SIGNAL(triggered(bool)),this,SLOT(on_btnCancel_clicked()));
    connect(action_connect,SIGNAL(triggered(bool)),this,SLOT(on_btnConnect_clicked()));
    connect(action_disconnect,SIGNAL(triggered(bool)),this,SLOT(on_btnDisconnect_clicked()));
    connect(action_exit,SIGNAL(triggered(bool)),this,SLOT(on_Action_Exit(bool)));
    connect(action_show,SIGNAL(triggered(bool)),this,SLOT(show()));
    connect(ApiThread,SIGNAL(started()),apiset,SLOT(GetPageAsync()));
    connect(apiset,SIGNAL(Finished()),ApiThread,SLOT(quit()));
    connect(apiset,SIGNAL(OnDocRecieved(QJsonDocument)),this,SLOT(onServersRecieved(QJsonDocument)));
    connect( ApiThread, SIGNAL(finished()), apiset, SLOT(deleteLater()) );
    connect( ApiThread, SIGNAL(finished()), ApiThread, SLOT(deleteLater()) );
    connect(OpenVPNThread,SIGNAL(started()),owrapper,SLOT(StartOpenVPN()));
    connect(owrapper,SIGNAL(OpenVPNStarted()),this,SLOT(OnOpenVPNProcessStarted()));
    connect(owrapper,SIGNAL(OpenVPNExited()),this,SLOT(OnOpenVPNProcessStopped()));
    connect(owrapper,SIGNAL(OnDriversReady()),this,SLOT(OnOpenVPNDriversReady()));
    connect(owrapper,SIGNAL(OnDriversFaild()),this,SLOT(OnOpenVPNDriversFailed()));
    connect(listener,SIGNAL(OnText(QString)),this,SLOT(OnOpenVPNDataRecieved(QString)));
    connect(listener,SIGNAL(OnError()),this,SLOT(OnOpenVPNDataError()));
    connect(listener,SIGNAL(OnConnected()),this,SLOT(OnVpnDataConnected()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    LoadSettings();


    ApiThread->start();
}
void MainWindow::InstallDrivers(){

}

void MainWindow::OnOpenVPNProcessStarted(){
    listener->StartListening(owrapper->m_port);
}
void MainWindow::OnOpenVPNProcessStopped(){
    on_btnDisconnect_clicked();
}
void MainWindow::OnOpenVPNDataRecieved(const QString& Data){
    QString data;
    if(Data.startsWith(QString(">BYTECOUNT:"))){
        data = Data.mid(11);
        QString recieved = data.left(data.indexOf(",")) ;
        QString sent = data.mid(data.indexOf(",")+1);
        ui->txtBytesIn->setText(QLocale(QLocale::English).toString(recieved.toUInt()));
        ui->txtBytesOut->setText(QLocale(QLocale::English).toString(sent.toUInt()));
    }else{
        if(Data.contains("CONNECTED,SUCCESS")){
            if(!isOpenVPNConnected)
                on_Connected();
            ShowBytesInOut(true);
            //listener->WriteToSocket("state off\r\n");
            listener->WriteToSocket("bytecount 1\r\n");
        }
        else if (Data.contains("ASSIGN_IP") || Data.contains("GET_CONFIG")){
            if(!isOpenVPNConnected)
                setStatus(QString("Registering your computer on network"),MsgType::Normal);
        }
        else if (Data.contains("AUTH")){
             if(!isOpenVPNConnected)
            setStatus(QString("Veryfing your username and password..."),MsgType::Normal);
        }
        else if (Data.contains("CONNECTING") || Data.contains("RESOLVE") || Data.contains("WAIT")){
             if(!isOpenVPNConnected)
            setStatus(QString("Connecting to %1 %2 ...").arg(SelectedProtocol,SelectedServer),MsgType::Normal);
        }
    }


}
void MainWindow::OnOpenVPNDataError(){

}
void MainWindow::OnVpnDataConnected(){
    listener->WriteToSocket("state on\r\n");
}

void MainWindow::LoadSettings(){
    QSettings settings("VPNMakers","NextVPN",this);
    settings.beginGroup("ui_Settings");
    if(settings.value("rememberme",false).toBool()){
        ui->checkBox->setChecked(true);
        ui->txtUsername->setText(settings.value("username","").toString());
        ui->txtPassword->setText(settings.value("password","").toString());
    }else{
         ui->checkBox->setChecked(false);
         ui->txtUsername->setText("");
         ui->txtPassword->setText("");
    }
    if(settings.value("useProxifier",true).toBool() == true){
        ui->rdUseProxifier->click();
    }else{
        ui->rdSetProxy->click();
    }
    Utilities::Proto svType = (Utilities::Proto) settings.value("servertype",Utilities::stunnel).toInt();
    switch(svType){
    case Utilities::zebedee:
        ui->cmb_protos->setCurrentIndex(2);
        break;
    case Utilities::stunnel:
        ui->cmb_protos->setCurrentIndex(1);
        break;
    case Utilities::openvpn:
        ui->cmb_protos->setCurrentIndex(4);
        break;
    case Utilities::sslvpn:
        ui->cmb_protos->setCurrentIndex(5);
        break;
    case Utilities::sstp:
        ui->cmb_protos->setCurrentIndex(3);
        break;
    case Utilities::pptp:
        ui->cmb_protos->setCurrentIndex(6);
        break;
    case Utilities::cisco:
        ui->cmb_protos->setCurrentIndex(0);
        break;
    }
    settings.endGroup();

}
void MainWindow::SaveSettings(){
    QSettings settings("VPNMakers","NextVPN",this);
    settings.beginGroup("ui_Settings");
    if(ui->checkBox->checkState() == Qt::Checked){
        settings.setValue("rememberme",true);
        settings.setValue("username",ui->txtUsername->text());
        settings.setValue("password",ui->txtPassword->text());
    }
    settings.setValue("useProxifier",ui->rdUseProxifier->isChecked() ? true:false);
    settings.setValue("servertype",SelectedProto);
    settings.endGroup();

}

void MainWindow::setStatus(const QString &msg, MainWindow::MsgType type)
{
    switch(type){
    case Success:
        ui->lblStatus->setStyleSheet(QString("#lblStatus{color:green;}"));
        break;
    case Err:
        ui->lblStatus->setStyleSheet(QString("#lblStatus{color:red;}"));

        break;
    case Normal:
        ui->lblStatus->setStyleSheet(QString("#lblStatus{color:#000;}"));

        break;
    }
    ui->lblStatus->setText(msg);

}
void MainWindow::onServersRecieved(const QJsonDocument &doc){
    if(doc.isEmpty()){
        LoadOffline();
    }else{
        ServersObject = doc.object();
        QFile file(QDir::cleanPath(QCoreApplication::applicationDirPath() + "/servers"));
        file.open(QFile::WriteOnly);
        file.write(doc.toBinaryData());
        file.close();
        FillServers();
    }
}
void MainWindow::LoadOffline(){
    QFile file(QDir::cleanPath(QCoreApplication::applicationDirPath() + "/servers"));
    if(!file.exists()){
        setStatus("Unable to connect to server",Err);
        return;
    }
    file.open(QFile::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromBinaryData(file.readAll());
    if(!doc.isNull() && !doc.isEmpty()){
        ServersObject = doc.object();
        FillServers();
    }else{
        setStatus("Unable to connect to server",Err);
    }
    file.close();
}
void MainWindow::FillServers(){
    if(this->ServersObject.isEmpty()){
        if(!ApiThread->isRunning()){
            ApiThread->start();
        }
        return;
    }
    ui->btnConnect->setEnabled(true);
    ui->cmbServers->clear();
    if(this->ServersObject.isEmpty()){
        ui->cmbServers->setEnabled(false);
        return;
    }else{
        ui->cmbServers->setEnabled(true);
        SelectedProtoServers = Utilities::GetServer(ServersObject,SelectedProto);
        foreach(const Utilities::Server&  s, SelectedProtoServers){
            ui->cmbServers->addItem(s.country);
        }
        QJsonObject options = ServersObject.value("Options").toObject();
        QJsonValue jUdpProfile = options.value(QString("OpenVPN-UDP"));
        QJsonValue jTcpProfile = options.value(QString("OpenVPN-TCP"));
        QJsonValue jAppVersion = options.value(QString("AppVersion"));
        QJsonValue jDownloadURL = options.value(QString("DownloadURL"));
        udpProfile = jUdpProfile.toString() ;
        tcpProfile = jTcpProfile.toString() ;

        udpProfile = Utilities::base64_decode(udpProfile);
        tcpProfile = Utilities::base64_decode(tcpProfile);

        DownloadUrl = jDownloadURL.toString();
        NewVersion = jAppVersion.toInt(0);

        if(options.contains(QString("Message"))){
            QJsonValue value = options.value("Message");
            QString msg = value.toString();
            QSettings setts(QString("VPNMakers"),QString("NextVPN"),this);
            setts.beginGroup("x");
            bool ShowMsg = setts.value(QString("ShowMsgForVersion%1").arg(VERSION_STRING),true).toBool();
            if(ShowMsg){
                AlertDialog dlg ;
                dlg.setType(true);
                dlg.setMsg(msg);
                dlg.exec();
                setts.setValue(QString("ShowMsgForVersion%1").arg(VERSION_STRING),false);
            }
            setts.endGroup();
        }
        int Version = QString(VERSION_STRING).toInt();
        if(Version < NewVersion){
            AlertDialog dlg ;
            dlg.setType(false,QString("Update"));
            dlg.setMsg("\"هم اکنون نسخه جدید NextVPN در دسترس میباشد، لطفا نسخه جدید را از طریق دکمه Update دریافت نمایید.\"");
            if(dlg.exec()){
                QDesktopServices::openUrl(DownloadUrl);
            }

        }
    }

}
MainWindow::AuthType MainWindow::Authenticate(const QString Username , const QString Password){
    QString Url = QString(AUTH_URL).arg(Username,Password);
    QJsonDocument result = ApiHandler::GetPage(Url);
    if ( result.isEmpty()){
        return AuthType::AUTH_Error ;
    }
    QJsonObject MainObject = result.object();
    QJsonValue status = MainObject.value(QString("status"));
    QString s = status.toString();
    if(s == "Wrong" ){
        return AuthType::AUTH_Invalid ;
    }else if (s=="Expired"){
        this->Email = MainObject.value(QString("email")).toString();
        return AuthType::AUTH_Expire ;
    }
    else{
        return AuthType::AUTH_Success ;
    }
}
void MainWindow::on_btnBack_clicked()
{
    ui->btnBack->setVisible(false);
    ui->btnSettings->setVisible(true);
    ui->setting_group->setVisible(false);
    ui->connection_group->setVisible(true);
    ui->btnConnect->setVisible(true);
}
void MainWindow::onRasUpdate(int state)
{
    RasdialConsole::ConnectionState s = (RasdialConsole::ConnectionState) state ;
    switch(s){
    case RasdialConsole::Authenticating:
        setStatus("Veryfing your username and password",MsgType::Normal);
        break;
    case RasdialConsole::InvalidUsernamePassword:
        setStatus("Incorrect username or password",MsgType::Err);
        on_Disconnected(false);
        break;
    case RasdialConsole::Connected:
        setStatus("Connected.",MsgType::Success);
        on_Connected();
        break;
    case RasdialConsole::Disconnected:
        setStatus("Offline",MsgType::Normal);
        on_Disconnected(true);
        break;
    case RasdialConsole::RegisteringOnNetwork:
        setStatus("Registering your computer on network",MsgType::Normal);
        break;
    case RasdialConsole::Connecting:
        setStatus(QString("Connecting to %1 %2 ...").arg(Utilities::pptp == SelectedProto ?"VPN+" : "SSTP",SelectedServer),MsgType::Normal);
        break;
    case RasdialConsole::Error:
        on_Disconnected(false);
        setStatus(QString("Unknown Error"),Err);
        break;
    case RasdialConsole::BeginDialed:
        on_Disconnected(false);
        setStatus(QString("Connection is being dialed by OS."),Err);
        break;
    }
}
void MainWindow::onRasError(const QString &err)
{
     setStatus(err,Err);
     on_Disconnected(false);
}
void MainWindow::on_btnSettings_clicked()
{
    ui->btnBack->setVisible(true);
    ui->btnSettings->setVisible(false);
    ui->setting_group->setVisible(true);
    ui->connection_group->setVisible(false);
    ui->btnConnect->setVisible(false);

}

void MainWindow::OnOpenVPNDriversFailed()
{
    setStatus("Unable to install OpenVPN drivers.",Err);
    on_Disconnected(false);
}

void MainWindow::OnOpenVPNDriversReady()
{
    int management_port = Utilities::FindOpenPort(8053,8093);
    owrapper->m_port = management_port;
    OpenVPNThread->start();
}
void MainWindow::on_btnConnect_clicked()
{
    if(ui->txtUsername->text() == "" || ui->txtPassword->text() == "" ){
        setStatus("Please enter your username and password",Err);
        return;
    }
    on_Connecting();
    AuthType at = AUTH_Invalid;
    if(SelectedProto == Utilities::pptp || SelectedProto == Utilities::sstp){
        at = AUTH_Success ;
    }else{
        at = Authenticate(ui->txtUsername->text(),ui->txtPassword->text());
    }
    if(at == AUTH_Error){
        setStatus("Unable to connect to server",Err);
        on_Disconnected(false);
    }else if(at == AUTH_Expire){
        setStatus("Incorrect username or password",Err);
        on_Disconnected(false);
        ShowRechargeMessage();
    }else if (at == AUTH_Invalid){
        setStatus("Incorrect username or password",Err);
        on_Disconnected(false);
    }else{
        setStatus("",Normal);
        SaveSettings();
        Utilities::Server sv =  SelectedProtoServers.at(ui->cmbServers->currentIndex());
        int FreePort = Utilities::FindOpenPort(8080,8190);
        switch(SelectedProto){
        case Utilities::sslvpn:
            Utilities::StartStunnel(sv.server,FreePort,sv.port.toInt());
            if(ui->rdUseProxifier->isChecked()){
                Utilities::ConfigureProxifierProfile(FreePort,ui->txtUsername->text(),ui->txtPassword->text());
                Utilities::StartProxifier();
            }else{
                Utilities::setProxy(QString("127.0.0.1:%1").arg(QString::number(FreePort)));
            }
            SelectedServer = sv.country ;
            SelectedProtocol = "sslVPN";
            on_Connected();
            break;
        case Utilities::stunnel:

            Utilities::StartStunnel(sv.server,FreePort,sv.port.toInt());
            if(ui->rdUseProxifier->isChecked()){
                Utilities::ConfigureProxifierProfile(FreePort,ui->txtUsername->text(),ui->txtPassword->text());
                Utilities::StartProxifier();
            }else{
                Utilities::setProxy(QString("127.0.0.1:%1").arg(QString::number(FreePort)));
            }
            SelectedServer = sv.country ;
            SelectedProtocol = "sslProxy";
            on_Connected();
            break;
        case Utilities::pptp:
            rasConsole->setSettings(ui->txtUsername->text(),ui->txtPassword->text(),sv.server,RasdialConsole::pptp);
            rasThread->start();
            SelectedServer = sv.country ;
            SelectedProtocol = "VPN+";
            setStatus(QString("Connecting to %1 %2 ...").arg("VPN+",SelectedServer),MsgType::Normal);
            break;
        case Utilities::sstp:
            rasConsole->setSettings(ui->txtUsername->text(),ui->txtPassword->text(),sv.server,RasdialConsole::sstp);
            rasThread->start();
            SelectedServer = sv.country ;
            SelectedProtocol = "SSTP";
            setStatus(QString("Connecting to %1 %2 ...").arg("SSTP",SelectedServer),MsgType::Normal);
            break;
        case Utilities::zebedee:
            Utilities::StartZebedee(sv.server,FreePort,sv.port.toInt());
            if(ui->rdUseProxifier->isChecked()){
                Utilities::ConfigureProxifierProfile(FreePort,ui->txtUsername->text(),ui->txtPassword->text());
                Utilities::StartProxifier();
            }else{
                Utilities::setProxy(QString("127.0.0.1:%1").arg(QString::number(FreePort)));
            }
            SelectedServer = sv.country ;
            SelectedProtocol = "Tunnel+";
            on_Connected();
            break;
        case Utilities::openvpn:
            QtConcurrent::run(OpenVPNWrapper::CheckDrivers);
            Utilities::StartOpenVPN(udpProfile,tcpProfile,sv.server,ui->txtUsername->text(),ui->txtPassword->text(),sv.tcp);
            setStatus(QString("Connecting to %1 %2 ...").arg(SelectedProtocol,SelectedServer),MsgType::Normal);
            break;

        }
    }
}
void MainWindow::ShowRechargeMessage()
{
    AlertDialog dlg;
    dlg.setType(false);
    dlg.setMsg("\"اعتبار اکانت تمام شده است. شما میتوانید با فشار دادن کلید زیر اعتبار اکانت خود را تمدید کنید.\"");
    int r = dlg.exec();
    if(r){
        if(!ServersObject.isEmpty()){
            QJsonObject  options = ServersObject.value(QString("Options")).toObject();
            QJsonValue jlink = options.value(QString("ExtendURL"));
            QString link = jlink.toString();
            link = link.arg(ui->txtUsername->text(),Email);
            QDesktopServices::openUrl(QUrl(link));
        }
    }
}
void MainWindow::on_Connecting()
{
    action_disconnect->setEnabled(false);
    action_connect->setEnabled(false);
    action_cancel->setEnabled(true);
    ui->btnConnect->setVisible(false);
    ui->btnCancel->setVisible(true);
    ui->btnDisconnect->setVisible(false);
    ui->connection_group->setEnabled(false);
    ui->btnSettings->setEnabled(false);
    ui->btnBack->setVisible(false);
    trayIcon->setIcon(*ic_connecting);
    SelectedServer = ui->cmbServers->currentText();
    setStatus(QString("Connecting to %1 %2 ...").arg(SelectedProtocol,SelectedServer),MsgType::Normal);
    trayIcon->showMessage("NextVPN",QString("Connecting to %1 %2").arg(SelectedProtocol,SelectedServer),QSystemTrayIcon::Information,1000);
    trayIcon->setToolTip(QString("NextVPN: Connecting..."));
    trayIcon->show();
}

void MainWindow::on_Connected()
{
    QString els = QDateTime::fromTime_t(0).toUTC().toString("hh:mm:ss");
    ui->txtDuration->setText(els) ;
    isOpenVPNConnected= true;
    action_disconnect->setEnabled(true);
    action_connect->setEnabled(false);
    action_cancel->setEnabled(false);
    ui->setting_group->setVisible(false);
    ui->btnConnect->setVisible(false);
    ui->btnCancel->setVisible(false);
    ui->connection_group->setEnabled(false);
    ui->connected_group->setEnabled(true);
    ui->btnSettings->setEnabled(false);
    ui->btnBack->setVisible(false);
    ui->btnSettings->setVisible(false);
    ui->connected_group->setVisible(true);
    ui->connected_group->setEnabled(true);
    ui->connection_group->setVisible(false);
    ui->connection_group->setEnabled(false);
    ui->btnDisconnect->setVisible(true);
    ui->txtCountry->setText(SelectedServer);
    ui->txtProtocol->setText(SelectedProtocol);
    trayIcon->setIcon(*ic_connected);
    trayIcon->setToolTip(QString("NextVPN: Connected."));
    trayIcon->showMessage("NextVPN",QString("Connected to %1 %2").arg(SelectedProtocol,SelectedServer),QSystemTrayIcon::Information,1000);
    setStatus("Connected.",Success);
    trayIcon->show();
    Seconds =0 ;
    timer.start(1000);

}
void MainWindow::onTimer(){
    QString els = QDateTime::fromTime_t(++Seconds).toUTC().toString("hh:mm:ss");
    ui->txtDuration->setText(els) ;
}

void MainWindow::on_Disconnected(bool showMessage = false)
{
    if(rasThread->isRunning()) rasThread->terminate();
    if(OpenVPNThread->isRunning()) OpenVPNThread->terminate();
    QtConcurrent::run(RasdialConsole::KillVPN);
    QtConcurrent::run(OpenVPNWrapper::StopOpenVPN);
    if(QSysInfo::windowsVersion() == QSysInfo::WV_XP)
        QtConcurrent::run(OpenVPNWrapper::DeleteDriversForWinXP);
    isOpenVPNConnected= false;
    action_disconnect->setEnabled(false);
    action_connect->setEnabled(true);
    action_cancel->setEnabled(false);
    ui->btnConnect->setVisible(true);
    ui->btnCancel->setVisible(false);
    ui->btnDisconnect->setVisible(false);
    ui->connected_group->setVisible(false);
    ui->connected_group->setEnabled(false);
    ui->connection_group->setVisible(true);
    ui->connection_group->setEnabled(true);
        timer.stop();
    Seconds = 0;
    ShowBytesInOut(false);
    ui->btnSettings->setEnabled(true);
    ui->btnBack->setVisible(false);
    ui->btnSettings->setVisible(true);
    QtConcurrent::run(Utilities::unsetProxy);
    if(showMessage)
    setStatus("Offline",Normal);
    trayIcon->setIcon(*ic_notconnected);
    trayIcon->setToolTip(QString("NextVPN: Not connected."));
    if(showMessage)
        trayIcon->showMessage("NextVPN",QString("Disconnected From %1").arg(SelectedServer),QSystemTrayIcon::Information,1000);
    trayIcon->show();
}

void MainWindow::on_btnCancel_clicked()
{
    on_btnDisconnect_clicked();
}
//Done
void MainWindow::ShowBytesInOut(bool visible){
    if (visible){
        ui->lblBytesIn->setVisible(true);
        ui->txtBytesIn->setText("0");
        ui->txtBytesOut->setText("0");

        ui->lblBytesOut->setVisible(true);
        ui->txtBytesIn->setVisible(true);
        ui->txtBytesOut->setVisible(true);

    }else{
        ui->lblBytesIn->setVisible(false);
        ui->lblBytesOut->setVisible(false);
        ui->txtBytesIn->setVisible(false);
        ui->txtBytesOut->setVisible(false);
    }
}
void MainWindow::on_TrayIconClicked(QSystemTrayIcon::ActivationReason reason){
    if(reason == QSystemTrayIcon::DoubleClick){
        this->show();
    }
}
void MainWindow::on_Action_Exit(bool triggered){
    exit = true ;
    this->close();
}
void MainWindow::closeEvent(QCloseEvent *event){
    if(this->exit){
        event->accept();
        return;
    }
    event->ignore();
    this->hide();
    trayIcon->showMessage("NextVPN","Minimized",QSystemTrayIcon::Information,2000);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_btnDisconnect_clicked()
{
    timer.stop();
    switch(SelectedProto){
    case Utilities::stunnel :
    case Utilities::sslvpn:
        QtConcurrent::run(Utilities::KillStunnel);
        QtConcurrent::run(Utilities::KillProxifier);
        break;
    case Utilities::zebedee :
        QtConcurrent::run(Utilities::KillZebedee);
        QtConcurrent::run(Utilities::KillProxifier);
        break;
    case Utilities::openvpn :
        if(OpenVPNThread->isRunning()){
            OpenVPNThread->terminate();
        }
        QtConcurrent::run(OpenVPNWrapper::StopOpenVPN);
        break;
    case Utilities::sstp:
    case Utilities::pptp:
        if(rasThread){
            if(rasThread->isRunning()){
                rasThread->terminate();

            }
        }
        QtConcurrent::run(RasdialConsole::KillVPN);
        break;
    }
    on_Disconnected(true);
}

//void MainWindow::on_rdOpenVPN_clicked()
//{
//    SelectedProto = Utilities::openvpn;
//    SelectedProtocol = "OpenVPN";
//    FillServers();
//}

//void MainWindow::on_rdSSTP_clicked()
//{
//    SelectedProto = Utilities::sstp;
//    SelectedProtocol = "SSTP";
//    FillServers();
//}

//void MainWindow::on_rdSslProxy_clicked()
//{
//    SelectedProto = Utilities::stunnel;
//    SelectedProtocol = "sslProxy";
//    FillServers();
//}

//void MainWindow::on_rdSslVPN_clicked()
//{
//    SelectedProto = Utilities::sslvpn;
//    SelectedProtocol = "sslVPN";
//    FillServers();
//}

//void MainWindow::on_rdTunnelPlus_clicked()
//{
//    SelectedProto = Utilities::zebedee;
//    SelectedProtocol = "Tunnel+";
//    FillServers();
//}

//void MainWindow::on_rdVPN_clicked()
//{
//    SelectedProto = Utilities::pptp;
//    SelectedProtocol = "VPN+";
//    FillServers();
//}

void MainWindow::on_btnWebsite_clicked()
{
    if(!ServersObject.isEmpty()){
        QJsonObject  options = ServersObject.value(QString("Options")).toObject();
        QJsonValue jlink = options.value(QString("WebsiteURL"));
        QString link = jlink.toString();
        QDesktopServices::openUrl(QUrl(link));
    }
}

void MainWindow::on_cmb_protos_currentIndexChanged(int index)
{
    switch (index){
    case 0:
        SelectedProto = Utilities::cisco ;
        SelectedProtocol = "Cisco AnyConnect";
        break;
    case 1:
        SelectedProto = Utilities::stunnel;
        SelectedProtocol = "sslProxy" ;
        break;
    case 2:
        SelectedProto = Utilities::zebedee ;
        SelectedProtocol = "Tunnel+" ;
        break;
    case 3:
        SelectedProto = Utilities::sstp ;
        SelectedProtocol  = "SSTP";
        break;
    case 4:
        SelectedProto = Utilities::openvpn;
        SelectedProtocol = "OpenVPN";
        break;
    case 5:
        SelectedProto = Utilities::sslvpn;
        SelectedProtocol = "sslVPN";
        break;
    case 6:
        SelectedProto = Utilities::pptp;
        SelectedProtocol = "VPN+";
        break;

    }
    FillServers();
}
