#include "SimulationApp.h"
#include "core/utils/logger.h"
#include "core/config/SystemConfig.h"

namespace link16 {
namespace application {

// 构造函数
SimulationApp::SimulationApp() : initialized(false), configPath("simulation_config.ini"), resultPath("simulation_results.txt") {
    // 设置默认仿真参数
    simParams.snr = 10.0;       // 10dB
    simParams.ber = 0.0;        // 初始误码率为0
    simParams.duration = 10.0;  // 10秒
    simParams.iterations = 100; // 100次迭代
    
    // 设置默认物理层参数
    physParams.frequency = 969.0e6;  // 969MHz
    physParams.bandwidth = 3.0e6;    // 3MHz
    physParams.power = 20.0;         // 20dBm
    physParams.hoppingPattern = 1;   // 跳频模式1
}

// 析构函数
SimulationApp::~SimulationApp() {
    if (initialized) {
        shutdown();
    }
}

// 初始化仿真
bool SimulationApp::initialize(const std::string& configFile) {
    LOG_INFO("正在初始化Link16仿真应用...");
    
    configPath = configFile;
    
    // 加载配置
    config::SystemConfig& config = config::SystemConfig::getInstance();
    if (!config.loadConfigFile(configPath)) {
        LOG_WARNING("无法加载仿真配置文件，使用默认配置");
    } else {
        // 从配置文件加载仿真参数
        std::string snrStr = config.getConfigValue("simulation_snr");
        if (!snrStr.empty()) {
            simParams.snr = std::stod(snrStr);
        }
        
        std::string durationStr = config.getConfigValue("simulation_duration");
        if (!durationStr.empty()) {
            simParams.duration = std::stod(durationStr);
        }
        
        std::string iterationsStr = config.getConfigValue("simulation_iterations");
        if (!iterationsStr.empty()) {
            simParams.iterations = std::stoi(iterationsStr);
        }
        
        // 从配置文件加载物理层参数
        std::string frequencyStr = config.getConfigValue("physical_frequency");
        if (!frequencyStr.empty()) {
            physParams.frequency = std::stod(frequencyStr);
        }
        
        std::string bandwidthStr = config.getConfigValue("physical_bandwidth");
        if (!bandwidthStr.empty()) {
            physParams.bandwidth = std::stod(bandwidthStr);
        }
        
        std::string powerStr = config.getConfigValue("physical_power");
        if (!powerStr.empty()) {
            physParams.power = std::stod(powerStr);
        }
        
        std::string hoppingPatternStr = config.getConfigValue("physical_hopping_pattern");
        if (!hoppingPatternStr.empty()) {
            physParams.hoppingPattern = std::stoi(hoppingPatternStr);
        }
        
        // 获取结果文件路径
        std::string resultPathStr = config.getConfigValue("result_file_path");
        if (!resultPathStr.empty()) {
            resultPath = resultPathStr;
        }
    }
    
    initialized = true;
    LOG_INFO("Link16仿真应用初始化完成");
    return true;
}

// 运行仿真
void SimulationApp::run() {
    if (!initialized) {
        LOG_ERROR("仿真应用未初始化，无法运行");
        return;
    }
    
    LOG_INFO("Link16仿真开始运行");
    LOG_INFO("仿真参数: SNR=" + std::to_string(simParams.snr) + "dB, 持续时间=" + 
             std::to_string(simParams.duration) + "s, 迭代次数=" + std::to_string(simParams.iterations));
    LOG_INFO("物理层参数: 频率=" + std::to_string(physParams.frequency/1e6) + "MHz, 带宽=" + 
             std::to_string(physParams.bandwidth/1e6) + "MHz, 功率=" + std::to_string(physParams.power) + "dBm");
    
    // 仿真主循环
    // 这里将添加实际的仿真逻辑
    
    LOG_INFO("Link16仿真运行结束");
    LOG_INFO("仿真结果已保存到: " + resultPath);
}

// 设置仿真参数
void SimulationApp::setSimulationParams(const SimulationParams& params) {
    simParams = params;
    LOG_INFO("仿真参数已更新");
}

// 获取仿真参数
SimulationParams SimulationApp::getSimulationParams() const {
    return simParams;
}

// 设置物理层参数
void SimulationApp::setPhysicalParams(const PhysicalParams& params) {
    physParams = params;
    LOG_INFO("物理层参数已更新");
}

// 获取物理层参数
PhysicalParams SimulationApp::getPhysicalParams() const {
    return physParams;
}

// 关闭仿真
void SimulationApp::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("正在关闭Link16仿真应用...");
    
    // 执行清理操作
    
    initialized = false;
    LOG_INFO("Link16仿真应用已关闭");
}

} // namespace application
} // namespace link16
