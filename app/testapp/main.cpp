#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QStyleFactory>

#include <windows.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    Mainwindow w;
    w.show();
    return QApplication::exec();
}
