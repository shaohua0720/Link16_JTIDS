#pragma once

#include <string>
#include <memory>
#include "message/STDPMsg.h"

namespace link16 {
namespace protocol {

/**
 * @brief 协议层消息处理器接口
 */
class MessageProcessor {
public:
    // 构造函数
    MessageProcessor();
    
    // 析构函数
    virtual ~MessageProcessor();
    
    // 初始化
    bool initialize();
    
    // 关闭
    void shutdown();
    
    // 格式化消息
    bool formatMessage(int n, int m, const std::string& message, STDPMsg& stdpMsg);
    
    // 解析消息
    bool parseMessage(const STDPMsg& stdpMsg, int& n, int& m, std::string& message);
    
    // 验证消息
    bool validateMessage(const STDPMsg& stdpMsg);
    
    // 获取消息优先级
    int getMessagePriority(const STDPMsg& stdpMsg);
    
    // 设置发送方ID
    void setSenderID(int senderID);
    
    // 设置接收方ID
    void setReceiverID(int receiverID);

private:
    // 发送方ID
    int senderID;
    
    // 接收方ID
    int receiverID;
    
    // 初始化状态
    bool initialized;
    
    // 内部方法：构建HeaderWord
    bool buildHeaderWord(const STDPMsg& stdpMsg, int senderID);
    
    // 内部方法：构建InitialWord
    bool buildInitialWord(const STDPMsg& stdpMsg, int n, int m, const std::string& message);
    
    // 内部方法：构建ExtendWord
    bool buildExtendWord(const STDPMsg& stdpMsg, const std::string& message);
    
    // 内部方法：构建ContinueWord
    bool buildContinueWord(const STDPMsg& stdpMsg, const std::string& message);
};

} // namespace protocol
} // namespace link16