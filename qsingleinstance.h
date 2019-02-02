#ifndef QSINGLEINSTANCE_H
#define QSINGLEINSTANCE_H

#include <QObject>
#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>


class QSingleInstance : public QObject
{
    Q_OBJECT
public:
    QSingleInstance( const QString& key );
    ~QSingleInstance();
    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY( QSingleInstance )
};

#endif // QSINGLEINSTANCE_H
