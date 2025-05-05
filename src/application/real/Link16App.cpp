#include "Link16App.h"
#include "core/utils/logger.h"
#include "core/config/SystemConfig.h"
#include "protocol/interface/interface.h"

namespace link16 {
namespace application {

// 构造函数
Link16App::Link16App() : initialized(false), configPath("config.ini"), dataPath("data.txt") {
}

// 析构函数
Link16App::~Link16App() {
    if (initialized) {
        shutdown();
    }
}

// 初始化应用
bool Link16App::initialize() {
    LOG_INFO("正在初始化Link16应用...");
    
    // 加载配置
    config::SystemConfig& config = config::SystemConfig::getInstance();
    if (!config.loadConfigFile(configPath)) {
        LOG_WARNING("无法加载配置文件，使用默认配置");
    }
    
    // 获取数据文件路径
    dataPath = config.getConfigValue("data_file_path");
    if (dataPath.empty()) {
        dataPath = "data.txt";
    }
    
    initialized = true;
    LOG_INFO("Link16应用初始化完成");
    return true;
}

// 运行应用
void Link16App::run() {
    if (!initialized) {
        LOG_ERROR("应用未初始化，无法运行");
        return;
    }
    
    LOG_INFO("Link16应用开始运行");
    
    // 应用主循环
    // 这里将添加实际的应用逻辑
    
    LOG_INFO("Link16应用运行结束");
}

// 发送消息
bool Link16App::sendMessage(int n, int m, const std::string& message) {
    if (!initialized) {
        LOG_ERROR("应用未初始化，无法发送消息");
        return false;
    }
    
    LOG_INFO("发送消息: J" + std::to_string(n) + "." + std::to_string(m) + " - " + message);
    
    // 调用协议接口发送消息
    int32_t result = encoder_Link16(n, m, const_cast<std::string&>(message));
    
    if (result != 0) {
        LOG_ERROR("消息发送失败，错误码: " + std::to_string(result));
        return false;
    }
    
    LOG_INFO("消息发送成功");
    return true;
}

// 接收消息
bool Link16App::receiveMessage(std::string& message, int& n, int& m) {
    if (!initialized) {
        LOG_ERROR("应用未初始化，无法接收消息");
        return false;
    }
    
    LOG_INFO("正在接收消息...");
    
    // 调用协议接口接收消息
    int32_t result = decoder_Link16(message, n, m);
    
    if (result != 0) {
        LOG_ERROR("消息接收失败，错误码: " + std::to_string(result));
        return false;
    }
    
    LOG_INFO("接收到消息: J" + std::to_string(n) + "." + std::to_string(m) + " - " + message);
    return true;
}

// 关闭应用
void Link16App::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("正在关闭Link16应用...");
    
    // 执行清理操作
    
    initialized = false;
    LOG_INFO("Link16应用已关闭");
}

} // namespace application
} // namespace link16
