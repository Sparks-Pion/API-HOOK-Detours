#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../QRibbon/QRibbon.h"

#include <QFileDialog>
#include <QTextCodec>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <QDebug>

QTextCodec *gbk = QTextCodec::codecForName("gbk");

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget2->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget2->horizontalHeader()->setStretchLastSection(true);

    connect(ui->btn_dll, &QPushButton::clicked, this, [this] {
        auto fileDialog = new QFileDialog(this, "选择进行插桩的DLL文件",
                                          "./", "DLL Files (*.dll)");
        fileDialog->setFileMode(QFileDialog::ExistingFile);
        fileDialog->setViewMode(QFileDialog::Detail);
        if (fileDialog->exec() == QDialog::Accepted)
            ui->line_dll->setText(fileDialog->selectedFiles()[0]);
    });
    connect(ui->btn_exe, &QPushButton::clicked, this, [this] {
        auto fileDialog = new QFileDialog(this, "选择进行插桩的EXE文件",
                                          "./", "EXE Files (*.exe)");
        fileDialog->setFileMode(QFileDialog::ExistingFile);
        fileDialog->setViewMode(QFileDialog::Detail);
        if (fileDialog->exec() == QDialog::Accepted)
            ui->line_exe->setText(fileDialog->selectedFiles()[0]);
    });
    connect(ui->btn_start, &QPushButton::clicked, this, &MainWindow::inject);

//    connect(ui->clearBtn, &QPushButton::clicked, this, [this] {
//          todo::clear();
//    });

    connect(ui->searchEdit, &QLineEdit::returnPressed, this, [this] {
        auto text = ui->searchEdit->text();
        if (text.isEmpty()) {
            for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
                ui->tableWidget->setRowHidden(i, false);
                for (int j = 0; j < ui->tableWidget->columnCount(); ++j)
                    ui->tableWidget->item(i, j)->setForeground(Qt::black);
            }
            return;
        }
        QRegularExpression re(text);
        // 不区分大小写 QRegularExpression::CaseInsensitiveOption
        re.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
            bool isMatch = false;
            for (int j = 0; j < ui->tableWidget->columnCount(); ++j) {
                ui->tableWidget->item(i, j)->setForeground(Qt::black);
                if (re.match(ui->tableWidget->item(i, j)->text()).hasMatch()) {
                    ui->tableWidget->setRowHidden(i, false);
                    ui->tableWidget->item(i, j)->setForeground(Qt::red);
                    isMatch = true;
                }
            }
            if (!isMatch) {
                ui->tableWidget->setRowHidden(i, true);
            }
        }
    });

    connect(ui->searchEdit2, &QLineEdit::returnPressed, this, [this] {
        auto text = ui->searchEdit2->text();
        if(text.isEmpty()) {
            for (int i = 0; i < ui->tableWidget2->rowCount(); ++i) {
                ui->tableWidget2->setRowHidden(i, false);
                for (int j = 0; j < ui->tableWidget2->columnCount(); ++j)
                    ui->tableWidget2->item(i, j)->setForeground(Qt::black);
            }
            return;
        }
        QRegularExpression re(text);
        // 不区分大小写 QRegularExpression::CaseInsensitiveOption
        re.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        for (int i = 0; i < ui->tableWidget2->rowCount(); ++i) {
            bool isMatch = false;
            for (int j = 0; j < ui->tableWidget2->columnCount(); ++j) {
                ui->tableWidget2->item(i, j)->setForeground(Qt::black);
                if (re.match(ui->tableWidget2->item(i, j)->text()).hasMatch()) {
                    ui->tableWidget2->setRowHidden(i, false);
                    ui->tableWidget2->item(i, j)->setForeground(Qt::red);
                    isMatch = true;
                }
            }
            if (!isMatch) {
                ui->tableWidget2->setRowHidden(i, true);
            }
        }
    });

    connect(ui->topage2Btn, &QPushButton::clicked, this, [this] {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->topage3Btn, &QPushButton::clicked, this, [this] {
        ui->stackedWidget->setCurrentIndex(2);
    });
}

MainWindow::~MainWindow() {
    delete ui;
    logger->wait();
    executor->wait();
}

void MainWindow::inject() {
    QByteArray arg = gbk->fromUnicode(ui->line_arg->text());
    QByteArray exe = gbk->fromUnicode(ui->line_exe->text());
    QByteArray dll = gbk->fromUnicode(ui->line_dll->text());
    if (exe.size() == 0 || dll.size() == 0) {
        QMessageBox::warning(this, "警告", "请先选择DLL和EXE文件");
        return;
    }
    executor = new Executor(arg.data(), exe.data(), dll.data(), this);
    executor->start();

    logger = new Logger(R"(\\.\Pipe\test)", this);
    logger->start();

    connect(logger, &Logger::received, this, [this](const QString &msg) {
        QString str = msg;
        str = str.replace("\r"," ").replace("\n"," ");
        QStringList list = str.split("\t");
        qDebug() << list;
        if (list.size() >= 4) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(list[0]));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(list[1]));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(list[2]));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(list[3]));
            if(list.size() == 5)
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(list[4]));
            else
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(""));
        }
        if(list.size() >= 6){
            int row = ui->tableWidget2->rowCount();
            ui->tableWidget2->insertRow(row);
            ui->tableWidget2->setItem(row, 0, new QTableWidgetItem(list[0]));
            ui->tableWidget2->setItem(row, 1, new QTableWidgetItem(list[1]));
            ui->tableWidget2->setItem(row, 2, new QTableWidgetItem(list[2]));
            ui->tableWidget2->setItem(row, 3, new QTableWidgetItem(list[3]));
            ui->tableWidget2->setItem(row, 4, new QTableWidgetItem(list[4]));
            ui->tableWidget2->setItem(row, 5, new QTableWidgetItem(list[5]));
        }
        // 注册表启动项查找
        QRegularExpression re(R"(SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run)");
        // 不区分大小写 QRegularExpression::CaseInsensitiveOption
        re.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        if (re.match(str).hasMatch()) {
            int row = ui->tableWidget2->rowCount();
            ui->tableWidget2->insertRow(row);
            ui->tableWidget2->setItem(row, 0, new QTableWidgetItem(list[0]));
            ui->tableWidget2->setItem(row, 1, new QTableWidgetItem(list[1]));
            ui->tableWidget2->setItem(row, 2, new QTableWidgetItem(list[2]));
            ui->tableWidget2->setItem(row, 3, new QTableWidgetItem(list[3]));
            ui->tableWidget2->setItem(row, 4, new QTableWidgetItem(list[4]));
            ui->tableWidget2->setItem(row, 5, new QTableWidgetItem("注册表启动项 敏感位置操作!"));
        }
    });
    connect(executor, &Executor::workDone, this, [](const QString &msg) {
        qDebug() << "workDone: " << msg;
    });
    connect(logger, &Logger::workStatus, this, [this](bool status) {
        if (status) {
            ui->stackedWidget->setCurrentIndex(1);
            this->showMaximized();
        } else {
            ui->stackedWidget->setCurrentIndex(0);
            QMessageBox::warning(this, "警告", "注入失败");
        }
    });
}
