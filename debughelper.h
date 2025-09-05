#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#include <QDebug>
#include <QDateTime>

// 宏定义：输出带时间的调试信息
// 参数 showMilliSec = true 表示显示毫秒，false 表示只显示到秒
#define DebugTime(msg, showMilliSec) \
do { \
        QString timeStr; \
        if (showMilliSec) { \
            timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"); \
    } else { \
            timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); \
    } \
        qDebug() << timeStr << msg; \
} while(0)


// 可变参数宏，支持多个输出值
// 参数 showMilliSec = true 表示显示毫秒
#define DebugTimeMulti(showMilliSec, ...) \
do { \
        QString timeStr = showMilliSec ? \
                         QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") : \
                         QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); \
        qDebug() << timeStr << __VA_ARGS__; \
} while(0)

#endif // DEBUGHELPER_H
