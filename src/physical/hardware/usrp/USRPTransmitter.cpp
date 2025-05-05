#include "USRPTransmitter.h"
#include "core/utils/logger.h"
#include <iostream>

namespace link16 {
namespace physical {
namespace hardware {

// 构造函数
USRPTransmitter::USRPTransmitter()
    : USRPInterface(), txGain(0.0), txAntenna("TX/RX"), txFrequency(0.0) {
}

// 析构函数
USRPTransmitter::~USRPTransmitter() {
}

// 初始化发送器
bool USRPTransmitter::initialize(const std::string& args) {
    if (!USRPInterface::initialize(args)) {
        return false;
    }
    
    LOG_INFO("初始化USRP发送器");
    
    // 设置默认参数
    txGain = 20.0;          // 20dB
    txAntenna = "TX/RX";
    txFrequency = 969.0e6;  // 969MHz
    
    return true;
}

// 发送复数样本
bool USRPTransmitter::transmit(const std::vector<std::complex<double>>& samples) {
    if (!isInitialized()) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }
    
    LOG_INFO("发送复数样本: " + std::to_string(samples.size()) + " 个样本");
    
    // 这里只是一个接口，实际实现将在后续完成
    // 在实际实现中，将使用UHD库发送复数样本
    
    return true;
}

// 发送实数样本
bool USRPTransmitter::transmit(const std::vector<double>& samples) {
    if (!isInitialized()) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }
    
    LOG_INFO("发送实数样本: " + std::to_string(samples.size()) + " 个样本");
    
    // 将实数样本转换为复数样本
    std::vector<std::complex<double>> complexSamples;
    complexSamples.reserve(samples.size());
    for (const auto& sample : samples) {
        complexSamples.push_back(std::complex<double>(sample, 0.0));
    }
    
    // 调用复数样本发送函数
    return transmit(complexSamples);
}

// 发送二进制数据
bool USRPTransmitter::transmitBits(const std::string& bits) {
    if (!isInitialized()) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }
    
    LOG_INFO("发送二进制数据: " + std::to_string(bits.length()) + " 位");
    
    // 将二进制数据转换为实数样本
    std::vector<double> samples;
    samples.reserve(bits.length());
    for (const auto& bit : bits) {
        if (bit == '0') {
            samples.push_back(-1.0);
        } else if (bit == '1') {
            samples.push_back(1.0);
        } else {
            LOG_ERROR("无效的二进制数据: " + std::string(1, bit));
            return false;
        }
    }
    
    // 调用实数样本发送函数
    return transmit(samples);
}

// 设置发送增益
bool USRPTransmitter::setTxGain(double gain) {
    if (!isInitialized()) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP发送增益: " + std::to_string(gain) + " dB");
    txGain = gain;
    return setGain(gain);
}

// 获取发送增益
double USRPTransmitter::getTxGain() const {
    return txGain;
}

// 设置发送天线
bool USRPTransmitter::setTxAntenna(const std::string& antenna) {
    if (!isInitialized()) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP发送天线: " + antenna);
    txAntenna = antenna;
    return setAntenna(antenna);
}

// 获取发送天线
std::string USRPTransmitter::getTxAntenna() const {
    return txAntenna;
}

// 设置发送频率
bool USRPTransmitter::setTxFrequency(double freq) {
    if (!isInitialized()) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP发送频率: " + std::to_string(freq / 1e6) + " MHz");
    txFrequency = freq;
    return setFrequency(freq);
}

// 获取发送频率
double USRPTransmitter::getTxFrequency() const {
    return txFrequency;
}

} // namespace hardware
} // namespace physical
} // namespace link16
