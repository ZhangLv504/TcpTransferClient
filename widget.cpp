#pragma execution_character_set("utf-8")
#include "widget.h"
#include "./ui_widget.h"

#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include "debughelper.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 指定 parent，Qt 会自动释放
    m_client = new MyTcpClient(this);
    
    // 连接信号槽
    connect(ui->openFileBtn, &QPushButton::clicked, this, &Widget::onOpenFileSlot);
    connect(ui->transferBtn, &QPushButton::clicked, this, &Widget::onTransferFileSlot);
    connect(ui->connectBtn, &QPushButton::clicked, this, [this](){
        // 判断 socket 状态
        if (m_client->isConnected()) {
            // 已连接 -> 断开
            ui->infosTextEdit->append(LogWithTime("正在断开服务器..."));
            m_client->disconnectFromServer();
            ui->connectBtn->setText("连接服务器"); // 更新按钮文本
        } else {
            // 未连接 -> 尝试连接
            QString ip = ui->ipaddrUI->getIP();
            quint16 port = ui->portLE->text().toUShort();
            
            ui->infosTextEdit->append(LogWithTime("正在连接服务器..."));
            m_client->connectToServer(ip, port);
            ui->connectBtn->setText("断开连接"); // 先改按钮显示“断开”，等连接成功也可以更新
        }
    });
    
    // 处理调试信息
    connect(m_client, &MyTcpClient::debugInfos, this, [this](const QString &info){
        ui->infosTextEdit->append(LogWithTime(info)); // LogWithTime 在 UI 层添加时间
    });
    
    // 监听 IP 和 Port 输入变化
    connect(ui->ipaddrUI, &IpAddressUI::ipTextChanged, this, &Widget::updateConnectBtnState);
    connect(ui->portLE, &QLineEdit::textChanged, this, &Widget::updateConnectBtnState);
    
    // 处理连接状态改变信息
    connect(m_client, &MyTcpClient::connectionChanged, this, [&](bool connected){
        ui->infosTextEdit->append(LogWithTime(
            QString("连接状态: %1").arg(connected ? "已连接" : "已断开")
            ));
        if(connected){
            ui->connectBtn->setText("断开连接");
        } else {
            ui->connectBtn->setText("连接服务器");
        }
    });
    
    // 处理文件传输进度信息
    connect(m_client, &MyTcpClient::fileProgress, this, [&](qint64 sent, qint64 total){
        DebugTimeSec() << "Progress:" << sent << "/" << total;
        ui->transferProcessBar->setMaximum(total);
        ui->transferProcessBar->setValue(sent);
    });
    
    // 处理错误信息
    connect(m_client, &MyTcpClient::errorOccurred, this, [&](const QString &err){
        ui->infosTextEdit->append(LogWithTimeError(QString("错误: %1").arg(err)));
    });
}

Widget::~Widget()
{
    delete ui;
    // 不需要手动 delete m_client
    // 因为构造时给了 parent = this，Qt 会自动回收
}

// 选择需要传输的文件
void Widget::onOpenFileSlot()
{
    // 获取程序所在目录
    QString exeDir = QCoreApplication::applicationDirPath();
    
    // 弹出文件对话框，允许选择多个文件
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,                   // 父窗口
        tr("选择文件"),          // 对话框标题
        exeDir,                 // 默认路径
        tr("所有文件(*.*)")      // 文件过滤     
    );
    
    if(!filePaths.empty())
    {
        m_selectedFiles = filePaths;  // 保存
        // 将每个文件路径换行显示在 QPlainTextEdit 上
        ui->filePathPTE->clear(); // 先清空
        for (const QString& path : filePaths) {
            ui->filePathPTE->appendPlainText(path);
        }
        // adjustFilePathHeight();
        ui->filePathPTE->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
}

// 开始传输
void Widget::onTransferFileSlot()
{
    if (m_selectedFiles.isEmpty()) {
        ui->infosTextEdit->append(LogWithTimeError("请先选择文件！"));
        return;
    }
    
    if (!m_client->isConnected()) {
        ui->infosTextEdit->append(LogWithTimeError("请先连接服务器！"));
        return;
    }
    
    for (const QString &file : m_selectedFiles) {
        m_client->sendFile(file);
    }
}

// 调整文件路径高度
void Widget::adjustFilePathHeight()
{
    QPlainTextEdit *pte = ui->filePathPTE;
    
    // 文档高度
    qreal newHeight  = static_cast<int>(pte->document()->size().height());

    int maxHeight = 200;
    
    if (newHeight > maxHeight) {
        newHeight = maxHeight;
        pte->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else {
        pte->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    
    pte->setFixedHeight(newHeight);
}

// 更新连接按钮状态
void Widget::updateConnectBtnState()
{
    QString ip = ui->ipaddrUI->getIP().trimmed();
    QString port = ui->portLE->text().trimmed();
    
    // 判断是否都有值
    bool enable = !ip.isEmpty() && !port.isEmpty();
    ui->connectBtn->setEnabled(enable);
}
