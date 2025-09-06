#ifndef IPADDRESSUI_H
#define IPADDRESSUI_H

#include <QWidget>

namespace Ui {
class IpAddressUI;
}

class IpAddressUI : public QWidget
{
    Q_OBJECT

public:
    // 构造函数
    explicit IpAddressUI(QWidget *parent = nullptr);
    // 析构函数    
    ~IpAddressUI();
    
    // 设置样式的方法
    void setStyles(const QString &borderColor, const QString &bgColor, int borderRadius);
    // 设置默认IP（可在初始化或重置时调用）
    void setDefaultIP(const QString &defaultIp);

public:
    // 事件过滤器，用于捕获子控件的按键事件
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    // 默认推荐尺寸，用于布局参考
    virtual QSize sizeHint() const override;
    // 最小推荐尺寸，用于布局最小值
    virtual QSize minimumSizeHint() const override;
    
public:  
    // 获取完整IP地址
    QString getIP() const;
    // 设置IP地址
    void setIP(const QString &ip);
    
    // 设置背景颜色
    void setBgColor(const QString &bgColor);
    // 设置边框颜色
    void setBorderColor(const QString &borderColor);
    // 设置边框圆角角度
    void setBorderRadius(int borderRadius);
    
    // 清空四个输入框，并将焦点置于第一个
    void clear();
    
private slots:
    // 四个IP段文本值改变时触发
    void onTextChanged(const QString &text);
    
signals:
    // ip内容存在信号
    void ipTextChanged();
    
private:
    Ui::IpAddressUI *ui;
    
    QString ipAddr;                // 存储完整IP地址
    QString backGroundColor;       // 背景颜色
    QString borderColor;           // 边框颜色
    int borderRadius;              // 边框圆角角度
};

#endif // IPADDRESSUI_H
