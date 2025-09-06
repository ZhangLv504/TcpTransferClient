#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mytcpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    
public slots:
    // 选择需要传输的文件
    void onOpenFileSlot();
    // 开始传输
    void onTransferFileSlot();
    
private:     
    // 调整文件路径高度
    void adjustFilePathHeight();
    // 更新连接按钮状态
    void updateConnectBtnState();

private:
    Ui::Widget *ui;
    
    MyTcpClient *m_client;       // 使用指针保存
    QStringList m_selectedFiles; // 保存选择的文件路径
};
#endif // WIDGET_H
