#include "USRPDevice.h"
#include "core/utils/logger.h"
#include <thread>
#include <chrono>

namespace link16 {
namespace physical {
namespace hardware {

// 构造函数
USRPDevice::USRPDevice() 
    : device(nullptr), txStream(nullptr), rxStream(nullptr),
      txFrequency(0.0), rxFrequency(0.0),
      txGain(0.0), rxGain(0.0),
      txSampleRate(0.0), rxSampleRate(0.0),
      txAntenna(""), rxAntenna(""),
      initialized(false) {
}

// 析构函数
USRPDevice::~USRPDevice() {
    close();
}

// 初始化设备
bool USRPDevice::initialize(const std::string& args) {
    LOG_INFO("初始化USRP设备: " + args);
    
    // 这里是一个模拟实现，实际应该使用UHD库
    // 在实际实现中，应该包含UHD头文件，并使用UHD API
    
    // 模拟初始化过程
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 设置默认参数
    txFrequency = 969.0e6;    // 969MHz
    rxFrequency = 969.0e6;    // 969MHz
    txGain = 20.0;            // 20dB
    rxGain = 30.0;            // 30dB
    txSampleRate = 1.0e6;     // 1MHz
    rxSampleRate = 1.0e6;     // 1MHz
    txAntenna = "TX/RX";      // 默认天线
    rxAntenna = "RX2";        // 默认天线
    
    initialized = true;
    
    LOG_INFO("USRP设备初始化成功");
    
    return true;
}

// 关闭设备
void USRPDevice::close() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("关闭USRP设备");
    
    // 释放资源
    device = nullptr;
    txStream = nullptr;
    rxStream = nullptr;
    
    initialized = false;
}

// 设置发送频率
bool USRPDevice::setTxFrequency(double frequency) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置发送频率: " + std::to_string(frequency / 1.0e6) + " MHz");
    
    // 检查频率范围
    if (frequency < 50.0e6 || frequency > 6.0e9) {
        LOG_ERROR("频率超出范围: " + std::to_string(frequency / 1.0e6) + " MHz");
        return false;
    }
    
    txFrequency = frequency;
    return true;
}

// 获取发送频率
double USRPDevice::getTxFrequency() const {
    return txFrequency;
}

// 设置接收频率
bool USRPDevice::setRxFrequency(double frequency) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置接收频率: " + std::to_string(frequency / 1.0e6) + " MHz");
    
    // 检查频率范围
    if (frequency < 50.0e6 || frequency > 6.0e9) {
        LOG_ERROR("频率超出范围: " + std::to_string(frequency / 1.0e6) + " MHz");
        return false;
    }
    
    rxFrequency = frequency;
    return true;
}

// 获取接收频率
double USRPDevice::getRxFrequency() const {
    return rxFrequency;
}

// 设置发送增益
bool USRPDevice::setTxGain(double gain) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置发送增益: " + std::to_string(gain) + " dB");
    
    // 检查增益范围
    if (gain < 0.0 || gain > 90.0) {
        LOG_ERROR("增益超出范围: " + std::to_string(gain) + " dB");
        return false;
    }
    
    txGain = gain;
    return true;
}

// 获取发送增益
double USRPDevice::getTxGain() const {
    return txGain;
}

// 设置接收增益
bool USRPDevice::setRxGain(double gain) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置接收增益: " + std::to_string(gain) + " dB");
    
    // 检查增益范围
    if (gain < 0.0 || gain > 90.0) {
        LOG_ERROR("增益超出范围: " + std::to_string(gain) + " dB");
        return false;
    }
    
    rxGain = gain;
    return true;
}

// 获取接收增益
double USRPDevice::getRxGain() const {
    return rxGain;
}

// 设置发送采样率
bool USRPDevice::setTxSampleRate(double rate) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置发送采样率: " + std::to_string(rate / 1.0e6) + " Msps");
    
    // 检查采样率范围
    if (rate < 1.0e5 || rate > 200.0e6) {
        LOG_ERROR("采样率超出范围: " + std::to_string(rate / 1.0e6) + " Msps");
        return false;
    }
    
    txSampleRate = rate;
    return true;
}

// 获取发送采样率
double USRPDevice::getTxSampleRate() const {
    return txSampleRate;
}

// 设置接收采样率
bool USRPDevice::setRxSampleRate(double rate) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置接收采样率: " + std::to_string(rate / 1.0e6) + " Msps");
    
    // 检查采样率范围
    if (rate < 1.0e5 || rate > 200.0e6) {
        LOG_ERROR("采样率超出范围: " + std::to_string(rate / 1.0e6) + " Msps");
        return false;
    }
    
    rxSampleRate = rate;
    return true;
}

// 获取接收采样率
double USRPDevice::getRxSampleRate() const {
    return rxSampleRate;
}

// 设置发送天线
bool USRPDevice::setTxAntenna(const std::string& antenna) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置发送天线: " + antenna);
    
    txAntenna = antenna;
    return true;
}

// 获取发送天线
std::string USRPDevice::getTxAntenna() const {
    return txAntenna;
}

// 设置接收天线
bool USRPDevice::setRxAntenna(const std::string& antenna) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("设置接收天线: " + antenna);
    
    rxAntenna = antenna;
    return true;
}

// 获取接收天线
std::string USRPDevice::getRxAntenna() const {
    return rxAntenna;
}

// 发送复数信号
bool USRPDevice::transmit(const std::vector<std::complex<float>>& samples) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("发送信号: " + std::to_string(samples.size()) + " 个样本");
    
    // 模拟发送过程
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(samples.size() / txSampleRate * 1000)));
    
    return true;
}

// 接收复数信号
bool USRPDevice::receive(std::vector<std::complex<float>>& samples, size_t numSamples, double timeout) {
    if (!initialized) {
        LOG_ERROR("USRP设备未初始化");
        return false;
    }
    
    LOG_INFO("接收信号: " + std::to_string(numSamples) + " 个样本，超时: " + std::to_string(timeout) + " 秒");
    
    // 模拟接收过程
    samples.resize(numSamples);
    
    // 生成随机数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0.0f, 0.1f);
    
    for (auto& sample : samples) {
        sample = std::complex<float>(dist(gen), dist(gen));
    }
    
    // 模拟接收延迟
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(numSamples / rxSampleRate * 1000)));
    
    return true;
}

// 获取设备信息
std::string USRPDevice::getDeviceInfo() const {
    if (!initialized) {
        return "USRP设备未初始化";
    }
    
    std::string info = "USRP设备信息:\n";
    info += "  发送频率: " + std::to_string(txFrequency / 1.0e6) + " MHz\n";
    info += "  接收频率: " + std::to_string(rxFrequency / 1.0e6) + " MHz\n";
    info += "  发送增益: " + std::to_string(txGain) + " dB\n";
    info += "  接收增益: " + std::to_string(rxGain) + " dB\n";
    info += "  发送采样率: " + std::to_string(txSampleRate / 1.0e6) + " Msps\n";
    info += "  接收采样率: " + std::to_string(rxSampleRate / 1.0e6) + " Msps\n";
    info += "  发送天线: " + txAntenna + "\n";
    info += "  接收天线: " + rxAntenna;
    
    return info;
}

// 检查设备是否已初始化
bool USRPDevice::isInitialized() const {
    return initialized;
}

// 获取可用设备列表
std::vector<std::string> USRPDevice::getAvailableDevices() {
    // 这里是一个模拟实现，实际应该使用UHD库
    
    std::vector<std::string> devices;
    devices.push_back("type=b200");
    devices.push_back("type=x300,addr=192.168.10.2");
    devices.push_back("type=n200,addr=192.168.10.3");
    
    return devices;
}

} // namespace hardware
} // namespace physical
} // namespace link16
