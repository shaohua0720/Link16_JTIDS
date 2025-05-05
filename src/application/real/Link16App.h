#pragma once
#include <string>
#include "protocol/message/STDPMsg.h"

namespace link16 {
namespace application {

class Link16App {
public:
    // 构造函数
    Link16App();
    
    // 析构函数
    ~Link16App();
    
    // 初始化应用
    bool initialize();
    
    // 运行应用
    void run();
    
    // 发送消息
    bool sendMessage(int n, int m, const std::string& message);
    
    // 接收消息
    bool receiveMessage(std::string& message, int& n, int& m);
    
    // 关闭应用
    void shutdown();

private:
    // 应用状态
    bool initialized;
    
    // 配置文件路径
    std::string configPath;
    
    // 数据文件路径
    std::string dataPath;
};

} // namespace application
} // namespace link16
