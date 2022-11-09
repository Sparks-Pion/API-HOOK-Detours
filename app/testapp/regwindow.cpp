//
// Created by 21527 on 22/09/20.
//
#include "regwindow.h"

#include <windows.h>

#include <QDebug>
#include <QLayout>
#include <QTreeWidget>
#include <QTextCodec>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>

namespace {
    QTextCodec *gbk = QTextCodec::codecForName("gbk");
}

RegWindow::RegWindow(QWidget *parent) :
        QDialog(parent) {
    this->setWindowFlags(Qt::Window);
    this->resize(950, 700);
    this->setWindowTitle("注册表");
    // 获取注册表
    auto layout = new QVBoxLayout(this);
    auto treeWidget = new QTreeWidget(this);
    treeWidget->setColumnCount(2);
    treeWidget->setHeaderLabels(QStringList() << "名称" << "类型" << "值");
    // 调整列宽 70% 30%
    treeWidget->setColumnWidth(0, 500);
    treeWidget->setColumnWidth(1, 200);
    treeWidget->setColumnWidth(2, 200);
    treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    // 添加 HKEY_CURRENT_CONFIG HKEY_USERS HKEY_CURRENT_USER HKEY_LOCAL_MACHINE HKEY_CLASSES_ROOT
    auto rootKey = new QTreeWidgetItem(treeWidget, QStringList() << "HKEY_CURRENT_CONFIG");
    treeWidget->addTopLevelItem(rootKey);
    rootKey = new QTreeWidgetItem(treeWidget, QStringList() << "HKEY_CURRENT_USER");
    treeWidget->addTopLevelItem(rootKey);
    rootKey = new QTreeWidgetItem(treeWidget, QStringList() << "HKEY_LOCAL_MACHINE");
    treeWidget->addTopLevelItem(rootKey);
    rootKey = new QTreeWidgetItem(treeWidget, QStringList() << "HKEY_USERS");
    treeWidget->addTopLevelItem(rootKey);
    rootKey = new QTreeWidgetItem(treeWidget, QStringList() << "HKEY_CURRENT_CONFIG");
    treeWidget->addTopLevelItem(rootKey);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    auto addBtn = new QPushButton("添加", this);
    auto delBtn = new QPushButton("删除", this);
    auto delTreeBtn = new QPushButton("递归删除", this);
    auto flushBtn = new QPushButton("刷新", this);
    auto layout2 = new QHBoxLayout();
    layout2->addWidget(addBtn);
    layout2->addWidget(delBtn);
    layout2->addWidget(delTreeBtn);
    layout2->addWidget(flushBtn);
    layout2->addWidget(buttonBox);
    layout->addWidget(treeWidget);
    layout->addLayout(layout2);
    this->setLayout(layout);

    connect(buttonBox, &QDialogButtonBox::accepted, this, [this] {
        this->close();
    });
    // 搜索子节点
    connect(treeWidget, &QTreeWidget::itemClicked, treeWidget,
            [treeWidget](QTreeWidgetItem *item, int column) {
                QTreeWidgetItem *nowItem = item;
                if (item->text(0) == "") return;
                if (item->childCount() == 0) {
                    // 获取完整路径
                    QString path;
                    while (item->parent() != nullptr) {
                        path = item->text(0) + "\\" + path;
                        item = item->parent();
                    }
//                        qDebug() << path << item->text(0);
                    HKEY hKey;
                    if (item->text(0) == "HKEY_CURRENT_CONFIG") {
                        hKey = HKEY_CURRENT_CONFIG;
                    } else if (item->text(0) == "HKEY_CURRENT_USER") {
                        hKey = HKEY_CURRENT_USER;
                    } else if (item->text(0) == "HKEY_LOCAL_MACHINE") {
                        hKey = HKEY_LOCAL_MACHINE;
                    } else if (item->text(0) == "HKEY_USERS") {
                        hKey = HKEY_USERS;
                    } else if (item->text(0) == "HKEY_CLASSES_ROOT") {
                        hKey = HKEY_CLASSES_ROOT;
                    } else {
                        return;
                    }
                    getRegTree(nowItem, hKey, path);
                    treeWidget->expandItem(nowItem);
                }
            });
    // 添加
    connect(addBtn, &QPushButton::clicked, treeWidget, [treeWidget] {
        auto item = treeWidget->currentItem();
        if (item == nullptr) {
            return;
        }
        auto Dialog = new QDialog(treeWidget);
        Dialog->setWindowTitle("添加");
        auto layout = new QGridLayout(Dialog);
        auto nameEdit = new QLineEdit(Dialog);
        layout->addWidget(new QLabel("名称", Dialog), 0, 0);
        layout->addWidget(nameEdit, 0, 1);
        auto typeEdit = new QComboBox(Dialog);
        layout->addWidget(new QLabel("类型", Dialog), 1, 0);
        layout->addWidget(typeEdit, 1, 1);
        auto valueEdit = new QLineEdit(Dialog);
        layout->addWidget(new QLabel("值", Dialog), 2, 0);
        layout->addWidget(valueEdit, 2, 1);
        typeEdit->addItems(
                QStringList() << "项" << "项(Ex)" << "项(Transacted)" << "REG_SZ(Ex)"
                              << "REG_DWORD(Ex)" << "REG_QWORD(Ex)" << "REG_BINARY(Ex)" << "REG_MULTI_SZ(Ex)"
                              << "REG_EXPAND_SZ(Ex)");
        auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
        layout->addWidget(buttonBox, 3, 0, 1, 2);
        Dialog->setLayout(layout);
        connect(buttonBox, &QDialogButtonBox::accepted, [=] {
            if (nameEdit->text().isEmpty()) {
                QMessageBox::warning(Dialog, "错误", "名称不能为空");
                return;
            }
            auto nowItem = item;
            // 获取完整路径
            QString path;
            while (nowItem->parent() != nullptr) {
                path = nowItem->text(0) + "\\" + path;
                nowItem = nowItem->parent();
            }
            HKEY hKey;
            if (nowItem->text(0) == "HKEY_CURRENT_CONFIG")
                RegOpenKeyEx(HKEY_CURRENT_CONFIG, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
            else if (nowItem->text(0) == "HKEY_CURRENT_USER")
                RegOpenKeyEx(HKEY_CURRENT_USER, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
            else if (nowItem->text(0) == "HKEY_LOCAL_MACHINE")
                RegOpenKeyEx(HKEY_LOCAL_MACHINE, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
            else if (nowItem->text(0) == "HKEY_USERS")
                RegOpenKeyEx(HKEY_USERS, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
            else if (nowItem->text(0) == "HKEY_CLASSES_ROOT")
                RegOpenKeyEx(HKEY_CLASSES_ROOT, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
            else return;

            LSTATUS status = 0;
            if (typeEdit->currentText() == "项") {
                status = RegCreateKey(hKey, gbk->fromUnicode(nameEdit->text()).data(), &hKey);
            } else if (typeEdit->currentText() == "项(Ex)") {
                status = RegCreateKeyEx(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, nullptr,
                                        REG_OPTION_NON_VOLATILE,
                                        KEY_ALL_ACCESS, nullptr, &hKey, nullptr);
            } else if (typeEdit->currentText() == "项(Transacted)") {
                status = RegCreateKeyTransacted(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, nullptr,
                                                REG_OPTION_NON_VOLATILE,
                                                KEY_ALL_ACCESS, nullptr, &hKey, nullptr, nullptr, nullptr);
            } else if (typeEdit->currentText() == "REG_SZ(Ex)") {
                status = RegSetValueEx(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, REG_SZ,
                                       (BYTE *) gbk->fromUnicode(valueEdit->text()).data(),
                                       gbk->fromUnicode(valueEdit->text()).size());
            } else if (typeEdit->currentText() == "REG_DWORD(Ex)") {
                DWORD value = valueEdit->text().toUInt();
                status = RegSetValueEx(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, REG_DWORD,
                                       (BYTE *) &value, sizeof(value));
            } else if (typeEdit->currentText() == "REG_QWORD(Ex)") {
                DWORD value = valueEdit->text().toULongLong();
                status = RegSetValueEx(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, REG_QWORD,
                                       (BYTE *) &value, sizeof(value));
            } else if (typeEdit->currentText() == "REG_BINARY(Ex)") {
                QByteArray value = QByteArray::fromHex(valueEdit->text().toUtf8());
                status = RegSetValueEx(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, REG_BINARY,
                                       (BYTE *) value.data(), value.size());
            } else if (typeEdit->currentText() == "REG_MULTI_SZ(Ex)") {
                QStringList value = valueEdit->text().split(";");
                QByteArray data;
                for (auto &i: value) {
                    data.append(gbk->fromUnicode(i));
                    data.append('\0');
                }
                data.append('\0');
                status = RegSetValueEx(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, REG_MULTI_SZ,
                                       (BYTE *) data.data(), data.size());
            } else if (typeEdit->currentText() == "REG_EXPAND_SZ(Ex)") {
                status = RegSetValueEx(hKey, gbk->fromUnicode(nameEdit->text()).data(), 0, REG_EXPAND_SZ,
                                       (BYTE *) gbk->fromUnicode(valueEdit->text()).data(),
                                       gbk->fromUnicode(valueEdit->text()).size());
            }
            RegCloseKey(hKey);
            if (status != ERROR_SUCCESS)
                QMessageBox::warning(Dialog, "错误", "创建失败");
            else
                QMessageBox::information(Dialog, "提示", "创建成功");
        });
        Dialog->resize(650, 250);
        Dialog->show();
    });
    // 删除
    connect(delBtn, &QPushButton::clicked, treeWidget, [treeWidget]() {
        if (treeWidget->currentItem() == nullptr) {
            QMessageBox::warning(treeWidget, "错误", "请选择要删除的项");
            return;
        }
        if (treeWidget->currentItem()->parent() == nullptr) {
            QMessageBox::warning(treeWidget, "错误", "根项不可删除");
            return;
        }
        QMessageBox::StandardButton button;
        button = QMessageBox::question(treeWidget, "提示", "确定要删除吗？", QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::No) return;

        // 获取完整路径
        auto *nowItem = treeWidget->currentItem()->parent();
        QString path;
        while (nowItem->parent() != nullptr) {
            path = nowItem->text(0) + "\\" + path;
            nowItem = nowItem->parent();
        }
        HKEY hKey;
        if (nowItem->text(0) == "HKEY_CURRENT_CONFIG")
            RegOpenKeyEx(HKEY_CURRENT_CONFIG, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_CURRENT_USER")
            RegOpenKeyEx(HKEY_CURRENT_USER, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_LOCAL_MACHINE")
            RegOpenKeyEx(HKEY_LOCAL_MACHINE, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_USERS")
            RegOpenKeyEx(HKEY_USERS, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_CLASSES_ROOT")
            RegOpenKeyEx(HKEY_CLASSES_ROOT, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else return;

        LSTATUS status;
        if (treeWidget->currentItem()->text(1) == "项")
            status = RegDeleteKey(hKey, gbk->fromUnicode(treeWidget->currentItem()->text(0)).data());
        else
            status = RegDeleteValue(hKey, gbk->fromUnicode(treeWidget->currentItem()->text(0)).data());

        RegCloseKey(hKey);
        if (status != ERROR_SUCCESS)
            QMessageBox::warning(treeWidget, "错误", "删除失败");
        else
            QMessageBox::information(treeWidget, "提示", "删除成功");
    });
    // 递归删除
    connect(delTreeBtn, &QPushButton::clicked, treeWidget, [treeWidget]() {
        if (treeWidget->currentItem() == nullptr || treeWidget->currentItem()->text(1) != "项") {
            QMessageBox::warning(treeWidget, "错误", "请选择要删除的项");
            return;
        }
        if (treeWidget->currentItem()->parent() == nullptr) {
            QMessageBox::warning(treeWidget, "错误", "根项不可删除");
            return;
        }
        QMessageBox::StandardButton button;
        button = QMessageBox::question(treeWidget, "提示", "确定要删除吗？", QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::No) return;

        // 获取完整路径
        auto *nowItem = treeWidget->currentItem()->parent();
        QString path;
        while (nowItem->parent() != nullptr) {
            path = nowItem->text(0) + "\\" + path;
            nowItem = nowItem->parent();
        }
        HKEY hKey;
        if (nowItem->text(0) == "HKEY_CURRENT_CONFIG")
            RegOpenKeyEx(HKEY_CURRENT_CONFIG, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_CURRENT_USER")
            RegOpenKeyEx(HKEY_CURRENT_USER, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_LOCAL_MACHINE")
            RegOpenKeyEx(HKEY_LOCAL_MACHINE, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_USERS")
            RegOpenKeyEx(HKEY_USERS, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else if (nowItem->text(0) == "HKEY_CLASSES_ROOT")
            RegOpenKeyEx(HKEY_CLASSES_ROOT, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hKey);
        else return;

        LSTATUS status = RegDeleteTree(hKey, gbk->fromUnicode(treeWidget->currentItem()->text(0)).data());
        RegCloseKey(hKey);
        if (status != ERROR_SUCCESS)
            QMessageBox::warning(treeWidget, "错误", "删除失败");
        else
            QMessageBox::information(treeWidget, "提示", "删除成功");
    });
    // 刷新
    connect(flushBtn, &QPushButton::clicked, treeWidget, [treeWidget]() {
        treeWidget->currentItem()->takeChildren();
        emit treeWidget->itemClicked(treeWidget->currentItem(), 0);
    });
}

RegWindow::~RegWindow() = default;

void RegWindow::getRegTree(QTreeWidgetItem *parent, HKEY hKey, const QString &path) {
    // 打开注册表
    HKEY hSubKey;
    if (RegOpenKeyEx(hKey, gbk->fromUnicode(path).data(), 0, KEY_ALL_ACCESS, &hSubKey) != ERROR_SUCCESS) {
        qDebug() << "打开注册表失败";
        RegCloseKey(hSubKey);
        return;
    }
    // 获取子键数量
    DWORD subKeyCount;
    if (RegQueryInfoKey(hSubKey, nullptr, nullptr, nullptr, &subKeyCount, nullptr, nullptr, nullptr, nullptr,
                        nullptr,
                        nullptr, nullptr) != ERROR_SUCCESS) {
        qDebug() << "获取注册表子键数量失败";
        RegCloseKey(hSubKey);
        return;
    }
    if (subKeyCount == 0) {
//        qDebug() << "注册表没有子键";
    }

    HKEY hValueKey;
    if (RegOpenKeyEx(hKey, gbk->fromUnicode(path).data(), 0, KEY_QUERY_VALUE | KEY_READ, &hValueKey) !=
        ERROR_SUCCESS) {
        qDebug() << "打开注册表失败";
        goto getvalueEnd;
    }
    DWORD valueCount;
    if (RegQueryInfoKey(hValueKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &valueCount, nullptr,
                        nullptr,
                        nullptr, nullptr) != ERROR_SUCCESS) {
        qDebug() << "获取注册表值数量失败";
        goto getvalueEnd;
    }
    if (valueCount == 0) {
//        qDebug() << "注册表没有值";
        goto getvalueEnd;
    }
//        qDebug() << "注册表值数量" << valueCount;
    // 获取值
    for (DWORD i = 0; i < valueCount; i++) {
        // 获取值名称长度
        DWORD valueNameSize = MAX_PATH;
        DWORD valueDataSize = 1024;
        char valueName[MAX_PATH]{};
        char valueData[1024]{};
        DWORD valueType;
        LSTATUS status = RegEnumValueA(hValueKey, i, valueName, &valueNameSize, nullptr, &valueType,
                                       (LPBYTE) valueData,
                                       &valueDataSize);
        switch (status) {
            case ERROR_SUCCESS:
                break;
            case ERROR_MORE_DATA:
                qDebug() << "ERROR_MORE_DATA";
                break;
            case ERROR_NO_MORE_ITEMS:
                qDebug() << "ERROR_NO_MORE_ITEMS";
                return;
            default:
                qDebug() << "获取注册表值名称长度失败" << status;
                return;
        }
        // 获取值名称
//            qDebug() << "值名称" << valueName;
//            qDebug() << "值名称长度" << valueData;
        QString valueTypeStr;
        QString valueStr;
        switch (valueType) {
            case REG_BINARY:
                valueTypeStr = "REG_BINARY";
                valueStr = QByteArray::fromRawData(valueData, (int) valueDataSize).toHex();
                break;
            case REG_DWORD:
                valueTypeStr = "REG_DWORD";
                valueStr = QString("0x") + QString::number(*reinterpret_cast<DWORD *>(valueData), 16);
                break;
            case REG_DWORD_BIG_ENDIAN:
                valueTypeStr = "REG_DWORD_BIG_ENDIAN";
                valueStr = QString("0x") + QString::number(*reinterpret_cast<DWORD *>(valueData), 16);
                break;
            case REG_EXPAND_SZ:
                valueTypeStr = "REG_EXPAND_SZ";
                valueStr = gbk->toUnicode(valueData);
                break;
            case REG_LINK:
                valueTypeStr = "REG_LINK";
                valueStr = gbk->toUnicode(valueData);
                break;
            case REG_MULTI_SZ:
                valueTypeStr = "REG_MULTI_SZ";
                valueStr = gbk->toUnicode(valueData);
                break;
            case REG_NONE:
                valueTypeStr = "REG_NONE";
                valueStr = QByteArray::fromRawData(valueData, (int) valueDataSize).toHex();
                break;
            case REG_QWORD:
                valueTypeStr = "REG_QWORD";
                valueStr = QString("0x") + QString::number(*reinterpret_cast<qint64 *>(valueData), 16);
                break;
            case REG_SZ:
                valueTypeStr = "REG_SZ";
                valueStr = gbk->toUnicode(valueData);
                break;
            default:
                valueTypeStr = "未知";
                valueStr = QByteArray::fromRawData(valueData, (int) valueDataSize).toHex();
                break;
        }
        // 添加值
        auto item = new QTreeWidgetItem(parent,
                                        QStringList() << gbk->toUnicode(valueName) << valueTypeStr << valueStr);
        parent->addChild(item);
    }
    getvalueEnd:
    RegCloseKey(hValueKey);
    // 获取子键
    for (DWORD i = 0; i < subKeyCount; i++) {
        // 获取子键名称
        char subKeyName[MAX_PATH]{};
        DWORD subKeyNameSize = MAX_PATH;
        if (RegEnumKeyEx(hSubKey, i, subKeyName, &subKeyNameSize, nullptr, nullptr, nullptr, nullptr) !=
            ERROR_SUCCESS) {
            qDebug() << "获取注册表子键名称失败";
            RegCloseKey(hSubKey);
            return;
        }
        // 获取子键
        char subKeyValue[MAX_PATH]{};
        DWORD subKeyValueSize = MAX_PATH;
        RegQueryValueEx(hSubKey, subKeyName, nullptr, nullptr, (LPBYTE) subKeyValue, &subKeyValueSize);
//        qDebug() << "子键名称: " << subKeyName;
//        qDebug() << "子键值: " << subKeyValue;
        auto item = new QTreeWidgetItem(parent, QStringList() << gbk->toUnicode(subKeyName) << "项" << gbk->toUnicode
                (subKeyValue));
        parent->addChild(item);
    }
    RegCloseKey(hSubKey);
}

