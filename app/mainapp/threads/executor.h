#ifndef QT_TEST_EXECUTOR_H
#define QT_TEST_EXECUTOR_H

#include <QObject>
#include <QThread>

#include <Windows.h>

class Executor : public QThread
{
Q_OBJECT

public:
    explicit  Executor(QObject *parent=nullptr);
    explicit  Executor(const char *arg,const char *exe,const char *dll,QObject *parent=nullptr);


signals:
    void workDone(QString msg);

public slots:
    void stopThread();

protected:
    virtual void run();

private:
    volatile bool bStop = false;//易失性变量,用volatile进行申明
    char szCommand[1024]{};
    char szExe[1024]{};
    char szDll[1024]{};
};


#endif //QT_TEST_EXECUTOR_H
