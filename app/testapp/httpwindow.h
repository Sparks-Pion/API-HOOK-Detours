//
// Created by 21527 on 22/09/27.
//

#ifndef DETOUR_DEMO_HTTPWINDOW_H
#define DETOUR_DEMO_HTTPWINDOW_H

// http 简单服务器
#include <QDialog>

#include <windows.h>

class HttpWindow : public QDialog {
Q_OBJECT

public:
    explicit HttpWindow(QWidget *parent = nullptr);

    ~HttpWindow() override;

};


#endif //DETOUR_DEMO_HTTPWINDOW_H
