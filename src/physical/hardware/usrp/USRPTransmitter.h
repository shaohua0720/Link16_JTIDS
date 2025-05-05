#pragma once
#include "USRPInterface.h"
#include <vector>
#include <complex>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace link16 {
namespace physical {
namespace hardware {

class USRPTransmitter : public USRPInterface {
public:
    // 构造函数
    USRPTransmitter();

    // 析构函数
    virtual ~USRPTransmitter();

    // 实现USRPInterface接口
    virtual bool initialize(const std::string& args = "") override;
    virtual void close() override;
    virtual bool setFrequency(double freq) override;
    virtual double getFrequency() const override;
    virtual bool setSampleRate(double rate) override;
    virtual double getSampleRate() const override;
    virtual bool setGain(double gain) override;
    virtual double getGain() const override;
    virtual bool setBandwidth(double bandwidth) override;
    virtual double getBandwidth() const override;
    virtual bool setAntenna(const std::string& antenna) override;
    virtual std::string getAntenna() const override;
    virtual std::string getDeviceInfo() const override;
    virtual bool isInitialized() const override;

    // 发送器特有方法

    // 发送复数样本
    bool transmit(const std::vector<std::complex<float>>& samples);

    // 发送实数样本
    bool transmit(const std::vector<float>& samples);

    // 发送二进制数据
    bool transmitBits(const std::string& bits);

    // 设置发送通道
    bool setTxChannel(size_t channel);

    // 获取发送通道
    size_t getTxChannel() const;

    // 启动连续发送
    bool startContinuousTransmit(const std::vector<std::complex<float>>& samples, size_t repeatCount = 0);

    // 停止连续发送
    void stopContinuousTransmit();

    // 设置发送功率
    bool setTxPower(double power);

    // 获取发送功率
    double getTxPower() const;

private:
    // 设备句柄(使用void*避免包含UHD头文件)
    void* deviceHandle;

    // 发送流句柄
    void* txStream;

    // 设备参数
    std::string deviceArgs;
    double txFrequency;
    double txSampleRate;
    double txGain;
    double txBandwidth;
    std::string txAntenna;
    size_t txChannel;
    double txPower;

    // 设备状态
    bool initialized;
    std::atomic<bool> continuousTransmitRunning;

    // 发送线程
    std::thread transmitThread;

    // 互斥锁
    std::mutex deviceMutex;

    // 条件变量
    std::condition_variable stopCondition;

    // 连续发送数据
    std::vector<std::complex<float>> transmitBuffer;
    size_t repeatCount;

    // 连续发送线程函数
    void continuousTransmitThread();
};

} // namespace hardware
} // namespace physical
} // namespace link16
