#pragma once
#include <string>
#include <vector>
#include <memory>
#include "core/types/Link16Types.h"

namespace link16 {
namespace simulation {

// 端到端仿真接口
class EndToEndSimulation {
public:
    // 构造函数
    EndToEndSimulation();
    
    // 析构函数
    ~EndToEndSimulation();
    
    // 初始化
    bool initialize(const std::string& configFile = "");
    
    // 关闭
    void shutdown();
    
    // 运行仿真
    bool run();
    
    // 停止仿真
    void stop();
    
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
    
    // 设置测试消息
    void setTestMessage(const std::string& message);
    
    // 获取测试消息
    std::string getTestMessage() const;
    
    // 获取仿真结果
    std::vector<double> getResults() const;
    
    // 保存仿真结果
    bool saveResults(const std::string& resultFile);
    
    // 检查仿真是否正在运行
    bool isRunning() const;
    
    // 获取仿真进度(0.0-1.0)
    double getProgress() const;

private:
    // 仿真参数
    SimulationParams simParams;
    
    // 物理层参数
    PhysicalParams physParams;
    
    // 信道类型
    std::string channelType;
    
    // 测试消息
    std::string testMessage;
    
    // 仿真引擎
    std::shared_ptr<engine::SimulationEngine> simulationEngine;
    
    // 仿真结果
    std::vector<double> results;
    
    // 仿真状态
    bool running;
    
    // 仿真进度
    double progress;
    
    // 初始化状态
    bool initialized;
};

} // namespace simulation
} // namespace link16
