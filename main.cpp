#include "mainwindow.h"
#include "qsingleinstance.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QSingleInstance guard( "some_random_key" );
    if ( !guard.tryToRun() )
        return 0;
    QApplication a(argc, argv);

    MainWindow w;
    w.setFixedSize(w.size());
    w.show();

    return a.exec();
}
