#-------------------------------------------------
#
# Project created by QtCreator 2016-12-28T14:40:26
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NextVPN
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    utilities.cpp \
    apihandler.cpp \
    openvpnwrapper.cpp \
    openvpnstatelistener.cpp \
    alertdialog.cpp \
    rasdialconsole.cpp \
    qsingleinstance.cpp

HEADERS  += mainwindow.h \
    utilities.h \
    apihandler.h \
    openvpnwrapper.h \
    openvpnstatelistener.h \
    alertdialog.h \
    rasdialconsole.h \
    qsingleinstance.h

FORMS    += mainwindow.ui \
    alertdialog.ui

RESOURCES += \
    resources.qrc

win32 {
    RC_FILE = win.rc
    LIBS += -lKernel32
}

DEFINES += VERSION_STRING=\\\"2\\\" \
        AUTH_URL=\\\"http://api-vm.com/api/checkstatus/%1/%2\\\" \
        SRV_URL=\\\"http://api-vm.com/win.json\\\"

DISTFILES += \
    win.rc \
    NextVPN.exe.manifest

CONFIG += c++11
#CONFIG += release

