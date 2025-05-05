#include "SimulationConfig.h"
#include "core/utils/logger.h"
#include "core/utils/fileUtils.h"
#include <fstream>
#include <sstream>

namespace link16 {
namespace simulation {
namespace engine {

// 构造函数
SimulationConfig::SimulationConfig() : channelType("AWGN"), resultFilePath("simulation_results.txt"), verboseLogging(false) {
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

// 从文件加载配置
bool SimulationConfig::loadFromFile(const std::string& filePath) {
    if (!utils::FileUtils::fileExists(filePath)) {
        LOG_ERROR("配置文件不存在: " + filePath);
        return false;
    }
    
    std::string content = utils::FileUtils::readTextFile(filePath);
    if (content.empty()) {
        LOG_ERROR("配置文件为空或读取失败: " + filePath);
        return false;
    }
    
    std::istringstream iss(content);
    std::string line;
    
    while (std::getline(iss, line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // 解析键值对
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // 去除前后空格
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // 设置相应的配置项
            if (key == "simulation_snr") {
                simParams.snr = std::stod(value);
            } else if (key == "simulation_duration") {
                simParams.duration = std::stod(value);
            } else if (key == "simulation_iterations") {
                simParams.iterations = std::stoi(value);
            } else if (key == "physical_frequency") {
                physParams.frequency = std::stod(value);
            } else if (key == "physical_bandwidth") {
                physParams.bandwidth = std::stod(value);
            } else if (key == "physical_power") {
                physParams.power = std::stod(value);
            } else if (key == "physical_hopping_pattern") {
                physParams.hoppingPattern = std::stoi(value);
            } else if (key == "channel_type") {
                channelType = value;
            } else if (key == "result_file_path") {
                resultFilePath = value;
            } else if (key == "verbose_logging") {
                verboseLogging = (value == "true" || value == "1" || value == "yes");
            }
        }
    }
    
    LOG_INFO("成功从文件加载配置: " + filePath);
    return true;
}

// 保存配置到文件
bool SimulationConfig::saveToFile(const std::string& filePath) const {
    std::ostringstream oss;
    
    oss << "# Link16仿真配置文件\n";
    oss << "# 生成时间: " << __DATE__ << " " << __TIME__ << "\n\n";
    
    oss << "# 仿真参数\n";
    oss << "simulation_snr=" << simParams.snr << "\n";
    oss << "simulation_duration=" << simParams.duration << "\n";
    oss << "simulation_iterations=" << simParams.iterations << "\n\n";
    
    oss << "# 物理层参数\n";
    oss << "physical_frequency=" << physParams.frequency << "\n";
    oss << "physical_bandwidth=" << physParams.bandwidth << "\n";
    oss << "physical_power=" << physParams.power << "\n";
    oss << "physical_hopping_pattern=" << physParams.hoppingPattern << "\n\n";
    
    oss << "# 信道参数\n";
    oss << "channel_type=" << channelType << "\n\n";
    
    oss << "# 输出设置\n";
    oss << "result_file_path=" << resultFilePath << "\n";
    oss << "verbose_logging=" << (verboseLogging ? "true" : "false") << "\n";
    
    bool success = utils::FileUtils::writeTextFile(filePath, oss.str());
    if (success) {
        LOG_INFO("成功保存配置到文件: " + filePath);
    } else {
        LOG_ERROR("保存配置到文件失败: " + filePath);
    }
    
    return success;
}

// 设置仿真参数
void SimulationConfig::setSimulationParams(const SimulationParams& params) {
    simParams = params;
}

// 获取仿真参数
SimulationParams SimulationConfig::getSimulationParams() const {
    return simParams;
}

// 设置物理层参数
void SimulationConfig::setPhysicalParams(const PhysicalParams& params) {
    physParams = params;
}

// 获取物理层参数
PhysicalParams SimulationConfig::getPhysicalParams() const {
    return physParams;
}

// 设置信道类型
void SimulationConfig::setChannelType(const std::string& type) {
    channelType = type;
}

// 获取信道类型
std::string SimulationConfig::getChannelType() const {
    return channelType;
}

// 设置结果文件路径
void SimulationConfig::setResultFilePath(const std::string& path) {
    resultFilePath = path;
}

// 获取结果文件路径
std::string SimulationConfig::getResultFilePath() const {
    return resultFilePath;
}

// 设置是否启用详细日志
void SimulationConfig::setVerboseLogging(bool verbose) {
    verboseLogging = verbose;
}

// 获取是否启用详细日志
bool SimulationConfig::getVerboseLogging() const {
    return verboseLogging;
}

} // namespace engine
} // namespace simulation
} // namespace link16
