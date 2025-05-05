#include "USRPReceiver.h"
#include "core/utils/logger.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

namespace link16 {
namespace physical {
namespace hardware {

// 构造函数
USRPReceiver::USRPReceiver()
    : deviceHandle(nullptr), rxStream(nullptr),
      rxFrequency(969.0e6), rxSampleRate(1.0e6), rxGain(30.0), rxBandwidth(1.0e6),
      rxAntenna("RX2"), rxChannel(0),
      initialized(false), agcEnabled(false), continuousReceiveRunning(false) {
}

// 析构函数
USRPReceiver::~USRPReceiver() {
    if (initialized) {
        close();
    }
}

// 初始化接收器
bool USRPReceiver::initialize(const std::string& args) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (initialized) {
        LOG_WARNING("USRP接收器已经初始化");
        return true;
    }

    LOG_INFO("初始化USRP接收器: " + args);

    // 这里是一个模拟实现，实际应该使用UHD库
    // 在实际实现中，将使用UHD库初始化USRP设备

    deviceArgs = args;

    // 设置默认参数
    rxFrequency = 969.0e6;  // 969MHz
    rxSampleRate = 1.0e6;   // 1MHz
    rxGain = 30.0;          // 30dB
    rxBandwidth = 1.0e6;    // 1MHz
    rxAntenna = "RX2";
    rxChannel = 0;
    agcEnabled = false;

    // 模拟设备句柄
    deviceHandle = new int(1);

    // 模拟接收流句柄
    rxStream = new int(2);

    initialized = true;
    LOG_INFO("USRP接收器初始化成功");

    return true;
}

// 关闭接收器
void USRPReceiver::close() {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        return;
    }

    LOG_INFO("关闭USRP接收器");

    // 停止连续接收
    if (continuousReceiveRunning) {
        stopContinuousReceive();
    }

    // 释放资源
    if (rxStream) {
        delete static_cast<int*>(rxStream);
        rxStream = nullptr;
    }

    if (deviceHandle) {
        delete static_cast<int*>(deviceHandle);
        deviceHandle = nullptr;
    }

    initialized = false;
}

// 设置频率
bool USRPReceiver::setFrequency(double freq) {
    return setRxFrequency(freq);
}

// 获取频率
double USRPReceiver::getFrequency() const {
    return rxFrequency;
}

// 设置采样率
bool USRPReceiver::setSampleRate(double rate) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("设置USRP接收采样率: " + std::to_string(rate / 1e6) + " Msps");
    rxSampleRate = rate;
    return true;
}

// 获取采样率
double USRPReceiver::getSampleRate() const {
    return rxSampleRate;
}

// 设置增益
bool USRPReceiver::setGain(double gain) {
    return setRxGain(gain);
}

// 获取增益
double USRPReceiver::getGain() const {
    return rxGain;
}

// 设置带宽
bool USRPReceiver::setBandwidth(double bandwidth) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("设置USRP接收带宽: " + std::to_string(bandwidth / 1e6) + " MHz");
    rxBandwidth = bandwidth;
    return true;
}

// 获取带宽
double USRPReceiver::getBandwidth() const {
    return rxBandwidth;
}

// 设置天线
bool USRPReceiver::setAntenna(const std::string& antenna) {
    return setRxAntenna(antenna);
}

// 获取天线
std::string USRPReceiver::getAntenna() const {
    return rxAntenna;
}

// 获取设备信息
std::string USRPReceiver::getDeviceInfo() const {
    if (!initialized) {
        return "USRP接收器未初始化";
    }

    std::string info = "USRP接收器信息:\n";
    info += "  设备参数: " + deviceArgs + "\n";
    info += "  频率: " + std::to_string(rxFrequency / 1e6) + " MHz\n";
    info += "  采样率: " + std::to_string(rxSampleRate / 1e6) + " Msps\n";
    info += "  增益: " + std::to_string(rxGain) + " dB\n";
    info += "  带宽: " + std::to_string(rxBandwidth / 1e6) + " MHz\n";
    info += "  天线: " + rxAntenna + "\n";
    info += "  通道: " + std::to_string(rxChannel) + "\n";
    info += "  AGC: " + std::string(agcEnabled ? "启用" : "禁用");

    return info;
}

// 检查设备是否已初始化
bool USRPReceiver::isInitialized() const {
    return initialized;
}

// 接收复数样本
bool USRPReceiver::receive(std::vector<std::complex<float>>& samples, size_t numSamples, double timeout) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("接收复数样本: " + std::to_string(numSamples) + " 个样本，超时: " + std::to_string(timeout) + " 秒");

    // 这里是一个模拟实现，实际应该使用UHD库
    // 在实际实现中，将使用UHD库接收复数样本

    // 模拟接收延迟
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(numSamples / rxSampleRate * 1000)));

    // 生成随机样本
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0.0f, 0.1f);

    samples.resize(numSamples);
    for (size_t i = 0; i < numSamples; ++i) {
        samples[i] = std::complex<float>(dist(gen), dist(gen));
    }

    return true;
}

// 接收实数样本
bool USRPReceiver::receive(std::vector<float>& samples, size_t numSamples, double timeout) {
    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("接收实数样本: " + std::to_string(numSamples) + " 个样本，超时: " + std::to_string(timeout) + " 秒");

    // 接收复数样本
    std::vector<std::complex<float>> complexSamples;
    if (!receive(complexSamples, numSamples, timeout)) {
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
bool USRPReceiver::receiveBits(std::string& bits, size_t numBits, double timeout) {
    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("接收二进制数据: " + std::to_string(numBits) + " 位，超时: " + std::to_string(timeout) + " 秒");

    // 接收实数样本
    std::vector<float> samples;
    if (!receive(samples, numBits, timeout)) {
        return false;
    }

    // 将实数样本转换为二进制数据
    bits.resize(numBits);
    for (size_t i = 0; i < numBits; ++i) {
        bits[i] = (samples[i] > 0.0f) ? '1' : '0';
    }

    return true;
}

// 设置接收通道
bool USRPReceiver::setRxChannel(size_t channel) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("设置USRP接收通道: " + std::to_string(channel));
    rxChannel = channel;
    return true;
}

// 获取接收通道
size_t USRPReceiver::getRxChannel() const {
    return rxChannel;
}

// 启动连续接收
bool USRPReceiver::startContinuousReceive(std::function<void(const std::vector<std::complex<float>>&)> callback) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    if (continuousReceiveRunning) {
        LOG_WARNING("连续接收已经在运行");
        return true;
    }

    LOG_INFO("启动连续接收");

    // 保存回调函数
    receiveCallback = callback;

    // 设置运行标志
    continuousReceiveRunning = true;

    // 启动接收线程
    receiveThread = std::thread(&USRPReceiver::continuousReceiveThread, this);

    return true;
}

// 停止连续接收
void USRPReceiver::stopContinuousReceive() {
    {
        std::lock_guard<std::mutex> lock(deviceMutex);

        if (!continuousReceiveRunning) {
            return;
        }

        LOG_INFO("停止连续接收");

        // 清除运行标志
        continuousReceiveRunning = false;
    }

    // 通知线程停止
    stopCondition.notify_all();

    // 等待线程结束
    if (receiveThread.joinable()) {
        receiveThread.join();
    }
}

// 获取信号强度
double USRPReceiver::getSignalStrength() const {
    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return -100.0;
    }

    // 接收样本
    std::vector<std::complex<float>> samples;
    const_cast<USRPReceiver*>(this)->receive(samples, 1000, 0.1);

    // 计算信号强度
    return calculateSignalStrength(samples);
}

// 设置自动增益控制
bool USRPReceiver::setAGC(bool enable) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO(std::string("设置USRP接收AGC: ") + (enable ? "启用" : "禁用"));
    agcEnabled = enable;
    return true;
}

// 获取自动增益控制状态
bool USRPReceiver::getAGC() const {
    return agcEnabled;
}

// 设置接收增益
bool USRPReceiver::setRxGain(double gain) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("设置USRP接收增益: " + std::to_string(gain) + " dB");
    rxGain = gain;
    return true;
}

// 获取接收增益
double USRPReceiver::getRxGain() const {
    return rxGain;
}

// 设置接收天线
bool USRPReceiver::setRxAntenna(const std::string& antenna) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("设置USRP接收天线: " + antenna);
    rxAntenna = antenna;
    return true;
}

// 获取接收天线
std::string USRPReceiver::getRxAntenna() const {
    return rxAntenna;
}

// 设置接收频率
bool USRPReceiver::setRxFrequency(double freq) {
    std::lock_guard<std::mutex> lock(deviceMutex);

    if (!initialized) {
        LOG_ERROR("USRP接收器未初始化");
        return false;
    }

    LOG_INFO("设置USRP接收频率: " + std::to_string(freq / 1e6) + " MHz");
    rxFrequency = freq;
    return true;
}

// 获取接收频率
double USRPReceiver::getRxFrequency() const {
    return rxFrequency;
}

// 连续接收线程函数
void USRPReceiver::continuousReceiveThread() {
    LOG_INFO("连续接收线程启动");

    // 缓冲区大小
    const size_t bufferSize = 1024;

    // 接收缓冲区
    std::vector<std::complex<float>> buffer(bufferSize);

    // 线程循环
    while (continuousReceiveRunning) {
        // 接收数据
        if (receive(buffer, bufferSize, 0.1)) {
            // 调用回调函数
            if (receiveCallback) {
                receiveCallback(buffer);
            }
        }

        // 检查是否需要停止
        std::unique_lock<std::mutex> lock(deviceMutex);
        stopCondition.wait_for(lock, std::chrono::milliseconds(10), [this] { return !continuousReceiveRunning; });
    }

    LOG_INFO("连续接收线程结束");
}

// 计算信号强度
double USRPReceiver::calculateSignalStrength(const std::vector<std::complex<float>>& samples) const {
    if (samples.empty()) {
        return -100.0;
    }

    // 计算平均功率
    double power = 0.0;
    for (const auto& sample : samples) {
        power += std::norm(sample);
    }
    power /= samples.size();

    // 转换为dBm
    double powerDBm = 10.0 * std::log10(power) + 30.0;

    return powerDBm;
}

} // namespace hardware
} // namespace physical
} // namespace link16
