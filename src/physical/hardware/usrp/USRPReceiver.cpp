#include "USRPReceiver.h"
#include "core/utils/logger.h"
#include <iostream>
#include <random>

namespace link16 {
namespace physical {
namespace hardware {

// 构造函数
USRPReceiver::USRPReceiver()
    : USRPInterface(), rxGain(0.0), rxAntenna("RX2"), rxFrequency(0.0) {
}

// 析构函数
USRPReceiver::~USRPReceiver() {
}

// 初始化接收器
bool USRPReceiver::initialize(const std::string& args) {
    if (!USRPInterface::initialize(args)) {
        return false;
    }
    
    LOG_INFO("初始化USRP接收器");
    
    // 设置默认参数
    rxGain = 30.0;          // 30dB
    rxAntenna = "RX2";
    rxFrequency = 969.0e6;  // 969MHz
    
    return true;
}

// 接收复数样本
bool USRPReceiver::receive(std::vector<std::complex<double>>& samples, size_t numSamples) {
    if (!isInitialized()) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }
    
    LOG_INFO("接收复数样本: " + std::to_string(numSamples) + " 个样本");
    
    // 这里只是一个接口，实际实现将在后续完成
    // 在实际实现中，将使用UHD库接收复数样本
    
    // 为了编译通过，生成随机样本
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);
    
    samples.resize(numSamples);
    for (size_t i = 0; i < numSamples; ++i) {
        samples[i] = std::complex<double>(dist(gen), dist(gen));
    }
    
    return true;
}

// 接收实数样本
bool USRPReceiver::receive(std::vector<double>& samples, size_t numSamples) {
    if (!isInitialized()) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }
    
    LOG_INFO("接收实数样本: " + std::to_string(numSamples) + " 个样本");
    
    // 接收复数样本
    std::vector<std::complex<double>> complexSamples;
    if (!receive(complexSamples, numSamples)) {
        return false;
    }
    
    // 提取实部
    samples.resize(numSamples);
    for (size_t i = 0; i < numSamples; ++i) {
        samples[i] = complexSamples[i].real();
    }
    
    return true;
}

// 接收二进制数据
bool USRPReceiver::receiveBits(std::string& bits, size_t numBits) {
    if (!isInitialized()) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }
    
    LOG_INFO("接收二进制数据: " + std::to_string(numBits) + " 位");
    
    // 接收实数样本
    std::vector<double> samples;
    if (!receive(samples, numBits)) {
        return false;
    }
    
    // 将实数样本转换为二进制数据
    bits.resize(numBits);
    for (size_t i = 0; i < numBits; ++i) {
        bits[i] = (samples[i] > 0.0) ? '1' : '0';
    }
    
    return true;
}

// 设置接收增益
bool USRPReceiver::setRxGain(double gain) {
    if (!isInitialized()) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP接收增益: " + std::to_string(gain) + " dB");
    rxGain = gain;
    return setGain(gain);
}

// 获取接收增益
double USRPReceiver::getRxGain() const {
    return rxGain;
}

// 设置接收天线
bool USRPReceiver::setRxAntenna(const std::string& antenna) {
    if (!isInitialized()) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP接收天线: " + antenna);
    rxAntenna = antenna;
    return setAntenna(antenna);
}

// 获取接收天线
std::string USRPReceiver::getRxAntenna() const {
    return rxAntenna;
}

// 设置接收频率
bool USRPReceiver::setRxFrequency(double freq) {
    if (!isInitialized()) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP接收频率: " + std::to_string(freq / 1e6) + " MHz");
    rxFrequency = freq;
    return setFrequency(freq);
}

// 获取接收频率
double USRPReceiver::getRxFrequency() const {
    return rxFrequency;
}

} // namespace hardware
} // namespace physical
} // namespace link16
