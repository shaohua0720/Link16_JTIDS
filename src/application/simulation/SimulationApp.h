#pragma once
#include <string>
#include "core/types/Link16Types.h"

namespace link16 {
namespace application {

class SimulationApp {
public:
    // 构造函数
    SimulationApp();
    
    // 析构函数
    ~SimulationApp();
    
    // 初始化仿真
    bool initialize(const std::string& configFile = "simulation_config.ini");
    
    // 运行仿真
    void run();
    
    // 设置仿真参数
    void setSimulationParams(const SimulationParams& params);
    
    // 获取仿真参数
    SimulationParams getSimulationParams() const;
    
    // 设置物理层参数
    void setPhysicalParams(const PhysicalParams& params);
    
    // 获取物理层参数
    PhysicalParams getPhysicalParams() const;
    
    // 关闭仿真
    void shutdown();

private:
    // 仿真状态
    bool initialized;
    
    // 仿真参数
    SimulationParams simParams;
    
    // 物理层参数
    PhysicalParams physParams;
    
    // 配置文件路径
    std::string configPath;
    
    // 结果文件路径
    std::string resultPath;
};

} // namespace application
} // namespace link16
