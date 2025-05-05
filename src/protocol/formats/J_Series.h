#pragma once
#include <string>
#include <map>
#include <vector>

namespace link16 {
namespace protocol {
namespace formats {

// J系列消息类
class J_Series {
public:
    // 构造函数
    J_Series();
    
    // 析构函数
    ~J_Series();
    
    // 获取消息类型描述
    std::string getMessageTypeDescription(int n, int m) const;
    
    // 检查消息类型是否有效
    bool isValidMessageType(int n, int m) const;
    
    // 获取所有支持的消息类型
    std::vector<std::pair<int, int>> getAllSupportedMessageTypes() const;
    
    // 获取特定大类的所有消息类型
    std::vector<std::pair<int, int>> getMessageTypesByCategory(int n) const;

private:
    // 消息类型映射表
    std::map<std::string, std::string> messageTypeMap;
    
    // 初始化消息类型映射表
    void initializeMessageTypeMap();
};

} // namespace formats
} // namespace protocol
} // namespace link16
