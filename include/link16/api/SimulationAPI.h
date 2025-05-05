#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "core/types/Link16Types.h"

namespace link16 {
namespace api {

// 仿真API
class SimulationAPI {
public:
    // 获取单例实例
    static SimulationAPI& getInstance();
    
    // 初始化
    bool initialize();
    
    // 关闭
    void shutdown();
    
    // 加载仿真配置
    bool loadConfig(const std::string& configFile);
    
    // 保存仿真配置
    bool saveConfig(const std::string& configFile);
    
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
    
    // 运行仿真
    bool runSimulation();
    
    // 停止仿真
    void stopSimulation();
    
    // 获取仿真结果
    std::vector<double> getResults() const;
    
    // 保存仿真结果
    bool saveResults(const std::string& resultFile);
    
    // 检查仿真是否正在运行
    bool isSimulationRunning() const;
    
    // 获取仿真进度(0.0-1.0)
    double getSimulationProgress() const;

private:
    // 私有构造函数(单例模式)
    SimulationAPI();
    
    // 禁止拷贝和赋值
    SimulationAPI(const SimulationAPI&) = delete;
    SimulationAPI& operator=(const SimulationAPI&) = delete;
    
    // 初始化状态
    bool initialized;
    
    // 仿真参数
    SimulationParams simParams;
    
    // 物理层参数
    PhysicalParams physParams;
    
    // 信道类型
    std::string channelType;
    
    // 仿真结果
    std::vector<double> results;
    
    // 仿真状态
    bool running;
    
    // 仿真进度
    double progress;
};

// 全局函数
bool loadSimulationConfig(const std::string& configFile);
bool saveSimulationConfig(const std::string& configFile);
void setSimulationParams(const SimulationParams& params);
SimulationParams getSimulationParams();
void setPhysicalParams(const PhysicalParams& params);
PhysicalParams getPhysicalParams();
void setChannelType(const std::string& type);
std::string getChannelType();
bool runSimulation();
void stopSimulation();
std::vector<double> getSimulationResults();
bool saveSimulationResults(const std::string& resultFile);
bool isSimulationRunning();
double getSimulationProgress();

} // namespace api
} // namespace link16
