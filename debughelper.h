#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#include <QDebug>
#include <QDateTime>
#include <QString>
#include <QStringList>

/*
在 C++ 中：
宏是 简单文本替换，不支持重载。
当你定义第二个宏时，编译器会直接用第二个宏覆盖第一个宏。
*/
#define DebugTimeSec()   (qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
#define DebugTimeMS()    (qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))

// 普通调试信息
#define LogWithTime(msg) (QString("[%1] %2") \
    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")) \
    .arg(msg))
// 错误信息，字体红色
#define LogWithTimeError(msg) (QString("<span style='color:red'>[%1] %2</span>") \
    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")) \
    .arg(msg))

// 单个参数转换为 QString
inline QString toQString(const QString &s) { return s; }
inline QString toQString(const char *s)    { return QString::fromUtf8(s); }
inline QString toQString(const std::string &s) { return QString::fromStdString(s); }
inline QString toQString(int v)            { return QString::number(v); }
inline QString toQString(double v)         { return QString::number(v); }
inline QString toQString(bool v)           { return v ? "true" : "false"; }
// 支持多个参数，自动拼接成一个 QString
template<typename... Args>
QString LogWithTimeMulti(Args&&... args) {
    QStringList parts{ QString::fromUtf8(std::forward<Args>(args))... };
    return QString("[%1] %2")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(parts.join(" "));
}

#endif // DEBUGHELPER_H
