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

class USRPReceiver : public USRPInterface {
public:
    // 构造函数
    USRPReceiver();

    // 析构函数
    virtual ~USRPReceiver();

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

    // 接收器特有方法

    // 接收复数样本
    bool receive(std::vector<std::complex<float>>& samples, size_t numSamples, double timeout = 1.0);

    // 接收实数样本
    bool receive(std::vector<float>& samples, size_t numSamples, double timeout = 1.0);

    // 接收二进制数据
    bool receiveBits(std::string& bits, size_t numBits, double timeout = 1.0);

    // 设置接收通道
    bool setRxChannel(size_t channel);

    // 获取接收通道
    size_t getRxChannel() const;

    // 启动连续接收
    bool startContinuousReceive(std::function<void(const std::vector<std::complex<float>>&)> callback);

    // 停止连续接收
    void stopContinuousReceive();

    // 获取信号强度
    double getSignalStrength() const;

    // 设置自动增益控制
    bool setAGC(bool enable);

    // 获取自动增益控制状态
    bool getAGC() const;

private:
    // 设备句柄(使用void*避免包含UHD头文件)
    void* deviceHandle;

    // 接收流句柄
    void* rxStream;

    // 设备参数
    std::string deviceArgs;
    double rxFrequency;
    double rxSampleRate;
    double rxGain;
    double rxBandwidth;
    std::string rxAntenna;
    size_t rxChannel;

    // 设备状态
    bool initialized;
    bool agcEnabled;
    std::atomic<bool> continuousReceiveRunning;

    // 接收线程
    std::thread receiveThread;

    // 互斥锁
    std::mutex deviceMutex;

    // 条件变量
    std::condition_variable stopCondition;

    // 连续接收回调函数
    std::function<void(const std::vector<std::complex<float>>&)> receiveCallback;

    // 连续接收线程函数
    void continuousReceiveThread();

    // 计算信号强度
    double calculateSignalStrength(const std::vector<std::complex<float>>& samples) const;
};

} // namespace hardware
} // namespace physical
} // namespace link16
