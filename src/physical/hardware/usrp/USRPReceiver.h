#pragma once
#include "USRPInterface.h"
#include <vector>
#include <complex>
#include <string>

namespace link16 {
namespace physical {
namespace hardware {

class USRPReceiver : public USRPInterface {
public:
    // 构造函数
    USRPReceiver();
    
    // 析构函数
    virtual ~USRPReceiver();
    
    // 初始化接收器
    virtual bool initialize(const std::string& args = "") override;
    
    // 接收复数样本
    bool receive(std::vector<std::complex<double>>& samples, size_t numSamples);
    
    // 接收实数样本
    bool receive(std::vector<double>& samples, size_t numSamples);
    
    // 接收二进制数据
    bool receiveBits(std::string& bits, size_t numBits);
    
    // 设置接收增益
    bool setRxGain(double gain);
    
    // 获取接收增益
    double getRxGain() const;
    
    // 设置接收天线
    bool setRxAntenna(const std::string& antenna);
    
    // 获取接收天线
    std::string getRxAntenna() const;
    
    // 设置接收频率
    bool setRxFrequency(double freq);
    
    // 获取接收频率
    double getRxFrequency() const;

private:
    // 接收增益
    double rxGain;
    
    // 接收天线
    std::string rxAntenna;
    
    // 接收频率
    double rxFrequency;
};

} // namespace hardware
} // namespace physical
} // namespace link16
