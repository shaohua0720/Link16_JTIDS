#pragma once
#include <vector>
#include <complex>
#include <string>

namespace link16 {
namespace simulation {
namespace channel {

// 信道模型基类
class ChannelModel {
public:
    // 构造函数
    ChannelModel(double snr = 10.0);
    
    // 虚析构函数
    virtual ~ChannelModel();
    
    // 处理信号
    virtual std::vector<std::complex<double>> process(const std::vector<std::complex<double>>& input) = 0;
    
    // 设置信噪比(dB)
    void setSnr(double snr);
    
    // 获取信噪比(dB)
    double getSnr() const;
    
    // 设置信道参数
    virtual void setParameter(const std::string& name, double value);
    
    // 获取信道参数
    virtual double getParameter(const std::string& name) const;
    
    // 获取信道名称
    virtual std::string getName() const = 0;
    
    // 获取信道描述
    virtual std::string getDescription() const = 0;

protected:
    // 信噪比(dB)
    double snr;
    
    // 添加高斯白噪声
    std::vector<std::complex<double>> addNoise(const std::vector<std::complex<double>>& signal, double noiseLevel);
    
    // 计算信号功率
    double calculateSignalPower(const std::vector<std::complex<double>>& signal);
};

} // namespace channel
} // namespace simulation
} // namespace link16
