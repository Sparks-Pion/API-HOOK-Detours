//
// Created by 21527 on 22/09/27.
//

#include "socketwindow.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

SocketWindow::SocketWindow(bool isServer, QWidget *parent) {
    if (isServer) {
        // 服务端
        auto server = new QTcpServer(this);
        server->listen(QHostAddress::AnyIPv4,6666);
        auto textEdit = new QTextEdit(this);
        textEdit->setReadOnly(true);
        this->setLayout(new QVBoxLayout(this));
        this->layout()->addWidget(textEdit);
        connect(server, &QTcpServer::newConnection, this,[=](){
            auto socket = server->nextPendingConnection();
            connect(socket, &QTcpSocket::readyRead, this, [=](){
                auto data = socket->readAll();
                textEdit->append(data);
            });
        });
    } else {
        // 客户端
        auto socket = new QTcpSocket(this);
        socket->connectToHost(QHostAddress::LocalHost, 6666);
        connect(socket, &QTcpSocket::connected, this, [=]() {
            auto editor = new QTextEdit(this);
            auto sendBtn = new QPushButton("发送", this);
            auto layout = new QVBoxLayout(this);
            layout->addWidget(editor);
            layout->addWidget(sendBtn);
            connect(sendBtn, &QPushButton::clicked, this, [=]() {
                socket->write(editor->toPlainText().toUtf8());
            });
        });
    }
}

SocketWindow::~SocketWindow() {

}
