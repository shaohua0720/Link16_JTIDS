#include <iostream>
#include "core/utils/logger.h"
#include "Link16System.h"

int main(int argc, char* argv[]) {
    // 初始化日志系统
    link16::utils::Logger::getInstance().initialize("link16.log");
    LOG_INFO("Link16应用程序启动");

    // 获取Link16系统实例
    link16::application::Link16System& system = link16::application::Link16System::getInstance();
    
    // 初始化系统
    if (!system.initialize("config.ini")) {
        LOG_ERROR("系统初始化失败");
        return 1;
    }
    
    // 应用程序主逻辑
    std::cout << "Link16通信系统" << std::endl;
    std::cout << "版本: " << link16::getVersionString() << std::endl;
    std::cout << "==================" << std::endl;
    
    // 示例：发送消息
    if (system.sendMessage(7, 3, "Hello, Link16!")) {
        std::cout << "消息发送成功" << std::endl;
    }
    
    // 示例：接收消息
    int32_t n, m;
    std::string message;
    if (system.receiveMessage(n, m, message, 2000)) {
        std::cout << "接收到消息: N=" << n << ", M=" << m << std::endl;
        std::cout << "内容: " << message << std::endl;
    }
    
    // 示例：运行仿真
    link16::SimulationParams simParams;
    simParams.snr = 10.0;
    simParams.iterations = 1000;
    
    if (system.runSimulation(simParams)) {
        std::cout << "仿真完成" << std::endl;
    }
    
    // 示例：获取处理器
    try {
        auto& messageProcessor = system.getMessageProcessor();
        auto& codingProcessor = system.getCodingProcessor();
        auto& physicalProcessor = system.getPhysicalProcessor();
        
        std::cout << "成功获取所有处理器" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "获取处理器失败: " << e.what() << std::endl;
    }
    
    // 关闭系统
    system.shutdown();
    
    LOG_INFO("Link16应用程序正常退出");
    return 0;
}
