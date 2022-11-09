//
// Created by 21527 on 22/09/27.
//

#ifndef DETOUR_DEMO_SOCKETWINDOW_H
#define DETOUR_DEMO_SOCKETWINDOW_H

#include <QDialog>

#include <windows.h>

class SocketWindow : public QDialog {
Q_OBJECT

public:
    explicit SocketWindow(bool isServer, QWidget *parent = nullptr);

    ~SocketWindow() override;

};


#endif //DETOUR_DEMO_SOCKETWINDOW_H
