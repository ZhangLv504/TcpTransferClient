#ifndef README_H
#define README_H

#if 0
客户端 (MyTcpClient)                          服务端 (TcpServer)
    |                                            |
    | connectToServer(host, port)                |
    |------------------TCP 连接----------------->|
    |                                            |
    | [onConnected()]                            |
    |                                            |
    | 打开文件 -> 分块 (16KB 一块)                |
    |                                            |
循环开始：每个文件块
    |                                            |
    | 读取一块数据 (chunk)                        |
    | base64编码 -> 封装 JSON -> 加协议头         |
    |                                            |
    | sendRequest(packet)                        |
    |------------------发送数据----------------->|
    |                                            |
    |                          解析协议头、解析JSON|
    |                          保存文件块         |
    |                          构造ACK响应        |
    |<------------------发送ACK------------------|
    |                                            |
    | [onReadyRead()]                            |
    | processResponse()                          |
    | 输出：Chunk N transfer succeeded           |
    |                                            |
循环结束：直到所有块发送完成
    |                                            |
    | 文件传输完成                               |
    |                                            |
    | disconnectFromServer()                     |
    |------------------断开连接----------------->|
    |                                            |
#endif

#if 0
    SocketError error() const;

Q_SIGNALS:
#if QT_DEPRECATED_SINCE(5,15)
    QT_DEPRECATED_NETWORK_API_5_15_X("Use QAbstractSocket::errorOccurred(QAbstractSocket::SocketError) instead")
    void error(QAbstractSocket::SocketError);
#endif
    void errorOccurred(QAbstractSocket::SocketError);

同时存在error函数和error信号，故需要在使用error信号时明确区分error(QAbstractSocket::SocketError)
static_cast<>(void(QAbstractSocket::*)(QAbstractSocket::SocketError))(&QAbstractSocket::error)
#endif

#endif // README_H
