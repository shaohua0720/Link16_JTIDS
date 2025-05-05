#pragma once
#include <string>
#include <vector>
#include <complex>

namespace link16 {
namespace physical {
namespace hardware {

// USRP接口类 - 纯虚基类
class USRPInterface {
public:
    // 虚析构函数
    virtual ~USRPInterface() {}

    // 初始化USRP设备
    virtual bool initialize(const std::string& args = "") = 0;

    // 关闭USRP设备
    virtual void close() = 0;

    // 设置中心频率
    virtual bool setFrequency(double freq) = 0;

    // 获取中心频率
    virtual double getFrequency() const = 0;

    // 设置采样率
    virtual bool setSampleRate(double rate) = 0;

    // 获取采样率
    virtual double getSampleRate() const = 0;

    // 设置增益
    virtual bool setGain(double gain) = 0;

    // 获取增益
    virtual double getGain() const = 0;

    // 设置带宽
    virtual bool setBandwidth(double bandwidth) = 0;

    // 获取带宽
    virtual double getBandwidth() const = 0;

    // 设置天线
    virtual bool setAntenna(const std::string& antenna) = 0;

    // 获取天线
    virtual std::string getAntenna() const = 0;

    // 获取设备信息
    virtual std::string getDeviceInfo() const = 0;

    // 检查设备是否已初始化
    virtual bool isInitialized() const = 0;

    // 获取可用设备列表
    static std::vector<std::string> getAvailableDevices();
};

} // namespace hardware
} // namespace physical
} // namespace link16
