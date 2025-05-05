#include "J_Series.h"
#include "core/config/global.h"

namespace link16 {
namespace protocol {
namespace formats {

// 构造函数
J_Series::J_Series() {
    initializeMessageTypeMap();
}

// 析构函数
J_Series::~J_Series() {
}

// 获取消息类型描述
std::string J_Series::getMessageTypeDescription(int n, int m) const {
    std::string key = std::to_string(n) + " " + std::to_string(m);
    auto it = messageTypeMap.find(key);
    if (it != messageTypeMap.end()) {
        return it->second;
    }
    return "未知消息类型";
}

// 检查消息类型是否有效
bool J_Series::isValidMessageType(int n, int m) const {
    std::string key = std::to_string(n) + " " + std::to_string(m);
    return messageTypeMap.find(key) != messageTypeMap.end();
}

// 获取所有支持的消息类型
std::vector<std::pair<int, int>> J_Series::getAllSupportedMessageTypes() const {
    std::vector<std::pair<int, int>> result;
    for (const auto& pair : messageTypeMap) {
        std::string key = pair.first;
        size_t spacePos = key.find(' ');
        if (spacePos != std::string::npos) {
            int n = std::stoi(key.substr(0, spacePos));
            int m = std::stoi(key.substr(spacePos + 1));
            result.push_back(std::make_pair(n, m));
        }
    }
    return result;
}

// 获取特定大类的所有消息类型
std::vector<std::pair<int, int>> J_Series::getMessageTypesByCategory(int n) const {
    std::vector<std::pair<int, int>> result;
    std::string prefix = std::to_string(n) + " ";
    for (const auto& pair : messageTypeMap) {
        std::string key = pair.first;
        if (key.substr(0, prefix.length()) == prefix) {
            int m = std::stoi(key.substr(prefix.length()));
            result.push_back(std::make_pair(n, m));
        }
    }
    return result;
}

// 初始化消息类型映射表
void J_Series::initializeMessageTypeMap() {
    // 使用全局变量g_JType初始化
    messageTypeMap = g_JType;
}

} // namespace formats
} // namespace protocol
} // namespace link16
