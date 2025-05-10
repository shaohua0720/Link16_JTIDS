#pragma once

#include <string>
#include <vector>
#include <complex>
#include <memory>

namespace link16 {
namespace physical {

// 前向声明
class Modulator;
class FrequencyHopping;
class Synchronizer;
class Transmitter;
class Receiver;

/**
 * @brief 物理层处理器接口
 */
class PhysicalProcessor {
public:
    // 构造函数
    PhysicalProcessor();
    
    // 析构函数
    virtual ~PhysicalProcessor();
    
    // 初始化
    bool initialize();
    
    // 关闭
    void shutdown();
    
    // 发送数据
    bool transmitData(const std::string& data);
    
    // 接收数据
    bool receiveData(std::string& data, int timeout = 1000);
    
    // 调制
    bool modulate(const std::string& bitStream, std::vector<std::complex<double>>& symbols);
    
    // 解调
    bool demodulate(const std::vector<std::complex<double>>& symbols, std::string& bitStream);
    
    // 跳频
    bool frequencyHop(const std::vector<std::complex<double>>& inputSignal, std::vector<std::complex<double>>& outputSignal);
    
    // 解跳频
    bool frequencyDeHop(const std::vector<std::complex<double>>& inputSignal, std::vector<std::complex<double>>& outputSignal);
    
    // 同步
    bool synchronize(const std::vector<std::complex<double>>& signal);
    
    // 设置调制方式
    void setModulationType(const std::string& modulationType);
    
    // 获取调制方式
    const std::string& getModulationType() const;
    
    // 设置跳频模式
    void setHoppingPattern(const std::string& hoppingPattern);
    
    // 获取跳频模式
    const std::string& getHoppingPattern() const;
    
    // 设置中心频率
    void setCenterFrequency(double frequency);
    
    // 获取中心频率
    double getCenterFrequency() const;
    
    // 设置采样率
    void setSampleRate(double sampleRate);
    
    // 获取采样率
    double getSampleRate() const;
    
    // 设置发射功率
    void setTransmitPower(double power);
    
    // 获取发射功率
    double getTransmitPower() const;

private:
    // 调制器
    std::unique_ptr<Modulator> modulator;
    
    // 跳频器
    std::unique_ptr<FrequencyHopping> frequencyHopper;
    
    // 同步器
    std::unique_ptr<Synchronizer> synchronizer;
    
    // 发射器
    std::unique_ptr<Transmitter> transmitter;
    
    // 接收器
    std::unique_ptr<Receiver> receiver;
    
    // 调制方式
    std::string modulationType;
    
    // 跳频模式
    std::string hoppingPattern;
    
    // 中心频率
    double centerFrequency;
    
    // 采样率
    double sampleRate;
    
    // 发射功率
    double transmitPower;
    
    // 初始化状态
    bool initialized;
};

} // namespace physical
} // namespace link16