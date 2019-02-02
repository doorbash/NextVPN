#ifndef OPENVPNWRAPPER_H
#define OPENVPNWRAPPER_H
#include <QProcess>
#include <QObject>

class OpenVPNWrapper : public QObject
{
    Q_OBJECT
public:
    OpenVPNWrapper(QObject *parent = 0);
    int m_port = 8053;
    static void StopOpenVPN();
    static bool isDriverInstalled();
    static bool InstallDrivers();
    static void CheckDrivers();
    static bool InstallDriversForXp();
    static void DeleteDriversForWinXP();
signals:
    void OpenVPNStdOut(const QString& str);
    void OpenVPNExited();
    void OpenVPNStarted();
    void OnDriversReady();
    void OnDriversFaild();
public slots:
    void StartOpenVPN();
private slots:
    void onProcessExited(int);
    void onStarted();
private:
    QProcess *proc ;
    static OpenVPNWrapper *wrapper;
};


#endif // OPENVPNWRAPPER_H
