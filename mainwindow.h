#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "utilities.h"
#include "rasdialconsole.h"
#include <QMenu>
#include "apihandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include "openvpnwrapper.h"
#include "openvpnstatelistener.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    enum MsgType{
        Err,
        Success,
        Normal
    };
    enum AuthType{
        AUTH_Invalid,
        AUTH_Success,
        AUTH_Expire,
        AUTH_Error
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ShowBytesInOut(bool visible);
    void closeEvent(QCloseEvent *event);
    void LoadSettings();
    void SaveSettings();
    static void InstallDrivers();
private slots:
    void on_btnBack_clicked();
    void onRasUpdate(int state);
    void onRasError(const QString& err);
    void on_btnSettings_clicked();
    void OnOpenVPNDriversFailed();
    void OnOpenVPNDriversReady();
    void on_btnConnect_clicked();
    void on_Connecting();
    void on_Connected();
    void on_Disconnected(bool ShowMessage);
    void on_TrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_btnCancel_clicked();

    void on_btnDisconnect_clicked();

    void on_Action_Exit(bool triggered);
    void onServersRecieved(const QJsonDocument& doc);
//    void on_rdOpenVPN_clicked();

//    void on_rdSSTP_clicked();

//    void on_rdSslProxy_clicked();

//    void on_rdSslVPN_clicked();

//    void on_rdTunnelPlus_clicked();

//    void on_rdVPN_clicked();

    void on_btnWebsite_clicked();

    void onTimer();
    void OnOpenVPNDataRecieved(const QString &Data);
    void OnOpenVPNProcessStopped();
    void OnOpenVPNProcessStarted();
    void OnOpenVPNDataError();

    void OnVpnDataConnected();
    //void on_cmb_protos_activated(const QString &arg1);

    void on_cmb_protos_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void setupStartupUi();
    QSystemTrayIcon *trayIcon ;
    RasdialConsole * rasConsole;
    QThread *rasThread;
    QIcon *ic_notconnected,*ic_connected,*ic_connecting;
    QThread *OpenVPNThread ;
    QString SelectedServer,SelectedProtocol,VPNNAME,Email;
    Utilities::Proto SelectedProto;
    QMenu *menu;
    QAction *action_show,*action_exit,*action_disconnect,*action_cancel,*action_connect;
    bool exit = false;
    ApiHandler *apiset;
    QThread *ApiThread ;
    bool isConnected = false;
    QJsonObject ServersObject;
    void setStatus(const QString& msg ,MsgType type);
    QTimer timer;
    void FillServers();
    void LoadOffline();
    MainWindow::AuthType Authenticate(const QString Username, const QString Password);
    QList<Utilities::Server> SelectedProtoServers;
    QString tcpProfile,udpProfile,WebsiteLink,RechargeLink,DownloadUrl;
    int NewVersion ;
    void ShowRechargeMessage();
    long Seconds ;
    OpenVPNWrapper *owrapper;
    OpenVPNStateListener *listener;
    bool isOpenVPNConnected = false;
};

#endif // MAINWINDOW_H
