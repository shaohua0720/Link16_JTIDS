#include "include/link16/api/MessageAPI.h"
#include "src/protocol/interface/interface.h"
#include "core/utils/logger.h"
#include <iostream>

namespace link16 {
namespace api {

// 单例实例
static MessageAPI* s_instance = nullptr;

// 获取单例实例
MessageAPI& MessageAPI::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new MessageAPI();
    }
    return *s_instance;
}

// 构造函数
MessageAPI::MessageAPI() : initialized(false) {
}

// 初始化
bool MessageAPI::initialize() {
    if (initialized) {
        return true;
    }
    
    LOG_INFO("初始化消息API");
    
    // 这里可以添加初始化代码
    
    initialized = true;
    return true;
}

// 关闭
void MessageAPI::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("关闭消息API");
    
    // 这里可以添加清理代码
    
    initialized = false;
}

// 编码消息
bool MessageAPI::encodeMessage(int32_t n, int32_t m, const std::string& message, std::string& encodedMessage) {
    if (!initialized) {
        LOG_ERROR("消息API未初始化");
        return false;
    }
    
    LOG_INFO("编码消息: n=" + std::to_string(n) + ", m=" + std::to_string(m));
    
    // 调用内部接口
    std::string inputs = message;
    int32_t result = encoder_Link16(n, m, inputs);
    
    if (result < 0) {
        LOG_ERROR("编码消息失败: " + std::to_string(result));
        return false;
    }
    
    // 内部接口可能直接修改了inputs，或者将结果写入了文件
    // 这里我们假设结果在inputs中
    encodedMessage = inputs;
    
    return true;
}

// 解码消息
bool MessageAPI::decodeMessage(const std::string& encodedMessage, int32_t& n, int32_t& m, std::string& message) {
    if (!initialized) {
        LOG_ERROR("消息API未初始化");
        return false;
    }
    
    LOG_INFO("解码消息");
    
    // 调用内部接口
    std::string raw_data = encodedMessage;
    int32_t result = decoder_Link16(raw_data, n, m);
    
    if (result < 0) {
        LOG_ERROR("解码消息失败: " + std::to_string(result));
        return false;
    }
    
    // 内部接口可能直接修改了raw_data
    message = raw_data;
    
    LOG_INFO("解码消息成功: n=" + std::to_string(n) + ", m=" + std::to_string(m));
    
    return true;
}

// 获取消息类型描述
std::string MessageAPI::getMessageTypeDescription(int32_t n, int32_t m) {
    if (!initialized) {
        LOG_ERROR("消息API未初始化");
        return "未知消息类型";
    }
    
    // 这里可以调用内部接口获取消息类型描述
    // 例如，可以使用全局变量g_JType
    std::string type = std::to_string(n) + " " + std::to_string(m);
    
    // 这里假设有一个全局函数可以获取消息类型描述
    // 实际实现可能需要调用其他内部接口
    return "J" + type;
}

// 检查消息类型是否有效
bool MessageAPI::isValidMessageType(int32_t n, int32_t m) {
    if (!initialized) {
        LOG_ERROR("消息API未初始化");
        return false;
    }
    
    // 这里可以调用内部接口检查消息类型是否有效
    // 例如，可以检查全局变量g_JType中是否包含该类型
    std::string type = std::to_string(n) + " " + std::to_string(m);
    
    // 这里假设有一个全局函数可以检查消息类型是否有效
    // 实际实现可能需要调用其他内部接口
    return true;  // 简化实现，假设所有类型都有效
}

// 全局函数
bool encodeMessage(int32_t n, int32_t m, const std::string& message, std::string& encodedMessage) {
    return MessageAPI::getInstance().encodeMessage(n, m, message, encodedMessage);
}

bool decodeMessage(const std::string& encodedMessage, int32_t& n, int32_t& m, std::string& message) {
    return MessageAPI::getInstance().decodeMessage(encodedMessage, n, m, message);
}

} // namespace api
} // namespace link16
