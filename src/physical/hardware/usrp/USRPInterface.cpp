#include "USRPInterface.h"
#include "core/utils/logger.h"
#include <iostream>

namespace link16 {
namespace physical {
namespace hardware {

// 构造函数
USRPInterface::USRPInterface()
    : frequency(0.0), sampleRate(0.0), gain(0.0), bandwidth(0.0), antenna(""), initialized(false) {
}

// 析构函数
USRPInterface::~USRPInterface() {
    if (initialized) {
        close();
    }
}

// 初始化USRP设备
bool USRPInterface::initialize(const std::string& args) {
    // 这里只是一个接口，实际实现将在后续完成
    // 在实际实现中，将使用UHD库初始化USRP设备
    
    deviceArgs = args;
    LOG_INFO("USRP设备初始化: " + args);
    
    // 设置默认参数
    frequency = 969.0e6;  // 969MHz
    sampleRate = 1.0e6;   // 1MHz
    gain = 20.0;          // 20dB
    bandwidth = 1.0e6;    // 1MHz
    antenna = "TX/RX";
    
    initialized = true;
    return true;
}

// 设置中心频率
bool USRPInterface::setFrequency(double freq) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP频率: " + std::to_string(freq / 1e6) + " MHz");
    frequency = freq;
    return true;
}

// 获取中心频率
double USRPInterface::getFrequency() const {
    return frequency;
}

// 设置采样率
bool USRPInterface::setSampleRate(double rate) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP采样率: " + std::to_string(rate / 1e6) + " Msps");
    sampleRate = rate;
    return true;
}

// 获取采样率
double USRPInterface::getSampleRate() const {
    return sampleRate;
}

// 设置增益
bool USRPInterface::setGain(double g) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP增益: " + std::to_string(g) + " dB");
    gain = g;
    return true;
}

// 获取增益
double USRPInterface::getGain() const {
    return gain;
}

// 设置带宽
bool USRPInterface::setBandwidth(double bw) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP带宽: " + std::to_string(bw / 1e6) + " MHz");
    bandwidth = bw;
    return true;
}

// 获取带宽
double USRPInterface::getBandwidth() const {
    return bandwidth;
}

// 设置天线
bool USRPInterface::setAntenna(const std::string& ant) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置USRP天线: " + ant);
    antenna = ant;
    return true;
}

// 获取天线
std::string USRPInterface::getAntenna() const {
    return antenna;
}

// 关闭USRP设备
void USRPInterface::close() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("关闭USRP设备");
    initialized = false;
}

// 获取设备信息
std::string USRPInterface::getDeviceInfo() const {
    if (!initialized) {
        return "USRP设备未初始化";
    }
    
    std::string info = "USRP设备信息:\n";
    info += "  设备参数: " + deviceArgs + "\n";
    info += "  频率: " + std::to_string(frequency / 1e6) + " MHz\n";
    info += "  采样率: " + std::to_string(sampleRate / 1e6) + " Msps\n";
    info += "  增益: " + std::to_string(gain) + " dB\n";
    info += "  带宽: " + std::to_string(bandwidth / 1e6) + " MHz\n";
    info += "  天线: " + antenna;
    
    return info;
}

// 检查设备是否已初始化
bool USRPInterface::isInitialized() const {
    return initialized;
}

} // namespace hardware
} // namespace physical
} // namespace link16
