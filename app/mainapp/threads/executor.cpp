#include "executor.h"

#include <QDebug>
#include <QMessageBox>

#include <Windows.h>
#include "detours.h"

Executor::Executor(QObject *parent) : QThread(parent) {

}

Executor::Executor(const char *arg,const char *exe,const char *dll,QObject *parent) : QThread(parent)
{
    strcpy_s(szCommand, arg);
    strcpy_s(szExe, exe);
    strcpy_s(szDll, dll);
}

void Executor::run()
{
    //注意:在子线程中不能出现跟UI相关的代码(QMessageBox这些都不行)
    //ASSERT failure in QWidget: "Widgets must be created in the GUI thread.",
    //QMessageBox::information(NULL, "info", "I'm sub thread");
    //do something...
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
    if (!DetourCreateProcessWithDllExA(szExe, szCommand,
                                       NULL, NULL,
                                       TRUE, dwFlags,
                                       NULL, NULL,
                                       &si, &pi,
                                       szDll, NULL)) {
        DWORD dwError = GetLastError();
//        qDebug() << "DetourCreateProcessWithDllsA failed: " << dwError;
        if (dwError == ERROR_INVALID_HANDLE) {
#if DETOURS_64BIT
//            qDebug() << "withdll.exe: Can't detour a 32-bit target process from a 64-bit parent process.";
            emit workDone("withdll.exe: Can't detour a 32-bit target process from a 64-bit parent process.");
#else
            qDebug() << "withdll.exe: Can't detour a 64-bit target process from a 32-bit parent process.";
            emit workDone("withdll.exe: Can't detour a 64-bit target process from a 32-bit parent process.");
#endif
        }
        emit workDone("unknown error");
        return ;
    }
//    qDebug() << "DetourCreateProcessWithDllsA success";
    ResumeThread(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD dwResult = 0;
    if (!GetExitCodeProcess(pi.hProcess, &dwResult)) {
        DWORD dwError = GetLastError();
//        qDebug() << "GetExitCodeProcess failed: " << dwError;
        emit workDone("GetExitCodeProcess failed: " + QString::number(dwError));
        return ;
    }
//    qDebug() << "GetExitCodeProcess success" << dwResult;
    emit workDone("exitCode "+QString::number(dwResult));
    return ;
}

void Executor::stopThread()
{
    bStop = true;
}
