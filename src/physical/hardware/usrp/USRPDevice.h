#pragma once
#include <string>
#include <vector>
#include <complex>
#include <memory>

namespace link16 {
namespace physical {
namespace hardware {

// USRP设备类
class USRPDevice {
public:
    // 构造函数
    USRPDevice();
    
    // 析构函数
    ~USRPDevice();
    
    // 初始化设备
    bool initialize(const std::string& args = "");
    
    // 关闭设备
    void close();
    
    // 设置发送频率
    bool setTxFrequency(double frequency);
    
    // 获取发送频率
    double getTxFrequency() const;
    
    // 设置接收频率
    bool setRxFrequency(double frequency);
    
    // 获取接收频率
    double getRxFrequency() const;
    
    // 设置发送增益
    bool setTxGain(double gain);
    
    // 获取发送增益
    double getTxGain() const;
    
    // 设置接收增益
    bool setRxGain(double gain);
    
    // 获取接收增益
    double getRxGain() const;
    
    // 设置发送采样率
    bool setTxSampleRate(double rate);
    
    // 获取发送采样率
    double getTxSampleRate() const;
    
    // 设置接收采样率
    bool setRxSampleRate(double rate);
    
    // 获取接收采样率
    double getRxSampleRate() const;
    
    // 设置发送天线
    bool setTxAntenna(const std::string& antenna);
    
    // 获取发送天线
    std::string getTxAntenna() const;
    
    // 设置接收天线
    bool setRxAntenna(const std::string& antenna);
    
    // 获取接收天线
    std::string getRxAntenna() const;
    
    // 发送复数信号
    bool transmit(const std::vector<std::complex<float>>& samples);
    
    // 接收复数信号
    bool receive(std::vector<std::complex<float>>& samples, size_t numSamples, double timeout = 1.0);
    
    // 获取设备信息
    std::string getDeviceInfo() const;
    
    // 检查设备是否已初始化
    bool isInitialized() const;
    
    // 获取可用设备列表
    static std::vector<std::string> getAvailableDevices();

private:
    // 设备句柄(使用void*避免包含UHD头文件)
    void* device;
    
    // 发送流句柄
    void* txStream;
    
    // 接收流句柄
    void* rxStream;
    
    // 发送频率
    double txFrequency;
    
    // 接收频率
    double rxFrequency;
    
    // 发送增益
    double txGain;
    
    // 接收增益
    double rxGain;
    
    // 发送采样率
    double txSampleRate;
    
    // 接收采样率
    double rxSampleRate;
    
    // 发送天线
    std::string txAntenna;
    
    // 接收天线
    std::string rxAntenna;
    
    // 初始化状态
    bool initialized;
};

} // namespace hardware
} // namespace physical
} // namespace link16
