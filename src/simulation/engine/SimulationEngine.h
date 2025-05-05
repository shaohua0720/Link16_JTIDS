#pragma once
#include <string>
#include <memory>
#include <vector>
#include "SimulationConfig.h"
#include "../channel/base/ChannelModel.h"

namespace link16 {
namespace simulation {
namespace engine {

// 仿真引擎类
class SimulationEngine {
public:
    // 构造函数
    SimulationEngine();
    
    // 析构函数
    ~SimulationEngine();
    
    // 初始化仿真引擎
    bool initialize(const SimulationConfig& config);
    
    // 运行仿真
    bool run();
    
    // 设置信道模型
    void setChannelModel(std::shared_ptr<channel::ChannelModel> model);
    
    // 获取信道模型
    std::shared_ptr<channel::ChannelModel> getChannelModel() const;
    
    // 设置仿真配置
    void setConfig(const SimulationConfig& config);
    
    // 获取仿真配置
    SimulationConfig getConfig() const;
    
    // 获取仿真结果
    std::vector<double> getResults() const;
    
    // 保存仿真结果
    bool saveResults(const std::string& filePath) const;

private:
    // 仿真配置
    SimulationConfig config;
    
    // 信道模型
    std::shared_ptr<channel::ChannelModel> channelModel;
    
    // 仿真结果
    std::vector<double> results;
    
    // 仿真状态
    bool initialized;
    bool completed;
};

} // namespace engine
} // namespace simulation
} // namespace link16
