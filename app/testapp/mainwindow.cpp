// You may need to build the project (run Qt uic code generator) to get "ui_Mainwindow.h" resolved

#include "mainwindow.h"
#include "filewindow.h"
#include "regwindow.h"
#include "socketwindow.h"
#include "httpwindow.h"

#include <windows.h>

#include <QThread>
#include <QLineEdit>
#include <QTextCodec>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QByteArray>
#include <QCoreApplication>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

namespace {
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
}

Mainwindow::Mainwindow(QWidget *parent) :
        QMainWindow(parent) {

    auto messageBoxBtn = new QPushButton("MessageBox");
    messageBoxBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(messageBoxBtn, &QPushButton::clicked, this, [this] {
        int boxID = MessageBox(
                (HWND) this->winId(),
                TEXT("Resource not available\nDo you want to try again?"),
                TEXT("Account Details"),
                MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
        );
        if (boxID == IDCANCEL) {
            qDebug() << "选择了取消";
        } else if (boxID == IDTRYAGAIN) {
            qDebug() << "选择了重试";
        } else if (boxID == IDCONTINUE) {
            qDebug() << "选择了继续";
        } else
            qDebug() << "选择了其他";
    });

    auto messageBoxExBtn = new QPushButton("MessageBoxEx");
    messageBoxExBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(messageBoxExBtn, &QPushButton::clicked, this, [this] {
        int boxID = MessageBoxEx(
                (HWND) this->winId(),
                TEXT("Resource not available\nDo you want to try again?"),
                TEXT("Account Details"),
                MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
        );
        if (boxID == IDCANCEL) {
            qDebug() << "选择了取消";
        } else if (boxID == IDTRYAGAIN) {
            qDebug() << "选择了重试";
        } else if (boxID == IDCONTINUE) {
            qDebug() << "选择了继续";
        } else
            qDebug() << "选择了其他";
    });

    auto messageBoxIndirectBtn = new QPushButton("MessageBoxIndirect");
    messageBoxIndirectBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(messageBoxIndirectBtn, &QPushButton::clicked, this, [this] {
        MSGBOXPARAMS msgBoxParams;
        msgBoxParams.cbSize = sizeof(MSGBOXPARAMS);
        msgBoxParams.hwndOwner = (HWND) this->winId();
        msgBoxParams.hInstance = GetModuleHandle(nullptr);
        msgBoxParams.lpszText = TEXT("Resource not available\nDo you want to try again?");
        msgBoxParams.lpszCaption = TEXT("Account Details");
        msgBoxParams.dwStyle = MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2;
        msgBoxParams.lpszIcon = nullptr;
        msgBoxParams.dwContextHelpId = 0;
        msgBoxParams.lpfnMsgBoxCallback = nullptr;
        msgBoxParams.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
        int boxID = MessageBoxIndirect(&msgBoxParams);
        if (boxID == IDCANCEL) {
            qDebug() << "选择了取消";
        } else if (boxID == IDTRYAGAIN) {
            qDebug() << "选择了重试";
        } else if (boxID == IDCONTINUE) {
            qDebug() << "选择了继续";
        } else
            qDebug() << "选择了其他";
    });

    auto messageBoxTimeoutBtn = new QPushButton("MessageBoxTimeout");
    messageBoxTimeoutBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(messageBoxTimeoutBtn, &QPushButton::clicked, this, [this] {
        HMODULE library_handle = LoadLibrary("user32.dll");
        if (library_handle == nullptr) {
            QMessageBox::critical(this, "错误", "加载user32.dll失败");
            return;
        }
        int (WINAPI *MessageBoxTimeout)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType,
                                        WORD wLanguageId, DWORD dwMilliseconds);
        MessageBoxTimeout = reinterpret_cast<decltype(MessageBoxTimeout)>
        (GetProcAddress(library_handle, "MessageBoxTimeoutW"));
        int boxID = MessageBoxTimeout(
                (HWND) this->winId(),
                L"Resource not available\nDo you want to try again?",
                L"Account Details",
                MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                3000
        );
        if (boxID == IDCANCEL) {
            qDebug() << "选择了取消";
        } else if (boxID == IDTRYAGAIN) {
            qDebug() << "选择了重试";
        } else if (boxID == IDCONTINUE) {
            qDebug() << "选择了继续";
        } else
            qDebug() << "选择了其他";
        CloseHandle(library_handle);
    });

    auto heapCreateBtn = new QPushButton("HeapCreate");
    heapCreateBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(heapCreateBtn, &QPushButton::clicked, this, [this] {
        if (heapHandle != INVALID_HANDLE_VALUE) {
            QMessageBox::critical(this, "错误", "堆已经创建");
            return;
        }
        heapHandle = HeapCreate(0, 0, 0);
        if (heapHandle == INVALID_HANDLE_VALUE)
            QMessageBox::critical(this, "错误", "创建堆失败");
        else
            QMessageBox::information(this, "提示", "创建堆成功");
    });

    auto heapDestroyBtn = new QPushButton("HeapDestroy");
    heapDestroyBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(heapDestroyBtn, &QPushButton::clicked, this, [this] {
        if (heapHandle == INVALID_HANDLE_VALUE) {
            QMessageBox::critical(this, "错误", "堆未创建");
            return;
        }
        if (HeapDestroy(heapHandle))
            QMessageBox::information(this, "提示", "销毁堆成功");
        else
            QMessageBox::critical(this, "错误", "销毁堆失败");
        heapHandle = INVALID_HANDLE_VALUE;
    });

    auto heapAllocBtn = new QPushButton("HeapAlloc");
    heapAllocBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(heapAllocBtn, &QPushButton::clicked, this, [this] {
        if (heapHandle == INVALID_HANDLE_VALUE) {
            QMessageBox::critical(this, "错误", "堆未创建");
            return;
        }
        if (heapPtr != nullptr) {
            QMessageBox::critical(this, "错误", "堆已经分配");
            return;
        }
        heapPtr = HeapAlloc(heapHandle, 0, 1024);
        if (heapPtr == nullptr)
            QMessageBox::critical(this, "错误", "分配堆失败");
        else
            QMessageBox::information(this, "提示", "分配堆成功");
    });

    auto heapFreeBtn = new QPushButton("HeapFree");
    heapFreeBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(heapFreeBtn, &QPushButton::clicked, this, [this] {
        if (heapHandle == INVALID_HANDLE_VALUE) {
            QMessageBox::critical(this, "错误", "堆未创建");
            return;
        }
        if (heapPtr == nullptr) {
            QMessageBox::critical(this, "错误", "堆未分配");
            return;
        }
        if (HeapFree(heapHandle, 0, heapPtr))
            QMessageBox::information(this, "提示", "释放堆成功");
        else
            QMessageBox::critical(this, "错误", "释放堆失败");
        heapPtr = nullptr;
    });

    auto createFileBtn = new QPushButton("文件读写");
    createFileBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(createFileBtn, &QPushButton::clicked, this, [this] {
        QString fileName = QFileDialog::getOpenFileName(this, "选择文件", "", "所有文件(*.*)");
        if (fileName.isEmpty()) return;
        HANDLE fileHandle = CreateFile(
                gbk->fromUnicode(fileName).data(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            QMessageBox::warning(this, "错误", "打开文件失败");
            return;
        }
        auto fileWindow = new FileWindow(fileHandle, this);
        fileWindow->exec();
        CloseHandle(fileHandle);
    });

    auto createFileBtn2 = new QPushButton("新建文件");
    createFileBtn2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(createFileBtn2, &QPushButton::clicked, this, [this] {
        QString fileName = QFileDialog::getSaveFileName(this, "选择文件", "", "所有文件(*.*)");
        if (fileName.isEmpty()) return;
        HANDLE fileHandle = CreateFile(
                gbk->fromUnicode(fileName).data(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                nullptr,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            QMessageBox::warning(this, "错误", "打开文件失败");
            return;
        }
        auto fileWindow = new FileWindow(fileHandle, this);
        fileWindow->exec();
        CloseHandle(fileHandle);
    });

    auto regCreateKeyBtn = new QPushButton("注册表操作");
    regCreateKeyBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(regCreateKeyBtn, &QPushButton::clicked, this, [this] {
        auto regWindow = new RegWindow(this);
        regWindow->exec();
    });

    auto regCreateKeyBtn2 = new QPushButton("注册表编辑器");
    regCreateKeyBtn2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(regCreateKeyBtn2, &QPushButton::clicked, this, [this] {
        //QProcess::startDetached("regedit", QStringList());
        //CreateProcess regedit
        STARTUPINFO si{};
        PROCESS_INFORMATION pi{};
        si.cb = sizeof(si);
        char regeditPath[MAX_PATH] = "regedit.exe";
        if (CreateProcess(nullptr, regeditPath, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        } else {
            QMessageBox::warning(this, "错误", "启动注册表编辑器失败");
        }
    });

    auto unusualHeapBtn = new QPushButton("堆异常");
    unusualHeapBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(unusualHeapBtn, &QPushButton::clicked, this, [this] {
        HANDLE heap = HeapCreate(0, 0, 0);
        if (heap == INVALID_HANDLE_VALUE) {
            QMessageBox::critical(this, "错误", "创建堆失败");
            return;
        }
        auto lpMem = HeapAlloc(heap, 0, 1024);
        auto lpMen2 = lpMem, lpMen3 = lpMem;
        if (lpMem == nullptr) {
            CloseHandle(heap);
            QMessageBox::critical(this, "错误", "分配堆失败");
            return;
        }
        QMessageBox::information(this, "提示", "分配堆成功");

        QMessageBox::information(this, "提示", QString("第一次释放，结果：")
                                               + (HeapFree(heap, 0, lpMem) ? "成功" : "失败"));
        QMessageBox::information(this, "提示", QString("第二次释放，结果：")
                                               + (HeapFree(heap, 0, lpMen2) ? "成功" : "失败"));
        QMessageBox::information(this, "提示", QString("第三次释放，结果：")
                                               + (HeapFree(heap, 0, lpMen3) ? "成功" : "失败"));
        CloseHandle(heap);
    });

    auto unusualFileBtn1 = new QPushButton("文件异常-大量创建");
    unusualFileBtn1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(unusualFileBtn1, &QPushButton::clicked, this, [this] {
        QString dirName = QFileDialog::getExistingDirectory(this, "选择文件夹");
        if (dirName.isEmpty()) return;
        for (int i = 0; i < 1000; ++i) {
            QString path = dirName + "/" + QString::number(i);
            if (!CreateDirectory(gbk->fromUnicode(path).data(), nullptr)) {
                QMessageBox::critical(this, "错误", "创建文件夹失败");
                return;
            }
            for (int j = 0; j < 1000; ++j) {
                QString path2 = path + "/" + QString::number(j);
                HANDLE fileHandle = CreateFile(
                        gbk->fromUnicode(path2).data(),
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ,
                        nullptr,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        nullptr
                );
                if (fileHandle == INVALID_HANDLE_VALUE) {
                    QMessageBox::critical(this, "错误", "创建文件失败");
                    return;
                }
                CloseHandle(fileHandle);
            }
        }
        QMessageBox::information(this, "提示", "创建文件夹和文件成功");
    });

    auto unusualFileBtn2 = new QPushButton("CopyFile");
    unusualFileBtn2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(unusualFileBtn2, &QPushButton::clicked, this, [this] {
        TCHAR szPathOrig[MAX_PATH], szPathClone[MAX_PATH];
        GetModuleFileName(nullptr, szPathOrig, MAX_PATH);
        lstrcpy(szPathClone, szPathOrig);
        lstrcat(szPathClone, TEXT(".copy"));
        if (CopyFile(szPathOrig, szPathClone, FALSE))
            QMessageBox::information(this, "提示", "复制成功");
        else
            QMessageBox::critical(this, "错误", "复制失败");
    });

    auto unusualRegBtn1 = new QPushButton("注册表-启动项");
    unusualRegBtn1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(unusualRegBtn1, &QPushButton::clicked, this, [this] {
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0,
                         KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) {
            QMessageBox::critical(this, "错误", "打开注册表失败");
            return;
        }
        TCHAR szPathOrig[MAX_PATH];
        GetModuleFileName(nullptr, szPathOrig, MAX_PATH);
        if (RegSetValueEx(hKey, TEXT("test"), 0, REG_SZ, (LPBYTE) szPathOrig,
                          (lstrlen(szPathOrig) + 1) * sizeof(TCHAR)) != ERROR_SUCCESS) {
            QMessageBox::critical(this, "错误", "设置注册表失败");
            return;
        }
        RegCloseKey(hKey);
        QMessageBox::information(this, "提示", "设置注册表成功");
    });

    auto socketServerBtn = new QPushButton("SocketServer");
    socketServerBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(socketServerBtn, &QPushButton::clicked, this, [this] {
        auto server = new SocketWindow(true, this);
        server->show();
    });

    auto socketClientBtn = new QPushButton("SocketClient");
    socketClientBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(socketClientBtn, &QPushButton::clicked, this, [this] {
        auto client = new SocketWindow(false, this);
        client->show();
    });

    auto httpGetBtn = new QPushButton("HttpGet");
    httpGetBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(httpGetBtn, &QPushButton::clicked, this, [this] {
        auto urleditwindow = new QDialog(this);
        urleditwindow->setWindowTitle("请输入URL");
        auto urlEdit = new QLineEdit(urleditwindow);
        urlEdit->setPlaceholderText("http://ip-api.com/json/");
        auto okBtn = new QPushButton("确定", urleditwindow);
        connect(okBtn, &QPushButton::clicked, urleditwindow, [this, urleditwindow, urlEdit] {
            QString url = urlEdit->text();
            if (url.isEmpty()) {
                url = "http://ip-api.com/json/";
            }
            urleditwindow->close();
            QNetworkRequest request;
            auto naManager = new QNetworkAccessManager(this);
            request.setUrl(QUrl(url));
            auto reply = naManager->get(request);
            connect(reply, &QNetworkReply::finished, this, [this, reply] {
                if (reply->error() == QNetworkReply::NoError) {
                    auto data = reply->readAll();
                    auto window = new QDialog(this);
                    window->setWindowTitle("HttpGet");
                    auto textEdit = new QTextEdit(window);
                    textEdit->setPlainText(data);
                    auto layout = new QVBoxLayout(window);
                    layout->addWidget(textEdit);
                    window->setLayout(layout);
                    window->resize(800, 600);
                    window->show();
                } else {
                    QMessageBox::critical(this, "错误", QString("获取失败，错误：%1").arg(reply->errorString()));
                }
                reply->deleteLater();
            });
        });
        auto layout = new QVBoxLayout(urleditwindow);
        layout->addWidget(urlEdit);
        layout->addWidget(okBtn);
        urleditwindow->setLayout(layout);
        urleditwindow->resize(400,150);
        urleditwindow->show();
    });

    auto httpServerBtn = new QPushButton("HttpServer");
    httpServerBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(httpServerBtn, &QPushButton::clicked, this, [this] {
        auto server = new HttpWindow(this);
        server->show();
    });

    auto layout = new QGridLayout;
    layout->addWidget(messageBoxBtn, 0, 0);
    layout->addWidget(messageBoxExBtn, 1, 0);
    layout->addWidget(messageBoxIndirectBtn, 2, 0);
    layout->addWidget(messageBoxTimeoutBtn, 3, 0);
    layout->addWidget(heapCreateBtn, 0, 1);
    layout->addWidget(heapDestroyBtn, 1, 1);
    layout->addWidget(heapAllocBtn, 2, 1);
    layout->addWidget(heapFreeBtn, 3, 1);
    layout->addWidget(createFileBtn, 0, 2);
    layout->addWidget(createFileBtn2, 1, 2);
    layout->addWidget(regCreateKeyBtn, 2, 2);
    layout->addWidget(regCreateKeyBtn2, 3, 2);
    layout->addWidget(unusualHeapBtn, 0, 3);
    layout->addWidget(unusualFileBtn1, 1, 3);
    layout->addWidget(unusualFileBtn2, 2, 3);
    layout->addWidget(unusualRegBtn1, 3, 3);
    layout->addWidget(socketServerBtn, 0, 4);
    layout->addWidget(socketClientBtn, 1, 4);
    layout->addWidget(httpGetBtn, 2, 4);
    layout->addWidget(httpServerBtn, 3, 4);
    auto widget = new QWidget(this);
    widget->setLayout(layout);
    this->setCentralWidget(widget);
    this->resize(800, 600);
}

Mainwindow::~Mainwindow() {
    if (heapPtr != nullptr)
        HeapFree(heapHandle, 0, heapPtr);
    if (heapHandle != INVALID_HANDLE_VALUE)
        HeapDestroy(heapHandle);
}