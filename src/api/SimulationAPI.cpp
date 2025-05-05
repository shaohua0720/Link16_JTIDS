#include "include/link16/api/SimulationAPI.h"
#include "include/link16/simulation/ChannelSimulation.h"
#include "include/link16/simulation/EndToEndSimulation.h"
#include "core/utils/logger.h"
#include <iostream>
#include <fstream>

namespace link16 {
namespace api {

// 单例实例
static SimulationAPI* s_instance = nullptr;

// 获取单例实例
SimulationAPI& SimulationAPI::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new SimulationAPI();
    }
    return *s_instance;
}

// 构造函数
SimulationAPI::SimulationAPI() 
    : initialized(false), running(false), progress(0.0) {
    // 初始化默认参数
    simParams.snr = 10.0;
    simParams.numSamples = 1000;
    simParams.numTrials = 10;
    
    physParams.sampleRate = 1.0e6;
    physParams.symbolRate = 1.0e5;
    physParams.frequency = 969.0e6;
    
    channelType = "AWGN";
}

// 初始化
bool SimulationAPI::initialize() {
    if (initialized) {
        return true;
    }
    
    LOG_INFO("初始化仿真API");
    
    // 这里可以添加初始化代码
    
    initialized = true;
    return true;
}

// 关闭
void SimulationAPI::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("关闭仿真API");
    
    // 如果仿真正在运行，停止它
    if (running) {
        stopSimulation();
    }
    
    // 这里可以添加其他清理代码
    
    initialized = false;
}

// 加载仿真配置
bool SimulationAPI::loadConfig(const std::string& configFile) {
    if (!initialized) {
        LOG_ERROR("仿真API未初始化");
        return false;
    }
    
    LOG_INFO("加载仿真配置: " + configFile);
    
    // 这里应该实现配置文件的加载
    // 例如，可以使用JSON或XML解析器
    
    // 简化实现，假设加载成功
    return true;
}

// 保存仿真配置
bool SimulationAPI::saveConfig(const std::string& configFile) {
    if (!initialized) {
        LOG_ERROR("仿真API未初始化");
        return false;
    }
    
    LOG_INFO("保存仿真配置: " + configFile);
    
    // 这里应该实现配置文件的保存
    // 例如，可以使用JSON或XML生成器
    
    // 简化实现，假设保存成功
    return true;
}

// 设置仿真参数
void SimulationAPI::setSimulationParams(const SimulationParams& params) {
    if (!initialized) {
        LOG_ERROR("仿真API未初始化");
        return;
    }
    
    LOG_INFO("设置仿真参数: SNR=" + std::to_string(params.snr) + 
             ", 样本数=" + std::to_string(params.numSamples) + 
             ", 试验次数=" + std::to_string(params.numTrials));
    
    simParams = params;
}

// 获取仿真参数
SimulationParams SimulationAPI::getSimulationParams() const {
    return simParams;
}

// 设置物理层参数
void SimulationAPI::setPhysicalParams(const PhysicalParams& params) {
    if (!initialized) {
        LOG_ERROR("仿真API未初始化");
        return;
    }
    
    LOG_INFO("设置物理层参数: 采样率=" + std::to_string(params.sampleRate) + 
             ", 符号率=" + std::to_string(params.symbolRate) + 
             ", 频率=" + std::to_string(params.frequency));
    
    physParams = params;
}

// 获取物理层参数
PhysicalParams SimulationAPI::getPhysicalParams() const {
    return physParams;
}

// 设置信道类型
void SimulationAPI::setChannelType(const std::string& type) {
    if (!initialized) {
        LOG_ERROR("仿真API未初始化");
        return;
    }
    
    LOG_INFO("设置信道类型: " + type);
    
    channelType = type;
}

// 获取信道类型
std::string SimulationAPI::getChannelType() const {
    return channelType;
}

// 运行仿真
bool SimulationAPI::runSimulation() {
    if (!initialized) {
        LOG_ERROR("仿真API未初始化");
        return false;
    }
    
    if (running) {
        LOG_WARNING("仿真已经在运行");
        return true;
    }
    
    LOG_INFO("运行仿真: 信道类型=" + channelType);
    
    // 创建信道仿真对象
    simulation::ChannelSimulation channelSim;
    if (!channelSim.initialize(channelType, simParams)) {
        LOG_ERROR("初始化信道仿真失败");
        return false;
    }
    
    // 创建端到端仿真对象
    simulation::EndToEndSimulation e2eSim;
    if (!e2eSim.initialize()) {
        LOG_ERROR("初始化端到端仿真失败");
        return false;
    }
    
    // 设置仿真参数
    e2eSim.setSimulationParams(simParams);
    e2eSim.setPhysicalParams(physParams);
    e2eSim.setChannelType(channelType);
    
    // 运行仿真
    running = true;
    progress = 0.0;
    
    // 这里应该启动一个线程来运行仿真
    // 简化实现，直接运行
    bool result = e2eSim.run();
    
    if (result) {
        // 获取仿真结果
        results = e2eSim.getResults();
        progress = 1.0;
    } else {
        LOG_ERROR("运行仿真失败");
    }
    
    running = false;
    return result;
}

// 停止仿真
void SimulationAPI::stopSimulation() {
    if (!initialized || !running) {
        return;
    }
    
    LOG_INFO("停止仿真");
    
    // 这里应该停止仿真线程
    // 简化实现，直接设置状态
    running = false;
}

// 获取仿真结果
std::vector<double> SimulationAPI::getResults() const {
    return results;
}

// 保存仿真结果
bool SimulationAPI::saveResults(const std::string& resultFile) {
    if (!initialized) {
        LOG_ERROR("仿真API未初始化");
        return false;
    }
    
    LOG_INFO("保存仿真结果: " + resultFile);
    
    // 打开文件
    std::ofstream file(resultFile);
    if (!file.is_open()) {
        LOG_ERROR("无法打开文件: " + resultFile);
        return false;
    }
    
    // 写入结果
    file << "# 仿真结果\n";
    file << "# 信道类型: " << channelType << "\n";
    file << "# SNR: " << simParams.snr << " dB\n";
    file << "# 样本数: " << simParams.numSamples << "\n";
    file << "# 试验次数: " << simParams.numTrials << "\n";
    file << "# 采样率: " << physParams.sampleRate << " Hz\n";
    file << "# 符号率: " << physParams.symbolRate << " Hz\n";
    file << "# 频率: " << physParams.frequency << " Hz\n";
    file << "\n";
    
    for (size_t i = 0; i < results.size(); ++i) {
        file << i << "\t" << results[i] << "\n";
    }
    
    file.close();
    return true;
}

// 检查仿真是否正在运行
bool SimulationAPI::isSimulationRunning() const {
    return running;
}

// 获取仿真进度
double SimulationAPI::getSimulationProgress() const {
    return progress;
}

// 全局函数
bool loadSimulationConfig(const std::string& configFile) {
    return SimulationAPI::getInstance().loadConfig(configFile);
}

bool saveSimulationConfig(const std::string& configFile) {
    return SimulationAPI::getInstance().saveConfig(configFile);
}

void setSimulationParams(const SimulationParams& params) {
    SimulationAPI::getInstance().setSimulationParams(params);
}

SimulationParams getSimulationParams() {
    return SimulationAPI::getInstance().getSimulationParams();
}

void setPhysicalParams(const PhysicalParams& params) {
    SimulationAPI::getInstance().setPhysicalParams(params);
}

PhysicalParams getPhysicalParams() {
    return SimulationAPI::getInstance().getPhysicalParams();
}

void setChannelType(const std::string& type) {
    SimulationAPI::getInstance().setChannelType(type);
}

std::string getChannelType() {
    return SimulationAPI::getInstance().getChannelType();
}

bool runSimulation() {
    return SimulationAPI::getInstance().runSimulation();
}

void stopSimulation() {
    SimulationAPI::getInstance().stopSimulation();
}

std::vector<double> getSimulationResults() {
    return SimulationAPI::getInstance().getResults();
}

bool saveSimulationResults(const std::string& resultFile) {
    return SimulationAPI::getInstance().saveResults(resultFile);
}

bool isSimulationRunning() {
    return SimulationAPI::getInstance().isSimulationRunning();
}

double getSimulationProgress() {
    return SimulationAPI::getInstance().getSimulationProgress();
}

} // namespace api
} // namespace link16
