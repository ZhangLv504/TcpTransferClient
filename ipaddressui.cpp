#include "ipaddressui.h"
#include "ui_ipaddressui.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QKeyEvent>
#include <QDebug>

// 构造函数
IpAddressUI::IpAddressUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IpAddressUI)
{
    // 初始化UI界面
    ui->setupUi(this);
    
    // 默认样式设置
    backGroundColor = "#FFFFFF";  // 默认背景白色
    borderColor = "#A6B5B8";      // 默认边框颜色
    borderRadius = 3;             // 默认圆角3像素
    
    // 连接四个输入框的文本改变信号到槽函数
    connect(ui->txtIP1, &QLineEdit::textChanged, this, &IpAddressUI::onTextChanged);
    connect(ui->txtIP2, &QLineEdit::textChanged, this, &IpAddressUI::onTextChanged);
    connect(ui->txtIP3, &QLineEdit::textChanged, this, &IpAddressUI::onTextChanged);
    connect(ui->txtIP4, &QLineEdit::textChanged, this, &IpAddressUI::onTextChanged);
    
    // 使用正则表达式设置IP段校验
    QString pattern = "(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})"; // 匹配0-255的数字
    QRegularExpression regExp(pattern);
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regExp, this);
    // 给每个输入框设置校验器
    ui->txtIP1->setValidator(validator);
    ui->txtIP2->setValidator(validator);
    ui->txtIP3->setValidator(validator);
    ui->txtIP4->setValidator(validator);
    
    // 安装事件过滤器，用于监听按键（如"."或退格键）
    ui->txtIP1->installEventFilter(this);
    ui->txtIP2->installEventFilter(this);
    ui->txtIP3->installEventFilter(this);
    ui->txtIP4->installEventFilter(this);
    
    // 设置样式
    setStyles(borderColor, backGroundColor, borderRadius);
    // 设置默认 IP
    setDefaultIP("127.0.0.1");
}

// 析构函数
IpAddressUI::~IpAddressUI()
{
    delete ui;
}

// 设置样式
void IpAddressUI::setStyles(const QString &borderColor, const QString &bgColor, int borderRadius)
{
    // 创建 QSS 样式表
    QStringList qss;
    qss.append(QString("QFrame#frameIP{border:1px solid %1;border-radius:%2px;}").arg(borderColor).arg(borderRadius));
    qss.append(QString("QLabel{min-width:15px;background-color:%1;}").arg(bgColor));
    qss.append(QString("QLineEdit{background-color:%1;border:none;}").arg(bgColor));
    qss.append(QString("QLineEdit#txtIP1{border-top-left-radius:%1px;border-bottom-left-radius:%1px;}").arg(borderRadius));
    qss.append(QString("QLineEdit#txtIP4{border-top-right-radius:%1px;border-bottom-right-radius:%1px;}").arg(borderRadius));
    
    // 设置样式
    this->setStyleSheet(qss.join(""));
    
    // // 还可以单独为某个控件设置样式（例如 frame、lineEdit）
    // frame->setStyleSheet(qss.join(""));
    // txtIP1->setStyleSheet(qss.at(2)); // 设置 QLineEdit 样式
    // txtIP4->setStyleSheet(qss.at(3)); // 设置 txtIP4 样式
}

// 设置默认IP
void IpAddressUI::setDefaultIP(const QString &defaultIp)
{
    // 使用和 setIP 相同的合法性检查
    QRegularExpression re("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    if(!re.match(defaultIp).hasMatch())
    {
        qWarning() << "默认IP格式不合法:" << defaultIp;
        return;
    }
    
    // 只有在当前 ipAddr 为空或控件尚未设置 IP 时才设置默认值
    if(this->ipAddr.isEmpty())
    {
        this->ipAddr = defaultIp;
        
        // 将默认 IP 填入各个输入框
        QStringList list = defaultIp.split(".");
        ui->txtIP1->setText(list.at(0));
        ui->txtIP2->setText(list.at(1));
        ui->txtIP3->setText(list.at(2));
        ui->txtIP4->setText(list.at(3));
    }
}

// 事件过滤器
bool IpAddressUI::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QLineEdit* txt = (QLineEdit*)watched;
        // 只处理四个IP输入框的按键事件
        if(txt == ui->txtIP1 || txt == ui->txtIP2 || txt == ui->txtIP3 || txt == ui->txtIP4)
        {
            QKeyEvent* key = (QKeyEvent*)event;
            
            // 按下"."时自动跳到下一个输入框
            if(key->text() == ".") {
                this->focusNextChild();
            }
            
            // 按下退格键，如果当前框内容长度<=1，则焦点退回上一个输入框
            if(key->key() == Qt::Key_Backspace) {
                if (txt->text().length() <= 1) {
                    this->focusNextPrevChild(false);
                }
            }
        }
    }
    
    // 交给父类处理其它事件
    return QWidget::eventFilter(watched, event);
}

// 推荐尺寸
QSize IpAddressUI::sizeHint() const
{
    return QSize(225, 35);  // 默认宽高
}

// 最小推荐尺寸
QSize IpAddressUI::minimumSizeHint() const
{
    return QSize(225, 35);  // 默认宽高
}

// 获取完整IP
QString IpAddressUI::getIP() const
{
    return this->ipAddr;
}

// 设置IP地址
void IpAddressUI::setIP(const QString &ip)
{
    // 正则校验IP格式是否合法
    QRegularExpression re("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    if(!re.match(ip).hasMatch())
    {
        qWarning() << "IP格式不合法:" << ip;
        return; // 不合法则直接返回
    }
    
    // 如果IP与当前不同，则更新
    if(this->ipAddr != ip)
    {
        this->ipAddr = ip;
        
        // 将IP分段填入四个输入框
        QStringList list = ip.split(".");
        ui->txtIP1->setText(list.at(0));
        ui->txtIP2->setText(list.at(1));
        ui->txtIP3->setText(list.at(2));
        ui->txtIP4->setText(list.at(3));
    }
    
}

// 设置背景颜色
void IpAddressUI::setBgColor(const QString &bgColor)
{
    if (this->backGroundColor != bgColor) {
        this->backGroundColor = bgColor;
    }  
}

// 设置边框颜色
void IpAddressUI::setBorderColor(const QString &borderColor)
{
    if (this->borderColor != borderColor) {
        this->borderColor = borderColor;
    }  
}

// 设置边框圆角
void IpAddressUI::setBorderRadius(int borderRadius)
{
    if (this->borderRadius != borderRadius) {
        this->borderRadius = borderRadius;
    }  
}

// 清空四个输入框
void IpAddressUI::clear()
{
    ui->txtIP1->clear();
    ui->txtIP2->clear();
    ui->txtIP3->clear();
    ui->txtIP4->clear();
    // 焦点回到第一个
    ui->txtIP1->setFocus();
}

// 四个输入框文本改变槽函数
void IpAddressUI::onTextChanged(const QString &text)
{
    int len = text.length();
    int value = text.toInt();
    
    // 当当前网段输入长度=3且数值>=100时，自动跳到下一个输入框
    if (len == 3) {
        if (value >= 100 && value <= 255) {
            this->focusNextChild();
        }
    }
    
    // 每次文本改变都更新完整IP地址
    ipAddr = QString("%1.%2.%3.%4").arg(ui->txtIP1->text()).arg(ui->txtIP2->text())
                                   .arg(ui->txtIP3->text()).arg(ui->txtIP4->text());
    
    emit ipTextChanged();
}
