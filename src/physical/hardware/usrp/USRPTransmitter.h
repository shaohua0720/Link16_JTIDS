#pragma once
#include "USRPInterface.h"
#include <vector>
#include <complex>
#include <string>

namespace link16 {
namespace physical {
namespace hardware {

class USRPTransmitter : public USRPInterface {
public:
    // 构造函数
    USRPTransmitter();
    
    // 析构函数
    virtual ~USRPTransmitter();
    
    // 初始化发送器
    virtual bool initialize(const std::string& args = "") override;
    
    // 发送复数样本
    bool transmit(const std::vector<std::complex<double>>& samples);
    
    // 发送实数样本
    bool transmit(const std::vector<double>& samples);
    
    // 发送二进制数据
    bool transmitBits(const std::string& bits);
    
    // 设置发送增益
    bool setTxGain(double gain);
    
    // 获取发送增益
    double getTxGain() const;
    
    // 设置发送天线
    bool setTxAntenna(const std::string& antenna);
    
    // 获取发送天线
    std::string getTxAntenna() const;
    
    // 设置发送频率
    bool setTxFrequency(double freq);
    
    // 获取发送频率
    double getTxFrequency() const;

private:
    // 发送增益
    double txGain;
    
    // 发送天线
    std::string txAntenna;
    
    // 发送频率
    double txFrequency;
};

} // namespace hardware
} // namespace physical
} // namespace link16
