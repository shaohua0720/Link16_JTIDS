#include <iostream>
#include "core/utils/logger.h"
#include "core/config/SystemConfig.h"

int main(int argc, char* argv[]) {
    // 初始化日志系统
    link16::utils::Logger::getInstance().initialize("link16.log");
    LOG_INFO("Link16应用程序启动");
    
    // 加载系统配置
    link16::config::SystemConfig& config = link16::config::SystemConfig::getInstance();
    config.loadConfigFile("config.ini");
    
    LOG_INFO("系统配置加载完成");
    
    // 应用程序主逻辑
    std::cout << "Link16通信系统" << std::endl;
    std::cout << "版本: 1.0.0" << std::endl;
    std::cout << "==================" << std::endl;
    
    // 这里将添加实际的应用逻辑
    
    LOG_INFO("Link16应用程序正常退出");
    link16::utils::Logger::getInstance().shutdown();
    
    return 0;
}
