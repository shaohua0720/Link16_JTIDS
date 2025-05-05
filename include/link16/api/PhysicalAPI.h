#pragma once
#include <string>
#include <vector>
#include <complex>
#include <cstdint>

namespace link16 {
namespace api {

// 物理层API
class PhysicalAPI {
public:
    // 获取单例实例
    static PhysicalAPI& getInstance();
    
    // 初始化
    bool initialize();
    
    // 关闭
    void shutdown();
    
    // 初始化USRP设备
    bool initializeUSRP(const std::string& args = "");
    
    // 关闭USRP设备
    void closeUSRP();
    
    // 发送数据
    bool transmit(const std::string& data);
    
    // 接收数据
    bool receive(std::string& data, int timeout = 1000);
    
    // 设置发送频率
    bool setTxFrequency(double freq);
    
    // 获取发送频率
    double getTxFrequency() const;
    
    // 设置接收频率
    bool setRxFrequency(double freq);
    
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
    
    // 设置采样率
    bool setSampleRate(double rate);
    
    // 获取采样率
    double getSampleRate() const;
    
    // 设置调制方式
    bool setModulation(const std::string& modulation);
    
    // 获取调制方式
    std::string getModulation() const;
    
    // 设置跳频模式
    bool setFrequencyHoppingPattern(int pattern);
    
    // 获取跳频模式
    int getFrequencyHoppingPattern() const;
    
    // 启用跳频
    bool enableFrequencyHopping(bool enable);
    
    // 检查跳频是否启用
    bool isFrequencyHoppingEnabled() const;

private:
    // 私有构造函数(单例模式)
    PhysicalAPI();
    
    // 禁止拷贝和赋值
    PhysicalAPI(const PhysicalAPI&) = delete;
    PhysicalAPI& operator=(const PhysicalAPI&) = delete;
    
    // 初始化状态
    bool initialized;
    
    // USRP状态
    bool usrpInitialized;
    
    // 调制方式
    std::string modulation;
    
    // 跳频模式
    int frequencyHoppingPattern;
    
    // 跳频启用状态
    bool frequencyHoppingEnabled;
};

// 全局函数
bool initializeUSRP(const std::string& args = "");
void closeUSRP();
bool transmit(const std::string& data);
bool receive(std::string& data, int timeout = 1000);
bool setTxFrequency(double freq);
double getTxFrequency();
bool setRxFrequency(double freq);
double getRxFrequency();
bool setTxGain(double gain);
double getTxGain();
bool setRxGain(double gain);
double getRxGain();
bool setSampleRate(double rate);
double getSampleRate();
bool setModulation(const std::string& modulation);
std::string getModulation();
bool setFrequencyHoppingPattern(int pattern);
int getFrequencyHoppingPattern();
bool enableFrequencyHopping(bool enable);
bool isFrequencyHoppingEnabled();

} // namespace api
} // namespace link16
