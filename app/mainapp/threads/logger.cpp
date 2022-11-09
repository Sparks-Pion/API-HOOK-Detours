#include "Logger.h"

#include <QDebug>
#include <QMessageBox>

char buffer[1024*1024];

Logger::Logger(QObject *parent) : QThread(parent)
{

}

Logger::Logger(QString pipeName, QObject *parent) : QThread(parent)
{
    this->pipeName = _strdup(pipeName.toStdString().c_str());
}

void Logger::run()
{
    //注意:在子线程中不能出现跟UI相关的代码(QMessageBox这些都不行)
    //ASSERT failure in QWidget: "Widgets must be created in the GUI thread.",
    //QMessageBox::information(NULL, "info", "I'm sub thread");

    //do something...
    QThread::msleep(100);
    DWORD ReadNum;
    HANDLE g_hPipe = INVALID_HANDLE_VALUE;
    for(int try_connect = 0;try_connect<5;try_connect++)
    {
        g_hPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE,
                             0, NULL, OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL, NULL);
        if (g_hPipe == INVALID_HANDLE_VALUE)
            QThread::msleep(500);
        else
            break;
    }
    if (g_hPipe == INVALID_HANDLE_VALUE)
    {
        qDebug() << "Connect pipe failed!";
        emit workStatus(false);
        goto out;
    }
    emit workStatus(true);
    while(!bStop)
    {
        if(ReadFile(g_hPipe, buffer, sizeof(buffer), &ReadNum, NULL) == FALSE)
        {
            break;
        }
        buffer[ReadNum] = 0;
        auto str = QString::fromLocal8Bit(buffer);
        emit received(str);
    }
    out:
    qDebug()<<"Close pipe.";
//    emit received("Close pipe.");
    CloseHandle(g_hPipe);
}

void Logger::stopThread()
{
    bStop = true;
}
