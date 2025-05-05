#pragma once
#include <string>
#include <vector>
#include <complex>

namespace link16 {
namespace physical {
namespace hardware {

// USRP接口类
class USRPInterface {
public:
    // 构造函数
    USRPInterface();
    
    // 析构函数
    virtual ~USRPInterface();
    
    // 初始化USRP设备
    virtual bool initialize(const std::string& args = "");
    
    // 设置中心频率
    virtual bool setFrequency(double freq);
    
    // 获取中心频率
    virtual double getFrequency() const;
    
    // 设置采样率
    virtual bool setSampleRate(double rate);
    
    // 获取采样率
    virtual double getSampleRate() const;
    
    // 设置增益
    virtual bool setGain(double gain);
    
    // 获取增益
    virtual double getGain() const;
    
    // 设置带宽
    virtual bool setBandwidth(double bandwidth);
    
    // 获取带宽
    virtual double getBandwidth() const;
    
    // 设置天线
    virtual bool setAntenna(const std::string& antenna);
    
    // 获取天线
    virtual std::string getAntenna() const;
    
    // 关闭USRP设备
    virtual void close();
    
    // 获取设备信息
    virtual std::string getDeviceInfo() const;
    
    // 检查设备是否已初始化
    virtual bool isInitialized() const;

protected:
    // 设备参数
    std::string deviceArgs;
    double frequency;
    double sampleRate;
    double gain;
    double bandwidth;
    std::string antenna;
    
    // 设备状态
    bool initialized;
};

} // namespace hardware
} // namespace physical
} // namespace link16
