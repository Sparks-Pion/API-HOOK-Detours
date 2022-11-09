#ifndef DETOUR_DEMO_MAINWINDOW_H
#define DETOUR_DEMO_MAINWINDOW_H

#include <QMainWindow>

#include <windows.h>

class Mainwindow : public QMainWindow {
Q_OBJECT

public:
    explicit Mainwindow(QWidget *parent = nullptr);

    ~Mainwindow() override;

private:
    // 堆
    HANDLE heapHandle = INVALID_HANDLE_VALUE;
    LPVOID heapPtr = nullptr;
};


#endif //DETOUR_DEMO_MAINWINDOW_H
