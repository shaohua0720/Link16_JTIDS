#pragma once
#include "include/link16/Link16.h"
#include "core/types/Link16Types.h"
#include <string>
#include <memory>

namespace link16 {

// 前向声明
namespace protocol {
    class MessageProcessor;
}

namespace coding {
    class CodingProcessor;
}

namespace physical {
    class PhysicalProcessor;
}

namespace application {

// 前向声明
class Link16App;
class SimulationApp;

/**
 * @brief Link16系统类，整合各层功能
 */
class Link16System {
public:
    // 系统运行模式
    enum Mode {
        REAL,       // 实际通信模式
        SIMULATION  // 仿真模式
    };
    
    // 单例模式
    static Link16System& getInstance();
    
    // 初始化系统
    bool initialize(const std::string& configFile = "system_config.ini");
    
    // 关闭系统
    void shutdown();
    
    // 发送Link16消息
    bool sendMessage(int32_t n, int32_t m, const std::string& message);
    
    // 接收Link16消息
    bool receiveMessage(int32_t& n, int32_t& m, std::string& message, int timeoutMs = 1000);
    
    // 运行仿真模式
    bool runSimulation(const SimulationParams& params);
    
    // 获取当前模式
    Mode getMode() const;
    
    // 设置模式
    bool setMode(Mode mode);
    
    // 获取协议处理器
    protocol::MessageProcessor& getMessageProcessor();
    
    // 获取编码处理器
    coding::CodingProcessor& getCodingProcessor();
    
    // 获取物理处理器
    physical::PhysicalProcessor& getPhysicalProcessor();

private:
    // 私有构造函数(单例模式)
    Link16System();
    
    // 析构函数
    ~Link16System();
    
    // 禁止拷贝和赋值
    Link16System(const Link16System&) = delete;
    Link16System& operator=(const Link16System&) = delete;
    
    // 系统状态
    bool initialized;
    
    // 当前模式
    Mode currentMode;
    
    // 配置文件路径
    std::string configPath;
    
    // 协议处理器
    std::unique_ptr<protocol::MessageProcessor> messageProcessor;
    
    // 编码处理器
    std::unique_ptr<coding::CodingProcessor> codingProcessor;
    
    // 物理处理器
    std::unique_ptr<physical::PhysicalProcessor> physicalProcessor;
    
    // 实际通信应用
    std::unique_ptr<Link16App> realApp;
    
    // 仿真应用
    std::unique_ptr<SimulationApp> simApp;
};

} // namespace application
} // namespace link16

