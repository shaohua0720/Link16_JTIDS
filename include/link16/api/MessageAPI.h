#pragma once
#include <string>
#include <cstdint>

namespace link16 {
namespace api {

// 消息API
class MessageAPI {
public:
    // 获取单例实例
    static MessageAPI& getInstance();
    
    // 初始化
    bool initialize();
    
    // 关闭
    void shutdown();
    
    // 编码消息
    bool encodeMessage(int32_t n, int32_t m, const std::string& message, std::string& encodedMessage);
    
    // 解码消息
    bool decodeMessage(const std::string& encodedMessage, int32_t& n, int32_t& m, std::string& message);
    
    // 获取消息类型描述
    std::string getMessageTypeDescription(int32_t n, int32_t m);
    
    // 检查消息类型是否有效
    bool isValidMessageType(int32_t n, int32_t m);

private:
    // 私有构造函数(单例模式)
    MessageAPI();
    
    // 禁止拷贝和赋值
    MessageAPI(const MessageAPI&) = delete;
    MessageAPI& operator=(const MessageAPI&) = delete;
    
    // 初始化状态
    bool initialized;
};

// 全局函数
bool encodeMessage(int32_t n, int32_t m, const std::string& message, std::string& encodedMessage);
bool decodeMessage(const std::string& encodedMessage, int32_t& n, int32_t& m, std::string& message);

} // namespace api
} // namespace link16
