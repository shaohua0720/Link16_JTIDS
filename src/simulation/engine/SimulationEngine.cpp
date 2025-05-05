#include "SimulationEngine.h"
#include "core/utils/logger.h"
#include "core/utils/fileUtils.h"
#include "../channel/awgn/AWGNChannel.h"
#include <fstream>
#include <random>
#include <chrono>

namespace link16 {
namespace simulation {
namespace engine {

// 构造函数
SimulationEngine::SimulationEngine() : initialized(false), completed(false) {
}

// 析构函数
SimulationEngine::~SimulationEngine() {
}

// 初始化仿真引擎
bool SimulationEngine::initialize(const SimulationConfig& config) {
    LOG_INFO("正在初始化仿真引擎...");
    
    this->config = config;
    
    // 创建信道模型
    std::string channelType = config.getChannelType();
    SimulationParams simParams = config.getSimulationParams();
    
    if (channelType == "AWGN") {
        channelModel = std::make_shared<channel::AWGNChannel>(simParams.snr);
        LOG_INFO("已创建AWGN信道模型，SNR = " + std::to_string(simParams.snr) + " dB");
    } else {
        LOG_ERROR("不支持的信道类型: " + channelType);
        return false;
    }
    
    // 清空结果
    results.clear();
    
    initialized = true;
    completed = false;
    
    LOG_INFO("仿真引擎初始化完成");
    return true;
}

// 运行仿真
bool SimulationEngine::run() {
    if (!initialized) {
        LOG_ERROR("仿真引擎未初始化，无法运行");
        return false;
    }
    
    LOG_INFO("开始运行仿真...");
    
    SimulationParams simParams = config.getSimulationParams();
    PhysicalParams physParams = config.getPhysicalParams();
    
    // 记录开始时间
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 仿真主循环
    for (int i = 0; i < simParams.iterations; ++i) {
        if (config.getVerboseLogging()) {
            LOG_INFO("正在执行仿真迭代 " + std::to_string(i + 1) + "/" + std::to_string(simParams.iterations));
        }
        
        // 这里将添加实际的仿真逻辑
        // 1. 生成随机数据
        // 2. 通过信道模型处理数据
        // 3. 计算误码率
        // 4. 记录结果
        
        // 模拟仿真结果
        double ber = 0.5 * std::exp(-simParams.snr / 10.0);
        results.push_back(ber);
    }
    
    // 记录结束时间
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    LOG_INFO("仿真完成，耗时: " + std::to_string(duration) + " ms");
    
    completed = true;
    return true;
}

// 设置信道模型
void SimulationEngine::setChannelModel(std::shared_ptr<channel::ChannelModel> model) {
    channelModel = model;
}

// 获取信道模型
std::shared_ptr<channel::ChannelModel> SimulationEngine::getChannelModel() const {
    return channelModel;
}

// 设置仿真配置
void SimulationEngine::setConfig(const SimulationConfig& config) {
    this->config = config;
}

// 获取仿真配置
SimulationConfig SimulationEngine::getConfig() const {
    return config;
}

// 获取仿真结果
std::vector<double> SimulationEngine::getResults() const {
    return results;
}

// 保存仿真结果
bool SimulationEngine::saveResults(const std::string& filePath) const {
    if (!completed) {
        LOG_WARNING("仿真尚未完成，结果可能不完整");
    }
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        LOG_ERROR("无法打开文件进行写入: " + filePath);
        return false;
    }
    
    // 写入仿真参数
    SimulationParams simParams = config.getSimulationParams();
    PhysicalParams physParams = config.getPhysicalParams();
    
    file << "# Link16仿真结果\n";
    file << "# 生成时间: " << __DATE__ << " " << __TIME__ << "\n\n";
    
    file << "# 仿真参数\n";
    file << "SNR = " << simParams.snr << " dB\n";
    file << "持续时间 = " << simParams.duration << " s\n";
    file << "迭代次数 = " << simParams.iterations << "\n\n";
    
    file << "# 物理层参数\n";
    file << "频率 = " << physParams.frequency / 1e6 << " MHz\n";
    file << "带宽 = " << physParams.bandwidth / 1e6 << " MHz\n";
    file << "功率 = " << physParams.power << " dBm\n";
    file << "跳频模式 = " << physParams.hoppingPattern << "\n\n";
    
    file << "# 信道类型\n";
    file << channelModel->getName() << ": " << channelModel->getDescription() << "\n\n";
    
    file << "# 仿真结果\n";
    file << "迭代,误码率\n";
    
    for (size_t i = 0; i < results.size(); ++i) {
        file << i + 1 << "," << results[i] << "\n";
    }
    
    file.close();
    LOG_INFO("仿真结果已保存到: " + filePath);
    
    return true;
}

} // namespace engine
} // namespace simulation
} // namespace link16
