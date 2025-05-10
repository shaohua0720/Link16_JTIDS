#include "Link16System.h"
#include "real/Link16App.h"
#include "simulation/SimulationApp.h"
#include "core/utils/logger.h"
#include "core/config/SystemConfig.h"
#include "protocol/MessageProcessor.h"
#include "coding/CodingProcessor.h"
#include "physical/PhysicalProcessor.h"

namespace link16 {
namespace application {

// 单例实例
static Link16System* s_instance = nullptr;

// 获取单例实例
Link16System& Link16System::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new Link16System();
    }
    return *s_instance;
}

// 构造函数
Link16System::Link16System() 
    : initialized(false), currentMode(REAL), 
      realApp(nullptr), simApp(nullptr),
      messageProcessor(nullptr), codingProcessor(nullptr), physicalProcessor(nullptr) {
}

// 析构函数
Link16System::~Link16System() {
    if (initialized) {
        shutdown();
    }
}

// 初始化系统
bool Link16System::initialize(const std::string& configFile) {
    LOG_INFO("初始化Link16系统");
    
    // 保存配置路径
    configPath = configFile;
    
    // 加载系统配置
    config::SystemConfig& config = config::SystemConfig::getInstance();
    if (!config.loadConfigFile(configFile)) {
        LOG_ERROR("加载配置文件失败: " + configFile);
        return false;
    }
    
    // 初始化处理器
    messageProcessor = std::make_unique<protocol::MessageProcessor>();
    codingProcessor = std::make_unique<coding::CodingProcessor>();
    physicalProcessor = std::make_unique<physical::PhysicalProcessor>();
    
    if (!messageProcessor->initialize() || 
        !codingProcessor->initialize() || 
        !physicalProcessor->initialize()) {
        LOG_ERROR("初始化处理器失败");
        return false;
    }
    
    // 确定系统模式
    std::string modeStr = config.getConfigValue("system_mode");
    if (modeStr == "simulation") {
        currentMode = SIMULATION;
        LOG_INFO("系统运行在仿真模式");
        
        // 创建并初始化仿真应用
        simApp = std::make_unique<SimulationApp>();
        if (!simApp->initialize(configFile)) {
            LOG_ERROR("初始化仿真应用失败");
            return false;
        }
    } else {
        currentMode = REAL;
        LOG_INFO("系统运行在实际通信模式");
        
        // 创建并初始化实际通信应用
        realApp = std::make_unique<Link16App>();
        if (!realApp->initialize()) {
            LOG_ERROR("初始化实际通信应用失败");
            return false;
        }
    }
    
    initialized = true;
    LOG_INFO("Link16系统初始化完成");
    return true;
}

// 关闭系统
void Link16System::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("关闭Link16系统");
    
    // 根据当前模式关闭相应的应用
    if (currentMode == SIMULATION && simApp) {
        simApp->shutdown();
        simApp.reset();
    } else if (currentMode == REAL && realApp) {
        realApp->shutdown();
        realApp.reset();
    }
    
    // 关闭处理器
    if (physicalProcessor) {
        physicalProcessor->shutdown();
        physicalProcessor.reset();
    }
    
    if (codingProcessor) {
        codingProcessor->shutdown();
        codingProcessor.reset();
    }
    
    if (messageProcessor) {
        messageProcessor->shutdown();
        messageProcessor.reset();
    }
    
    initialized = false;
    LOG_INFO("Link16系统已关闭");
}

// 发送Link16消息
bool Link16System::sendMessage(int32_t n, int32_t m, const std::string& message) {
    if (!initialized) {
        LOG_ERROR("系统未初始化");
        return false;
    }
    
    // 根据当前模式调用相应的应用
    if (currentMode == REAL && realApp) {
        return realApp->sendMessage(n, m, message);
    } else if (currentMode == SIMULATION && simApp) {
        LOG_WARNING("仿真模式下不支持实际消息发送");
        return false;
    } else {
        LOG_ERROR("无效的系统模式或应用未初始化");
        return false;
    }
}

// 接收Link16消息
bool Link16System::receiveMessage(int32_t& n, int32_t& m, std::string& message, int timeoutMs) {
    if (!initialized) {
        LOG_ERROR("系统未初始化");
        return false;
    }
    
    // 根据当前模式调用相应的应用
    if (currentMode == REAL && realApp) {
        return realApp->receiveMessage(message, n, m);
    } else if (currentMode == SIMULATION && simApp) {
        LOG_WARNING("仿真模式下不支持实际消息接收");
        return false;
    } else {
        LOG_ERROR("无效的系统模式或应用未初始化");
        return false;
    }
}

// 运行仿真模式
bool Link16System::runSimulation(const SimulationParams& params) {
    if (!initialized) {
        LOG_ERROR("系统未初始化");
        return false;
    }
    
    // 如果当前不是仿真模式，切换到仿真模式
    if (currentMode != SIMULATION) {
        LOG_INFO("切换到仿真模式");
        
        // 关闭实际通信应用
        if (realApp) {
            realApp->shutdown();
            realApp.reset();
        }
        
        // 创建并初始化仿真应用
        simApp = std::make_unique<SimulationApp>();
        if (!simApp->initialize(configPath)) {
            LOG_ERROR("初始化仿真应用失败");
            return false;
        }
        
        currentMode = SIMULATION;
    }
    
    // 设置仿真参数并运行
    if (simApp) {
        simApp->setSimulationParams(params);
        simApp->run();
        return true;
    } else {
        LOG_ERROR("仿真应用未初始化");
        return false;
    }
}

// 获取当前模式
Link16System::Mode Link16System::getMode() const {
    return currentMode;
}

// 设置模式
bool Link16System::setMode(Mode mode) {
    if (mode == currentMode) {
        return true; // 已经是请求的模式
    }
    
    if (!initialized) {
        LOG_ERROR("系统未初始化，无法切换模式");
        return false;
    }
    
    LOG_INFO("切换系统模式: " + std::string(mode == REAL ? "实际通信模式" : "仿真模式"));
    
    if (mode == REAL) {
        // 关闭仿真应用
        if (simApp) {
            simApp->shutdown();
            simApp.reset();
        }
        
        // 创建并初始化实际通信应用
        realApp = std::make_unique<Link16App>();
        if (!realApp->initialize()) {
            LOG_ERROR("初始化实际通信应用失败");
            return false;
        }
    } else { // SIMULATION
        // 关闭实际通信应用
        if (realApp) {
            realApp->shutdown();
            realApp.reset();
        }
        
        // 创建并初始化仿真应用
        simApp = std::make_unique<SimulationApp>();
        if (!simApp->initialize(configPath)) {
            LOG_ERROR("初始化仿真应用失败");
            return false;
        }
    }
    
    currentMode = mode;
    LOG_INFO("系统模式切换完成");
    return true;
}

// 获取协议处理器
protocol::MessageProcessor& Link16System::getMessageProcessor() {
    if (!messageProcessor) {
        throw std::runtime_error("协议处理器未初始化");
    }
    return *messageProcessor;
}

// 获取编码处理器
coding::CodingProcessor& Link16System::getCodingProcessor() {
    if (!codingProcessor) {
        throw std::runtime_error("编码处理器未初始化");
    }
    return *codingProcessor;
}

// 获取物理处理器
physical::PhysicalProcessor& Link16System::getPhysicalProcessor() {
    if (!physicalProcessor) {
        throw std::runtime_error("物理处理器未初始化");
    }
    return *physicalProcessor;
}

} // namespace application
} // namespace link16

