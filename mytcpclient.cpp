#pragma execution_character_set("utf-8")
#include "mytcpclient.h"
#include "protocalstruct.h"
#include "debughelper.h"
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QtEndian>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include <QThread>
#include <QTimer>

// 构造函数：初始化成员变量
MyTcpClient::MyTcpClient(QObject *parent)
    : QObject(parent)
    , m_socket(nullptr)     // 套接字初始化为空
    , m_file()              // 文件对象默认未打开
    , m_fileSize(0)         // 文件大小置0
    , m_totalSent(0)        // 已发送字节数置0
    , m_chunkIndex(0)       // 分片索引从0开始
    , m_currentSeq(0)       // 请求序列号从0开始
{}

MyTcpClient::~MyTcpClient() {
    disconnectFromServer(); // 析构时确保断开连接并释放资源
}

// 连接到服务器
void MyTcpClient::connectToServer(const QString &host, quint16 port) {
    if (m_socket) {
        disconnectFromServer(); // 如果之前有连接，先断开
    }
    m_socket = new QTcpSocket(this); // 创建新的 TCP 套接字
    
    // 绑定 socket 信号和槽函数
    connect(m_socket, &QTcpSocket::connected, this, &MyTcpClient::onConnected); // 连接成功
    connect(m_socket, &QTcpSocket::readyRead, this, &MyTcpClient::onReadyRead); // 有数据可读
    connect(m_socket, &QTcpSocket::disconnected, this, &MyTcpClient::onDisconnected); // 断开连接
    connect(m_socket,
            static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &MyTcpClient::onError); // 出错处理
    
    emit debugInfos(QString("正在连接服务器: %1:%2").arg(host).arg(port));
    m_socket->connectToHost(host, port); // 发起连接
}

// 主动断开连接
void MyTcpClient::disconnectFromServer() {
    if (m_socket) {
        emit debugInfos("断开服务器连接");
        m_socket->disconnectFromHost(); // 请求断开
        m_socket->deleteLater(); // 延迟删除，防止野指针
        m_socket = nullptr;
    }
    
    m_buffer.clear();  // 清空接收缓冲区
    
    if (m_file.isOpen()) {
        m_file.close(); // 如果有文件打开，关闭
    }
}

// 判断是否已连接
bool MyTcpClient::isConnected() const {
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

// 发送文件入口
void MyTcpClient::sendFile(const QString &filePath) {
    if (!isConnected()) {
        emit errorOccurred("未连接服务器，无法发送文件");
        return;
    }
    
    if (m_file.isOpen()) {
        m_file.close(); // 确保之前文件关闭
    }
    
    m_file.setFileName(filePath);
    if (!m_file.open(QIODevice::ReadOnly)) { // 只读方式打开文件
        emit errorOccurred(QString("无法打开文件: %1").arg(filePath));
        return;
    }
    
    // 初始化发送状态
    m_filePath = filePath;
    m_fileSize = m_file.size();
    m_totalSent = 0;
    m_chunkIndex = 0;
    
    emit debugInfos(QString("开始发送文件: %1 文件大小为: %2").arg(m_filePath).arg(m_fileSize));
    
    
    // 先发第一块
    sendNextChunk();
}

// 发送下一片数据
void MyTcpClient::sendNextChunk() {
    const qint64 chunkSize = 16 * 1024; // 每片最大16KB
    if (m_file.atEnd()) {
        // 文件已经读完，传输完成
        emit debugInfos(QString("文件发送完成，总大小: %1 字节").arg(m_fileSize));
        m_file.close();
        // 文件发送完成
        emit fileSent(m_filePath);
        return;
    }
    
    QByteArray chunk = m_file.read(chunkSize);  // 读取一片
    m_totalSent += chunk.size();                // 累计已发送字节
    
    // 构建 data JSON（包含文件名、分片编号、分片内容）
    QJsonObject data;
    data["file_name"] = QFileInfo(m_filePath).fileName();       // 文件名（不带路径）
    data["chunk_index"] = m_chunkIndex++;                       // 当前分片编号        
    data["chunk_data"] = QString(chunk.toBase64());             // 分片内容（Base64编码，避免二进制传输问题）
    
    // 构建请求 JSON（包含通用字段）
    QJsonObject request;                
    request["seq"] = ++m_currentSeq;                            // 递增序列号
    request["timestamp"] = QDateTime::currentSecsSinceEpoch();  // 当前时间戳
    request["cmd"] = "file_transfer";                           // 命令字
    request["data"] = data;                                     // 数据体
    request["status"] = 0;                                      // 状态：0=正常
    request["message"] = "sending";                             // 状态描述
    
    emit debugInfos(QString("发送分片: %1 已发送: %2/%3")
                        .arg(m_chunkIndex - 1).arg(m_totalSent).arg(m_fileSize));
    
    // 发送请求
    sendRequest(request);
    
    // 发出进度信号
    emit fileProgress(m_totalSent, m_fileSize);
}

// 发送请求到服务器
void MyTcpClient::sendRequest(const QJsonObject &data) {
    if (!isConnected()) {
        emit errorOccurred("未连接服务器，无法发送请求");
        return;
    }
    QByteArray packet = buildPacket(0, data); // 构建完整包（头 + JSON）
    
    m_socket->write(packet); // 写入 socket
    
    if (!m_socket->waitForBytesWritten(3000)) { // 等待写入完成，超时3秒
        emit errorOccurred("发送数据超时");
    }
}

// 连接成功时回调
void MyTcpClient::onConnected() {
    emit debugInfos("已成功连接到服务器");
    m_buffer.clear(); // 清空接收缓冲区
    m_currentSeq = 0; // 重置序列号
    emit connectionChanged(true);
}

// 收到数据时回调
void MyTcpClient::onReadyRead() {
    if (!m_socket) return;
    m_buffer.append(m_socket->readAll());  // 读出所有数据追加到缓冲区
    
    // 循环解析数据包（可能一次收到多个包）
    while (m_buffer.size() >= sizeof(ProtocolHeader)) {
        ProtocolHeader header;
        memcpy(&header, m_buffer.constData(), sizeof(ProtocolHeader)); // 解析协议头
        if (!header.isValid()) {
            emit errorOccurred("协议头无效，断开连接");
            m_socket->disconnectFromHost();
            return;
        }
        // 判断是否收到完整包（头 + 数据体）
        if (m_buffer.size() < sizeof(ProtocolHeader) + header.body_len) break;
        
        // 提取完整消息
        QByteArray message = m_buffer.mid(sizeof(ProtocolHeader), header.body_len);
        m_buffer.remove(0, sizeof(ProtocolHeader) + header.body_len);
        
        emit debugInfos(QString("收到服务端消息，长度: %1").arg(header.body_len));
        processResponse(message); // 交给响应处理函数
    }
}

// 处理服务端响应
void MyTcpClient::processResponse(const QByteArray &message) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(message, &error);
    if (error.error != QJsonParseError::NoError) {
        emit errorOccurred(QString("JSON解析失败: %1").arg(error.errorString()));
        return;
    }
    
    QJsonObject json = doc.object();
    if (json["cmd"].toString() == "file_transfer_ack") { // 服务端确认分片
        QJsonObject data = json["data"].toObject();
        int chunkIndex = data["chunk_index"].toInt();
        int status = data["status"].toInt();
        
        if (status == 0) {
            emit debugInfos(QString("收到分片ACK，分片编号: %1").arg(chunkIndex));
            
            /*
            1.这个函数运行在主线程（因为你的 MyTcpClient 对象没有移动到其他线程），所以：
                主线程被阻塞 1 秒。
                所有事件处理、信号槽、QTcpSocket::readyRead 都无法执行。
                TCP 缓冲区可能堆积，导致 服务器收到分片时乱序或多次 readyRead 触发生成多个文件。
            2.一句话：阻塞主线程 + TCP 异步 IO → 数据处理错乱。
            */
            // QThread::sleep(1); // 暂停 1 秒
            
            // 处理分片ACK时
            // QTimer::singleShot(1000, this, &MyTcpClient::sendNextChunk);
            
            sendNextChunk(); // 收到确认，继续发下一片
        } else {
            emit errorOccurred(QString("分片 %1 传输失败").arg(chunkIndex));
        }
    }
}

// 断开连接时回调
void MyTcpClient::onDisconnected() {
    emit debugInfos("服务器已断开连接");
    emit connectionChanged(false);
    if (m_file.isOpen()) {
        m_file.close();
    }
}

// 发生错误时回调
void MyTcpClient::onError(QAbstractSocket::SocketError) {
    QString err = m_socket ? m_socket->errorString() : "未知socket错误";
    // emit debugInfos(QString("发生错误: %1").arg(err));
    emit errorOccurred(err);
    if (m_file.isOpen()) {
        m_file.close();
    }
}

// 构建完整数据包
QByteArray MyTcpClient::buildPacket(quint8 type, const QJsonObject &data) {
    QJsonDocument doc(data);
    QByteArray body = doc.toJson(QJsonDocument::Compact);   // JSON正文
    
    ProtocolHeader header;
    header.type = type;                                     // 包类型
    header.body_len = static_cast<quint32>(body.size());    // JSON长度
    
    QByteArray packet;
    packet.append(reinterpret_cast<const char*>(&header), sizeof(header));  // 添加协议头
    packet.append(body);                                                    // 添加正文
    
    emit debugInfos(QString("构建数据包，类型: %1 长度: %2").arg(type).arg(header.body_len));
    return packet;
}
