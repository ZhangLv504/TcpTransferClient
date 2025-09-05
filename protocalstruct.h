#ifndef PROTOCALSTRUCT_H
#define PROTOCALSTRUCT_H

#include <QtGlobal>

// 自定义协议头（定长，方便解析）
// 使用 #pragma pack(push, 1) 保证结构体紧凑排列，不会被编译器填充额外字节
#pragma pack(push, 1)
struct ProtocolHeader {
    quint16 magic;      // 固定为 0x55AA，用于校验数据是否正确
    quint8 version;     // 协议版本，当前=1
    quint8 type;        // 消息类型（客户端自定义，例如 0=file，1=心跳 等）
    quint8 flags;       // 标志位（可扩展用）
    quint8 reserved;    // 保留字段（预留扩展）
    quint32 body_len;   // 消息体长度（字节数）
    
    ProtocolHeader() {
        magic = 0x55AA;
        version = 1;
        type = 0;
        flags = 0;
        reserved = 0;
        body_len = 0;
    }
    
    // 校验协议头是否合法
    bool isValid() const {
        return magic == 0x55AA && version == 1;
    }
};
#pragma pack(pop)

#endif // PROTOCALSTRUCT_H
