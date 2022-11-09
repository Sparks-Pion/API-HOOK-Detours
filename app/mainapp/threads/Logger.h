#ifndef QT_TEST_LOGGER_H
#define QT_TEST_LOGGER_H

#include <QObject>
#include <QThread>

#include <Windows.h>

class Logger : public QThread
{
Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    Logger(QString pipeName, QObject *parent = nullptr);

signals:
    void received(QString msg);
    void workStatus(bool status);

public slots:
    void stopThread();

protected:
    virtual void run();

private:
    volatile bool bStop = false;//易失性变量,用volatile进行申明
    LPCSTR pipeName;
};



#endif //QT_TEST_LOGGER_H
