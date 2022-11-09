//
// Created by 21527 on 22/09/20.
//

#ifndef DETOUR_DEMO_REGWINDOW_H
#define DETOUR_DEMO_REGWINDOW_H

#include <QDialog>
#include <QTreeWidget>

#include <windows.h>

class RegWindow: public QDialog {
Q_OBJECT

public:
    explicit RegWindow(QWidget *parent = nullptr);
    ~RegWindow() override;

private:
    static void getRegTree(QTreeWidgetItem *parent, HKEY hKey,const QString &path="");
};


#endif //DETOUR_DEMO_REGWINDOW_H
