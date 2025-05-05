#include "USRPTransmitter.h"
#include "core/utils/logger.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

namespace link16 {
namespace physical {
namespace hardware {

// 构造函数
USRPTransmitter::USRPTransmitter()
    : deviceHandle(nullptr), txStream(nullptr),
      txFrequency(969.0e6), txSampleRate(1.0e6), txGain(20.0), txBandwidth(1.0e6),
      txAntenna("TX/RX"), txChannel(0), txPower(10.0),
      initialized(false), continuousTransmitRunning(false) {
}

// 析构函数
USRPTransmitter::~USRPTransmitter() {
    if (initialized) {
        close();
    }
}

// 初始化发送器
bool USRPTransmitter::initialize(const std::string& args) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (initialized) {
        LOG_WARNING("USRP发送器已经初始化");
        return true;
    }

    LOG_INFO("初始化USRP发送器: " + args);

    // 这里是一个模拟实现，实际应该使用UHD库
    // 在实际实现中，将使用UHD库初始化USRP设备

    deviceArgs = args;

    // 设置默认参数
    txFrequency = 969.0e6;  // 969MHz
    txSampleRate = 1.0e6;   // 1MHz
    txGain = 20.0;          // 20dB
    txBandwidth = 1.0e6;    // 1MHz
    txAntenna = "TX/RX";
    txChannel = 0;
    txPower = 10.0;         // 10dBm

    // 模拟设备句柄
    deviceHandle = new int(1);

    // 模拟发送流句柄
    txStream = new int(2);

    initialized = true;
    LOG_INFO("USRP发送器初始化成功");

    return true;
}

// 关闭发送器
void USRPTransmitter::close() {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        return;
    }

    LOG_INFO("关闭USRP发送器");

    // 停止连续发送
    if (continuousTransmitRunning) {
        stopContinuousTransmit();
    }

    // 释放资源
    if (txStream) {
        delete static_cast<int*>(txStream);
        txStream = nullptr;
    }

    if (deviceHandle) {
        delete static_cast<int*>(deviceHandle);
        deviceHandle = nullptr;
    }

    initialized = false;
}

// 设置频率
bool USRPTransmitter::setFrequency(double freq) {
    return setTxFrequency(freq);
}

// 获取频率
double USRPTransmitter::getFrequency() const {
    return txFrequency;
}

// 设置采样率
bool USRPTransmitter::setSampleRate(double rate) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("设置USRP发送采样率: " + std::to_string(rate / 1e6) + " Msps");
    txSampleRate = rate;
    return true;
}

// 获取采样率
double USRPTransmitter::getSampleRate() const {
    return txSampleRate;
}

// 设置增益
bool USRPTransmitter::setGain(double gain) {
    return setTxGain(gain);
}

// 获取增益
double USRPTransmitter::getGain() const {
    return txGain;
}

// 设置带宽
bool USRPTransmitter::setBandwidth(double bandwidth) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("设置USRP发送带宽: " + std::to_string(bandwidth / 1e6) + " MHz");
    txBandwidth = bandwidth;
    return true;
}

// 获取带宽
double USRPTransmitter::getBandwidth() const {
    return txBandwidth;
}

// 设置天线
bool USRPTransmitter::setAntenna(const std::string& antenna) {
    return setTxAntenna(antenna);
}

// 获取天线
std::string USRPTransmitter::getAntenna() const {
    return txAntenna;
}

// 获取设备信息
std::string USRPTransmitter::getDeviceInfo() const {
    if (!initialized) {
        return "USRP发送器未初始化";
    }

    std::string info = "USRP发送器信息:\n";
    info += "  设备参数: " + deviceArgs + "\n";
    info += "  频率: " + std::to_string(txFrequency / 1e6) + " MHz\n";
    info += "  采样率: " + std::to_string(txSampleRate / 1e6) + " Msps\n";
    info += "  增益: " + std::to_string(txGain) + " dB\n";
    info += "  带宽: " + std::to_string(txBandwidth / 1e6) + " MHz\n";
    info += "  天线: " + txAntenna + "\n";
    info += "  通道: " + std::to_string(txChannel) + "\n";
    info += "  功率: " + std::to_string(txPower) + " dBm";

    return info;
}

// 检查设备是否已初始化
bool USRPTransmitter::isInitialized() const {
    return initialized;
}

// 发送复数样本
bool USRPTransmitter::transmit(const std::vector<std::complex<float>>& samples) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("发送复数样本: " + std::to_string(samples.size()) + " 个样本");

    // 这里是一个模拟实现，实际应该使用UHD库
    // 在实际实现中，将使用UHD库发送复数样本

    // 模拟发送延迟
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(samples.size() / txSampleRate * 1000)));

    return true;
}

// 发送实数样本
bool USRPTransmitter::transmit(const std::vector<float>& samples) {
    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("发送实数样本: " + std::to_string(samples.size()) + " 个样本");

    // 将实数样本转换为复数样本
    std::vector<std::complex<float>> complexSamples;
    complexSamples.reserve(samples.size());
    for (const auto& sample : samples) {
        complexSamples.push_back(std::complex<float>(sample, 0.0f));
    }

    // 调用复数样本发送函数
    return transmit(complexSamples);
}

// 发送二进制数据
bool USRPTransmitter::transmitBits(const std::string& bits) {
    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("发送二进制数据: " + std::to_string(bits.length()) + " 位");

    // 将二进制数据转换为实数样本
    std::vector<float> samples;
    samples.reserve(bits.length());
    for (const auto& bit : bits) {
        if (bit == '0') {
            samples.push_back(-1.0f);
        } else if (bit == '1') {
            samples.push_back(1.0f);
        } else {
            LOG_ERROR("无效的二进制数据: " + std::string(1, bit));
            return false;
        }
    }

    // 调用实数样本发送函数
    return transmit(samples);
}

// 设置发送通道
bool USRPTransmitter::setTxChannel(size_t channel) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("设置USRP发送通道: " + std::to_string(channel));
    txChannel = channel;
    return true;
}

// 获取发送通道
size_t USRPTransmitter::getTxChannel() const {
    return txChannel;
}

// 启动连续发送
bool USRPTransmitter::startContinuousTransmit(const std::vector<std::complex<float>>& samples, size_t repeatCount) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    if (continuousTransmitRunning) {
        LOG_WARNING("连续发送已经在运行");
        return true;
    }

    LOG_INFO("启动连续发送，重复次数: " + std::to_string(repeatCount));

    // 保存发送数据
    transmitBuffer = samples;
    this->repeatCount = repeatCount;

    // 设置运行标志
    continuousTransmitRunning = true;

    // 启动发送线程
    transmitThread = std::thread(&USRPTransmitter::continuousTransmitThread, this);

    return true;
}

// 停止连续发送
void USRPTransmitter::stopContinuousTransmit() {
    {
        std::lock_guard<std::mutex> lock(deviceMutex);

        if (!continuousTransmitRunning) {
            return;
        }

        LOG_INFO("停止连续发送");

        // 清除运行标志
        continuousTransmitRunning = false;
    }

    // 通知线程停止
    stopCondition.notify_all();

    // 等待线程结束
    if (transmitThread.joinable()) {
        transmitThread.join();
    }
}

// 设置发送功率
bool USRPTransmitter::setTxPower(double power) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("设置USRP发送功率: " + std::to_string(power) + " dBm");
    txPower = power;
    return true;
}

// 获取发送功率
double USRPTransmitter::getTxPower() const {
    return txPower;
}

// 设置发送增益
bool USRPTransmitter::setTxGain(double gain) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("设置USRP发送增益: " + std::to_string(gain) + " dB");
    txGain = gain;
    return true;
}

// 获取发送增益
double USRPTransmitter::getTxGain() const {
    return txGain;
}

// 设置发送天线
bool USRPTransmitter::setTxAntenna(const std::string& antenna) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("设置USRP发送天线: " + antenna);
    txAntenna = antenna;
    return true;
}

// 获取发送天线
std::string USRPTransmitter::getTxAntenna() const {
    return txAntenna;
}

// 设置发送频率
bool USRPTransmitter::setTxFrequency(double freq) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP发送器未初始化");
        return false;
    }

    LOG_INFO("设置USRP发送频率: " + std::to_string(freq / 1e6) + " MHz");
    txFrequency = freq;
    return true;
}

// 获取发送频率
double USRPTransmitter::getTxFrequency() const {
    return txFrequency;
}

// 连续发送线程函数
void USRPTransmitter::continuousTransmitThread() {
    LOG_INFO("连续发送线程启动");

    // 发送计数
    size_t count = 0;

    // 线程循环
    while (continuousTransmitRunning) {
        // 发送数据
        transmit(transmitBuffer);

        // 更新计数
        count++;

        // 检查是否达到重复次数
        if (repeatCount > 0 && count >= repeatCount) {
            LOG_INFO("达到重复次数，停止连续发送");
            continuousTransmitRunning = false;
            break;
        }

        // 检查是否需要停止
        std::unique_lock<std::mutex> lock(deviceMutex);
        stopCondition.wait_for(lock, std::chrono::milliseconds(10), [this] { return !continuousTransmitRunning; });
    }

    LOG_INFO("连续发送线程结束");
}

} // namespace hardware
} // namespace physical
} // namespace link16
