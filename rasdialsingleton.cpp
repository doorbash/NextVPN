#include "rasdialsingleton.h"
#include <Windows.h>
#include <stdio.h>
#include <ras.h>
#include <rasdlg.h>
#include <tchar.h>
#include <strsafe.h>
#include <QObject>
#include <QDebug>
#include <QProcess>
RasDialSingleton * RasDialSingleton::RASDIAL;
RasDialSingleton::RasDialSingleton(QObject *parent):QObject(parent)
{
    RASDIAL = this ;
}
RasDialSingleton::~RasDialSingleton()
{
    RASDIAL = nullptr ;
}
void RasDialSingleton::RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
    char *szRasString[256] = {0}; // Buffer for storing the error string
    TCHAR szTempBuf[256] = {0};  // Buffer used for printing out the text

    if (dwError)  // Error occurred
    {
        RasGetErrorString(static_cast<UINT>(dwError), reinterpret_cast<LPWSTR>(szRasString),CELEMS(szRasString));

        ZeroMemory(static_cast<LPVOID>(szTempBuf), sizeof(szTempBuf));
        const wchar_t * tmp = L"Error: %d - %s\n";
        StringCchPrintf(szTempBuf, CELEMS(szTempBuf), tmp , dwError, szRasString);
        QString errMsg = QString::fromWCharArray(szTempBuf);
        if(dwError == 691){
            UpdateStatus(AuthError);
        }else{
            qDebug() << errMsg;
            UpdateError(errMsg);
            UpdateStatus(Error);
        }
        if(RASDIAL){
            if(RASDIAL->hRasConn != NULL){
                RasHangUp(RASDIAL->hRasConn);
                RASDIAL->hRasConn = NULL ;
            }
        }
        return;
    }

    // Map each of the states of RasDial() and display on the screen
    // the next state that RasDial() is entering
    switch (rasconnstate)
    {
    case RASCS_OpenPort:
        qDebug()<< "RASCS_OpenPort = " << rasconnstate;
        qDebug()<< "Opening port...";
        //g_pm_mainFrame->setLinkStateInfo("正在打开端口",OPENPORT);
        //g_pFrame->setUserInfo("test","test","test","test","test");
        UpdateStatus(Connecting);
        break;
    case RASCS_PortOpened:
        qDebug()<< "RASCS_PortOpened = " << rasconnstate;
        qDebug()<< "Port opened.";
        //g_pm_mainFrame->setLinkStateInfo("端口打开",PORTOPENED);
        UpdateStatus(Connecting);
        break;
    case RASCS_ConnectDevice:
        qDebug()<< "RASCS_ConnectDevice = " << rasconnstate;
        qDebug()<< "Connecting device...";
        UpdateStatus(Connecting);

        //g_pm_mainFrame->setLinkStateInfo("连接设备",CONNECTDEVICE);
        break;
    case RASCS_DeviceConnected:
        qDebug()<< "RASCS_DeviceConnected = " << rasconnstate;
        qDebug()<< "Device connected.";
        UpdateStatus(Connecting);
        break;
    case RASCS_AllDevicesConnected:
        qDebug()<< "RASCS_AllDevicesConnected = " << rasconnstate;
        qDebug()<< "All devices connected.";
        //g_pm_mainFrame->setLinkStateInfo("设备已连接",ALLDEVICESCONNECTED);
        break;
    case RASCS_Authenticate:
        qDebug()<< "RASCS_Authenticate = " << rasconnstate;
        qDebug()<< "Authenticating...";
        //g_pm_mainFrame->setLinkStateInfo("开始认证",AUTHENTICATE);
        UpdateStatus(Authenticating);

        break;
    case RASCS_AuthNotify:
        qDebug()<< "RASCS_AuthNotify = " << rasconnstate;
        qDebug()<< "Authentication notify.";
        break;
    case RASCS_AuthRetry:
        qDebug()<< "RASCS_AuthRetry = \n" << rasconnstate;
        qDebug()<< "Retrying authentication...";
        UpdateStatus(Retrying);

        break;
    case RASCS_AuthCallback:
        qDebug()<< "RASCS_AuthCallback = " << rasconnstate;
        qDebug()<< "Authentication callback...";
        break;
    case RASCS_AuthChangePassword:
        qDebug()<< "RASCS_AuthChangePassword = " << rasconnstate;
        qDebug()<< "Change password...";
        break;
    case RASCS_AuthProject:
        qDebug()<< "RASCS_AuthProject = " << rasconnstate;
        qDebug()<< "Projection phase started...";
        break;
    case RASCS_AuthLinkSpeed:
        qDebug()<< "RASCS_AuthLinkSpeed = " << rasconnstate;
        qDebug()<< "Negoting speed...";
        break;
    case RASCS_AuthAck:
        qDebug()<< "RASCS_AuthAck = " << rasconnstate;
        qDebug()<< "Authentication acknowledge...";
        break;
    case RASCS_ReAuthenticate:
        qDebug()<< "RASCS_ReAuthenticate = " << rasconnstate;
        qDebug()<< "Retrying Authentication...";
        break;
    case RASCS_Authenticated:
        qDebug()<< "RASCS_Authenticated = " << rasconnstate;
        qDebug()<< "Authentication complete.";
        //g_pm_mainFrame->setLinkStateInfo("认证通过",AUTHENTICATED);
        UpdateStatus(RegisteringOnNetwork);

        break;
    case RASCS_PrepareForCallback:
        qDebug()<< "RASCS_PrepareForCallback = " << rasconnstate;
        qDebug()<< "Preparing for callback...";
        break;
    case RASCS_WaitForModemReset:
        qDebug()<< "RASCS_WaitForModemReset = " << rasconnstate;
        qDebug()<< "Waiting for modem reset...";
        break;
    case RASCS_WaitForCallback:
        qDebug()<< "RASCS_WaitForCallback = " << rasconnstate;
        qDebug()<< "Waiting for callback...";
        break;
    case RASCS_Projected:
        qDebug()<< "RASCS_Projected = " << rasconnstate;
        qDebug()<< "Projection completed.";
        break;
#if (WINVER >= 0x400)
    case RASCS_StartAuthentication:// Windows 95 only
        qDebug()<< "RASCS_StartAuthentication = " << rasconnstate;
        qDebug()<< "Starting authentication...";
        UpdateStatus(Authenticating);

        break;
    case RASCS_CallbackComplete:   // Windows 95 only
        qDebug()<< "RASCS_CallbackComplete = " << rasconnstate;
        qDebug()<< "Callback complete.";
        break;
    case RASCS_LogonNetwork:   // Windows 95 only
        qDebug()<< "RASCS_LogonNetwork = " << rasconnstate;
        qDebug()<< "Login to the network.";
        break;
#endif
    case RASCS_SubEntryConnected:
        qDebug()<< "RASCS_SubEntryConnected = " << rasconnstate;
        qDebug()<< "Subentry connected.";
        break;
    case RASCS_SubEntryDisconnected:
        qDebug()<< "RASCS_SubEntryDisconnected = " << rasconnstate;
        qDebug()<< "Subentry disconnected.";
        break;
        //PAUSED STATES:
    case RASCS_Interactive:
        qDebug()<< "RASCS_Interactive = " << rasconnstate;
        qDebug()<< "In Paused state: Interactive mode.";
        break;
    case RASCS_RetryAuthentication:
        qDebug()<< "RASCS_RetryAuthentication = " << rasconnstate;
        qDebug()<< "In Paused state: Retry Authentication...";
        break;
    case RASCS_CallbackSetByCaller:
        qDebug()<< "RASCS_CallbackSetByCaller = " << rasconnstate;
        qDebug()<< "In Paused state: Callback set by Caller.";
        break;
    case RASCS_PasswordExpired:
        qDebug()<< "RASCS_PasswordExpired = " << rasconnstate;
        qDebug()<< "In Paused state: Password has expired...";
        break;

    case RASCS_Connected: // = RASCS_DONE:
        qDebug()<< "RASCS_Connected = " << rasconnstate;
        qDebug()<< "#########Connection completed.";
        if(RASDIAL)
            emit RASDIAL->onUpdateStatus(Connected);
       // g_pm_mainFrame->setLinkStateInfo("连接成功",CONNECTED);

        break;
    case RASCS_Disconnected:
        qDebug()<< "RASCS_Disconnected = " << rasconnstate;
        qDebug()<< "Disconnecting...";
        UpdateStatus(Disconnected);
        break;
    default:
        qDebug()<< "Unknown Status = " << rasconnstate;
        qDebug()<< "What are you going to do about it?";
        break;
    }
}
bool RasDialSingleton::connectVpn(const QString &RasEntry, const QString &Username, const QString &Password)
{
    wchar_t* rasConn = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,RasEntry.length() + 1);
    wchar_t *username = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,Username.length() + 1);
    wchar_t *password = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,Password.length() + 1);
    RasEntry.toWCharArray(rasConn);
    Username.toWCharArray(username);
    Password.toWCharArray(password);

    rasConn[RasEntry.length()] = '\0';
    username[Username.length()] = '\0';
    password[Password.length()] = '\0';

    LPRASDIALPARAMS lpRasDialParams = NULL;   // Structure to store the RasDial parameters
    if(hRasConn != NULL)
        RasHangUp(hRasConn);
    DWORD           nRet = 0;                 // Return value from a function
    lpRasDialParams = (LPRASDIALPARAMS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RASDIALPARAMS));
    if (NULL == lpRasDialParams)
    {
        qDebug() << "HeapAlloc failed\n";
    }
    lpRasDialParams->dwSize =sizeof(RASDIALPARAMS);
    StringCchCopy(lpRasDialParams->szEntryName, CELEMS(lpRasDialParams->szEntryName), L"NextVPN");
    StringCchCopy(lpRasDialParams->szUserName, CELEMS(lpRasDialParams->szUserName), username);
    StringCchCopy(lpRasDialParams->szPassword,CELEMS(lpRasDialParams->szPassword), password);

    hRasConn = NULL;
    nRet = RasDial(NULL, NULL, lpRasDialParams, 0,reinterpret_cast<void*>(&RasDialSingleton::RasDialFunc) , &hRasConn);
    if (nRet != 0)
    {
        qDebug() << "RasDial return: " << nRet;
        //756 Connection Locked - Restart Required or Required new Connection Name
        if(hRasConn != NULL)
            RasHangUp(hRasConn);
        RasDeleteEntry(NULL, lpRasDialParams->szEntryName);
        return false;
    }
    else
    {
        /*RasHangUp(m_hRasConn);
RasDeleteEntry(NULL,name);*/
    }
    return true;
}
void RasDialSingleton::createvpn(const QString &EntryName, const QString &Server, const QString &Username, const QString &Password, const QString &PSK, vpn_type type)
{
    wchar_t* name = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,EntryName.length() + 1);
    wchar_t *server = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,Server.length() + 1);
    wchar_t *username = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,Username.length() + 1);
    wchar_t *password = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,Password.length() + 1);
    wchar_t *psk = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,PSK.length() + 1);

    EntryName.toWCharArray(name);
    Server.toWCharArray(server);
    Username.toWCharArray(username);
    Password.toWCharArray(password);
    PSK.toWCharArray(psk);


    name[EntryName.length()] = '\0';
    server[Server.length()] = '\0';
    username[Username.length()] = '\0';
    password[Password.length()] = '\0' ;
    psk[PSK.length()] = '\0';


    DWORD size = 0;
    RasGetEntryProperties(NULL, L"", NULL, &size, NULL, NULL);
    LPRASENTRY pras = (LPRASENTRY)malloc(size);
    memset(pras, 0, size);
    pras->dwSize = size;
    pras->dwType = RASET_Vpn;
    pras->dwRedialCount = 1;
    pras->dwRedialPause = 10;
    pras->dwfNetProtocols = RASNP_Ip;
    pras->dwEncryptionType = ET_Optional;
    StringCchCopy(pras->szLocalPhoneNumber,CELEMS(pras->szLocalPhoneNumber) ,server);
    StringCchCopy(pras->szDeviceType,CELEMS(pras->szDeviceType), RASDT_Vpn);
    pras->dwfOptions = RASEO_RemoteDefaultGateway;

    if(pptp == type)
    {
        pras->dwfOptions |= RASEO_RequirePAP | RASEO_RequireEncryptedPw;
        pras->dwVpnStrategy = VS_PptpOnly;
    }
    else if(l2tp_psk == type)
    {
        pras->dwVpnStrategy = VS_L2tpOnly;
        pras->dwfOptions |= RASEO_RequireEncryptedPw;
        pras->dwfOptions2 |= RASEO2_UsePreSharedKey;
    }else if (sstp == type){
        pras->dwVpnStrategy = VS_SstpOnly;
        pras->dwfOptions |= RASEO_RequireMsCHAP ;
        pras->dwfOptions |= RASEO_RequireMsCHAP2;
        pras->dwfOptions |= RASEO_RequireEncryptedPw;

    }
    RasSetEntryProperties(NULL,L"NextVPN", pras, pras->dwSize, NULL, 0);
    RASCREDENTIALS ras_cre = {0};
    ras_cre.dwSize = sizeof(ras_cre);
    ras_cre.dwMask = RASCM_UserName | RASCM_Password;
    StringCchCopy(ras_cre.szUserName,CELEMS(ras_cre.szUserName), username);
    StringCchCopy(ras_cre.szPassword,CELEMS(ras_cre.szPassword), password);

    if(l2tp_psk == type)
    {
        RASCREDENTIALS ras_cre_psk = {0};
        ras_cre_psk.dwSize = sizeof(ras_cre_psk);
        ras_cre_psk.dwMask = RASCM_PreSharedKey;
        StringCchCopy(ras_cre_psk.szPassword,CELEMS(ras_cre_psk.szPassword), psk);
        RasSetCredentials(NULL, name, &ras_cre_psk, FALSE);
    }
    free(pras);
}
void RasDialSingleton::deleteEntry(const QString &EntryName)
{
    wchar_t* name = (wchar_t*) HeapAlloc(GetProcessHeap() ,HEAP_ZERO_MEMORY,EntryName.length() + 1);
    EntryName.toWCharArray(name);
    name[EntryName.length()] = '\0';
    RasDeleteEntryW(NULL,name);
}
void RasDialSingleton::hangUp(const QString &EntryName)
{
    if(this->hRasConn != NULL){
        RasHangUp(this->hRasConn);
        this->hRasConn = NULL ;
    }
}
void RasDialSingleton::UpdateStatus(ConnectionState state)
{
    if(RASDIAL)
        emit RASDIAL->onUpdateStatus(state);
}
void RasDialSingleton::UpdateError(const QString &err)
{
    if(RASDIAL)
        emit RASDIAL->onError(err);
}
