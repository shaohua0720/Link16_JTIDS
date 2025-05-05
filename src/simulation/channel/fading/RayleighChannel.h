#pragma once
#include "../base/ChannelModel.h"

namespace link16 {
namespace simulation {
namespace channel {

// 瑞利衰落信道模型
class RayleighChannel : public ChannelModel {
public:
    // 构造函数
    RayleighChannel(double snr = 10.0, double dopplerShift = 0.0);
    
    // 析构函数
    virtual ~RayleighChannel();
    
    // 处理信号
    virtual std::vector<std::complex<double>> process(const std::vector<std::complex<double>>& input) override;
    
    // 设置信道参数
    virtual void setParameter(const std::string& name, double value) override;
    
    // 获取信道参数
    virtual double getParameter(const std::string& name) const override;
    
    // 获取信道名称
    virtual std::string getName() const override;
    
    // 获取信道描述
    virtual std::string getDescription() const override;
    
    // 设置多普勒频移
    void setDopplerShift(double shift);
    
    // 获取多普勒频移
    double getDopplerShift() const;

private:
    // 多普勒频移(Hz)
    double dopplerShift;
    
    // 生成瑞利衰落系数
    std::vector<std::complex<double>> generateRayleighFading(size_t length);
};

} // namespace channel
} // namespace simulation
} // namespace link16
