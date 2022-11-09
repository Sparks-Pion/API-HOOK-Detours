#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../threads/Logger.h"
#include "../threads/executor.h"

#include <QMainWindow>
#include <Q_PID>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    QProcess *process = new QProcess(this);
    Logger *logger{};
    Executor *executor{};
    Ui::MainWindow *ui;

private slots:
    void inject();
};

#endif // MAINWINDOW_H
