#pragma once
#include <string>
#include <vector>
#include <complex>
#include <memory>
#include "core/types/Link16Types.h"

namespace link16 {
namespace simulation {

// 信道仿真接口
class ChannelSimulation {
public:
    // 构造函数
    ChannelSimulation();
    
    // 析构函数
    ~ChannelSimulation();
    
    // 初始化
    bool initialize(const std::string& channelType, const SimulationParams& params);
    
    // 关闭
    void shutdown();
    
    // 处理信号
    bool processSignal(const std::vector<std::complex<double>>& input, std::vector<std::complex<double>>& output);
    
    // 设置信噪比
    void setSnr(double snr);
    
    // 获取信噪比
    double getSnr() const;
    
    // 设置信道类型
    bool setChannelType(const std::string& type);
    
    // 获取信道类型
    std::string getChannelType() const;
    
    // 设置仿真参数
    void setSimulationParams(const SimulationParams& params);
    
    // 获取仿真参数
    SimulationParams getSimulationParams() const;
    
    // 获取误码率
    double getBER() const;

private:
    // 信道类型
    std::string channelType;
    
    // 仿真参数
    SimulationParams simParams;
    
    // 信道模型
    std::shared_ptr<channel::ChannelModel> channelModel;
    
    // 初始化状态
    bool initialized;
};

} // namespace simulation
} // namespace link16
