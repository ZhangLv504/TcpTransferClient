#include <QApplication>
#include "mytcpclient.h"
#include "ipaddressui.h"
#include "widget.h"
#include "debughelper.h"
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    // QString filePath = QCoreApplication::applicationDirPath() + "/test12345.md";
    // DebugTimeSec() << filePath;
    
    // // // 自动生成测试文件
    // // QFile f(filePath);
    // // if (!f.open(QIODevice::WriteOnly)) { qWarning() << "Cannot create test file"; }
    // // else { f.write("Hello Qt TCP File Transfer!\n"); f.close(); }
    
    // MyTcpClient client;
    
    // QObject::connect(&client, &MyTcpClient::connectionChanged, [&](bool connected){
    //     DebugTimeSec() << "Connection changed:" << connected;
    //     if (connected) client.sendFile(filePath);
    // });
    
    // QObject::connect(&client, &MyTcpClient::fileProgress, [&](qint64 sent, qint64 total){
    //     DebugTimeSec() << "Progress:" << sent << "/" << total;
    // });
    
    // QObject::connect(&client, &MyTcpClient::errorOccurred, [&](const QString &err){
    //     qWarning() << "Error:" << err;
    // });
    
    // client.connectToServer("127.0.0.1", 12345);
    
    // // IpAddressUI w;
    // // w.show();
    
    Widget w;
    w.show();
    
    return a.exec();
}
