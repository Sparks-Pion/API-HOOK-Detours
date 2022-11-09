//
// Created by 21527 on 22/09/20.
//

#ifndef DETOUR_DEMO_FILEWINDOW_H
#define DETOUR_DEMO_FILEWINDOW_H

#include <windows.h>

#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>

class FileWindow : public QDialog {

Q_OBJECT

public:
    explicit FileWindow(HANDLE fileHandle, QWidget *parent = nullptr) :
            QDialog(parent) {
        this->setWindowFlags(Qt::Window);
        this->resize(800, 600);
        this->setWindowTitle("修改文件内容");
        auto layout = new QVBoxLayout(this);
        auto textEdit = new QTextEdit(this);
        auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
        layout->addWidget(textEdit);
        layout->addWidget(buttonBox);
        this->setLayout(layout);

        // 读取文件内容
        DWORD fileSize = GetFileSize(fileHandle, nullptr);
        QByteArray fileContent((int) fileSize, 0);
        DWORD readSize = 0;
        BOOL status = ReadFile(fileHandle, fileContent.data(), fileSize, &readSize, nullptr);
        if(!status)
            QMessageBox::critical(this, "错误", "读取文件内容失败");
        else
            textEdit->setText(fileContent);

        // 保存文件内容
        connect(buttonBox, &QDialogButtonBox::accepted, this, [this, fileHandle, textEdit]() {
            SetFilePointer(fileHandle, 0, nullptr, FILE_BEGIN);
            QByteArray fileContent = textEdit->toPlainText().toUtf8();
            DWORD writeSize = 0;
            BOOL status = WriteFile(fileHandle, fileContent.data(), fileContent.size(), &writeSize, nullptr);
            SetEndOfFile(fileHandle);
            if(status == FALSE)
                QMessageBox::critical(this, "错误", "保存文件失败");
            else
                QMessageBox::information(this, "提示", "保存文件成功,写入字节数:" + QString::number(writeSize));
            this->close();
        });
        connect(buttonBox, &QDialogButtonBox::rejected, this, [this]() {
            this->close();
        });
    };
    ~FileWindow() override = default;;
};

#endif //DETOUR_DEMO_FILEWINDOW_H
