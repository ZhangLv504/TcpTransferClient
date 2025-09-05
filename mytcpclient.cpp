#include "mytcpclient.h"
#include "protocalstruct.h"
#include <QDebug>
#include <QDateTime>
#include <QtEndian>

#define DebugTime qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")

// 构造函数，初始化 socket 为 nullptr，序列号置 0
MyTcpClient::MyTcpClient(QObject *parent)
    : QObject(parent)
    , m_socket(nullptr)
    , m_currentSeq(0)
{}

// 析构函数，释放连接
MyTcpClient::~MyTcpClient()
{
    // 析构时确保连接已断开、资源已释放
    // 注意：deleteLater() 会在事件循环空闲时删除对象，此处不用显式等待
    disconnectFromServer();
}

// 连接服务器
void MyTcpClient::connectToServer(const QString &host, quint16 port) {
    if (m_socket)
    {
        // 如果之前已经创建过 socket（可能是上次连接遗留），先做一次完整清理
        // 这样可以避免意外使用旧连接的状态或信号槽重复连接
        disconnectFromServer();
    }
    
    // 为当前客户端创建一个新的 QTcpSocket，并把其生命周期托管给 this
    m_socket = new QTcpSocket(this);
    
    // 绑定信号槽：
    // readyRead 在每次收到新数据时触发；
    // connected/disconnected 标识连接生命周期；
    // error 信号用于把底层错误上抛到 UI 或日志。
    connect(m_socket, &QTcpSocket::connected, this, &MyTcpClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &MyTcpClient::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &MyTcpClient::onDisconnected);
    connect(m_socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &MyTcpClient::onError);
    
    // 发起异步连接请求：
    // 1) 成功后会触发 connected()
    // 2) 如立即出错（例如 DNS 失败）可能先触发 error()
    m_socket->connectToHost(host, port);
    
    // 如需同步等待，可调用 waitForConnected(timeout)，但会阻塞线程；此处保持异步更稳妥
}

// 主动断开连接
void MyTcpClient::disconnectFromServer() {
    if (m_socket) {
        // 请求优雅断开：会等待对端响应 FIN；如果对端无响应，Qt 仍可能在超时后强制关闭
        m_socket->disconnectFromHost();
        // 异步删除 socket 对象，防止在信号回调（例如 onDisconnected）还未返回时就立即 delete
        m_socket->deleteLater();
        m_socket = nullptr;
    }
    // 清空收包缓冲，避免旧数据污染下一次连接
    m_buffer.clear();
}

// 是否处于连接状态
bool MyTcpClient::isConnected() const {
    // ConnectedState 表示 TCP 三次握手已完成，可以收发数据
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

// 发送文件
void MyTcpClient::sendFile(const QString &filePath) {
    // 1) 基本状态校验：必须先处于连接状态
    if (!isConnected()) {
        emit errorOccurred("Not connected to server");
        return;
    }
    
    // 2) 打开要发送的文件（只读）
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        // 打开失败常见原因：路径不存在、权限不足、文件被占用等
        emit errorOccurred(QString("Cannot open file: %1").arg(filePath));
        return;
    }
    
    // 3) 发送策略：按块读取和发送，避免一次性把大文件读入内存
    //    这里选择 16KB 作为默认分块大小；可按网络/服务器能力调整。
    //    注意：base64 会引入约 33% 体积膨胀，影响带宽占用。
    const qint64 chunkSize = 16 * 1024;
    int chunkIndex = 0;             // 分块序号（从 0 递增），用于服务端重组和 ACK
    qint64 totalSent = 0;           // 已发送的原始字节总数（未 base64 前）
    qint64 fileSize = file.size();  // 文件总大小
    
    // 4) 按块读取文件并逐块发送
    //    这里采用“即读即发”的简单流控策略，未等待 ACK 即发送下一块（fire-and-forget 风格）。
    //    如果你需要严格的可靠性（例如必须等 ACK 再发下一块），可以：
    //       a) 在此处等待特定 seq/chunkIndex 的 ACK 信号；
    //       b) 或者维护一个发送队列与重传机制。
    while (!file.atEnd()) {
        // 从文件读取一个块（最后一块大小可能小于 chunkSize）
        QByteArray chunk = file.read(chunkSize);
        // 更新已发送进度（统计的是原始字节数量）
        totalSent += chunk.size();
        
        // 5) 组装本块的 JSON 数据：包含文件名、分块索引、以及分块数据的 base64 字符串
        //    之所以使用 base64，是为了把二进制数据安全地放进 JSON 文本。
        QJsonObject data;
        data["file_name"] = QFileInfo(filePath).fileName(); // 只传文件名，路径由接收端自行决定保存位置
        data["chunk_index"] = chunkIndex++;                 // 递增的分块序号，服务端据此写入正确位置
        data["chunk_data"] = QString(chunk.toBase64());     // 二进制 -> base64 字符串（UTF-8 安全）
        
        
        // 6) 外层请求封装：包含自增 seq、时间戳、命令字等，可以用于服务端路由和追踪
        QJsonObject request;
        request["seq"] = (int)++m_currentSeq;                       // 每个请求都有唯一序列号，便于服务端回溯
        request["timestamp"] = QDateTime::currentSecsSinceEpoch();  // 秒级时间戳，便于日志/超时分析
        request["cmd"] = "file_transfer";                           // 命令字：服务端可根据 cmd 选择处理器
        request["data"] = data;                                     // 负载：真正的业务数据
        request["status"] = 0;                                      // 这里的 status 是请求侧自定字段，非错误码
        request["message"] = "sending";                             // 描述性文本，方便排查
        
        // 7) 通过统一的 sendRequest 进行发送（会添加自定义协议头）
        sendRequest(request);
        // 8) 对外广播进度（便于 UI 显示进度条）
        emit fileProgress(totalSent, fileSize);
        // 9) 适度休眠：避免在高带宽/低延迟环境下把服务端写缓存瞬间打满。
        //    如果需要更精细的流控，可用：
        //       - socket->bytesToWrite() 动态判断待写缓存
        //       - 限速算法（令牌桶/漏桶）
        QThread::msleep(10);
    }
    
    file.close();
}

// 发送 JSON 请求（底层加协议头）
void MyTcpClient::sendRequest(const QJsonObject &data) {
    // 1) 再次确认连接状态（调用方也做过，但这里双保险）
    if (!isConnected()) {
        emit errorOccurred("Not connected to server");
        return;
    }
    
    // 2) 通过 buildPacket(type=0) 构造“协议头 + JSON 体”的完整数据包
    //    type 字段可用于区分业务类型（此处传 0，具体语义由协议双方约定）
    QByteArray packet = buildPacket(0, data);
    
    // 3) write 是异步写：数据进入 Qt 的待写缓冲，Qt 会在合适时机写入内核 socket 缓冲
    //    注意：write 返回后并不代表数据已真正发到对端
    m_socket->write(packet);
    
    // 4) waitForBytesWritten 会阻塞当前线程直到有数据写入内核或超时。
    //    对 GUI 线程来说，长时间阻塞会卡 UI，生产上可考虑去掉或缩短超时。
    if (!m_socket->waitForBytesWritten(3000))
    {
        // 超时并不意味着 0 字节写入，也可能只是写入未完成；此处统一抛出警告
        emit errorOccurred("Write timeout");
    }
}

// 连接成功回调
void MyTcpClient::onConnected() {
    // 1) 连接建立后，重置内部状态，避免上次连接的残留数据影响本次会话
    qDebug() << "连接服务器成功!"; 
    m_buffer.clear(); 
    m_currentSeq = 0;
    
    // 2) 通知 UI 或上层逻辑：连接已建立
    emit connectionChanged(true); 
}

// 可读数据回调
void MyTcpClient::onReadyRead() {
    if (!m_socket)
        return;
    
    // 1) 把本次收到的所有字节追加到 m_buffer（累积缓冲），因为 TCP 是“面向字节流”的：
    //    - 可能一次 readAll() 读到多个完整包（粘包）
    //    - 也可能只读到半个包（半包）
    m_buffer.append(m_socket->readAll());
    
    // 2) 循环尝试“剥离”出一个又一个完整包：必须先检查缓冲区是否有完整的协议头，
    //    再检查是否有足够的 body_len 长度的消息体。
    while (m_buffer.size() >= sizeof(ProtocolHeader)) {
        ProtocolHeader header;
        // 直接 memcpy 是最快的方式；也可改用 QDataStream 并设置大小端
        memcpy(&header, m_buffer.constData(), sizeof(ProtocolHeader));
        
        // 3) 校验协议头是否合规（魔数 / 版本）
        if (!header.isValid()) {
            // 如果头不合法，继续读没有意义，直接断开避免死循环或安全问题
            emit errorOccurred("Invalid protocol header");
            m_socket->disconnectFromHost();
            return;
        }
        
        // 4) 进一步的健壮性校验（可选但强烈建议）：
        //    比如限制 header.body_len 最大值，防止恶意包导致内存分配过大（DoS）
        //    例如：if (header.body_len > (10 * 1024 * 1024)) { ... }
        //    这里保持与你的原始逻辑一致，未做额外限制。
        
        // 5) 如果当前缓冲区不够一个完整包（头 + 体），先退出等待更多字节
        if (m_buffer.size() < sizeof(ProtocolHeader) + header.body_len)
            break;
        
        // 6) 从缓冲区切出一个完整消息体（不含头）
        QByteArray message = m_buffer.mid(sizeof(ProtocolHeader), header.body_len);
        
        // 7) 移除已处理的“头+体”，缓冲区可能仍残留后续包的数据（继续 while 解析）
        m_buffer.remove(0, sizeof(ProtocolHeader) + header.body_len);
        
        // 8) 把消息体交给 JSON 处理逻辑
        processResponse(message);
    }
}


// 处理服务端返回的数据
void MyTcpClient::processResponse(const QByteArray &message) {
    // 1) 尝试把字节数组解析为 JSON 文档
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(message, &error);
    if (error.error != QJsonParseError::NoError) {
        // 解析失败通常来自于：编码不合法、截断、协议不一致等
        emit errorOccurred(QString("JSON parse error: %1").arg(error.errorString())); 
        return; 
    }
    
     // 2) 提取 JSON 对象，依据 cmd 字段做路由
    QJsonObject json = doc.object();
    
    // 3) 这里只处理 "file_transfer_ack"（文件分块确认）：
    //    - data.chunk_index：服务端确认的是哪一块
    //    - data.status：0=成功，非 0=失败
    if (json["cmd"].toString() == "file_transfer_ack") {
        QJsonObject data = json["data"].toObject();
        int chunkIndex = data["chunk_index"].toInt();
        int status = data["status"].toInt();
        
        if (status == 0)
            // 成功：可在这里做更精细的统计（例如记录 ACK RTT、吞吐量等）
            qDebug() << "Chunk" << chunkIndex << "transfer succeeded";
        else
            // 失败：把失败的分块上报给上层；必要时可在此触发重传逻辑
            emit errorOccurred(QString("Chunk %1 failed").arg(chunkIndex));
    } else {
        // 4) 未知/未处理的 cmd：
        //    生产上可以在这里记录日志或做默认处理，避免静默丢弃消息
        // qDebug() << "Unhandled cmd:" << json["cmd"].toString();
    }
}

// 断开回调
void MyTcpClient::onDisconnected() {
    // 1) 连接被动或主动关闭都会触发此回调（包括对端关闭、网络中断等）
    qDebug() << "Disconnected";
    
    // 2) 通知 UI 或上层逻辑：连接已断开，可更新按钮状态/重连等
    emit connectionChanged(false); 
}

// 错误回调
void MyTcpClient::onError(QAbstractSocket::SocketError) {
    // 1) errorString() 返回最近一次错误的人类可读字符串
    // 2) 某些情况下 error 可能在 disconnected 之后才到达，因此做空指针保护
    QString err = m_socket ? m_socket->errorString() : "Socket error";
    emit errorOccurred(err);
}

// 构造完整协议包（头+JSON体）
QByteArray MyTcpClient::buildPacket(quint8 type, const QJsonObject &data) {
    // 1) JSON 序列化为紧凑格式字节数组（无多余空白，减少体积）
    QJsonDocument doc(data);
    QByteArray body = doc.toJson(QJsonDocument::Compact);
    
    // 2) 填充协议头：type 由调用方指定；body_len 为 JSON 字节长度
    ProtocolHeader header;
    header.type = type;
    header.body_len = static_cast<quint32>(body.size());
    // // 其余字段（magic/version/flags/reserved）在构造函数中已给默认值
    // 所有多字节字段（magic、body_len）都要用大端。
    // 单字节字段（version、type、flags、reserved）不用转换。
    
    // 3) 组合为“头 + 体”的连续字节流；对端以相同结构体解析即可
    QByteArray packet;
    packet.append(reinterpret_cast<const char*>(&header), sizeof(header));
    packet.append(body);
    
    // 4) 返回完整数据包；调用方负责交给 socket 发送
    return packet;
}






