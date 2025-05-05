#pragma once
#include <string>
#include <vector>
#include "core/types/Link16Types.h"

namespace link16 {
namespace simulation {
namespace engine {

// 仿真配置类
class SimulationConfig {
public:
    // 构造函数
    SimulationConfig();
    
    // 从文件加载配置
    bool loadFromFile(const std::string& filePath);
    
    // 保存配置到文件
    bool saveToFile(const std::string& filePath) const;
    
    // 设置仿真参数
    void setSimulationParams(const SimulationParams& params);
    
    // 获取仿真参数
    SimulationParams getSimulationParams() const;
    
    // 设置物理层参数
    void setPhysicalParams(const PhysicalParams& params);
    
    // 获取物理层参数
    PhysicalParams getPhysicalParams() const;
    
    // 设置信道类型
    void setChannelType(const std::string& type);
    
    // 获取信道类型
    std::string getChannelType() const;
    
    // 设置结果文件路径
    void setResultFilePath(const std::string& path);
    
    // 获取结果文件路径
    std::string getResultFilePath() const;
    
    // 设置是否启用详细日志
    void setVerboseLogging(bool verbose);
    
    // 获取是否启用详细日志
    bool getVerboseLogging() const;

private:
    // 仿真参数
    SimulationParams simParams;
    
    // 物理层参数
    PhysicalParams physParams;
    
    // 信道类型
    std::string channelType;
    
    // 结果文件路径
    std::string resultFilePath;
    
    // 是否启用详细日志
    bool verboseLogging;
};

} // namespace engine
} // namespace simulation
} // namespace link16
