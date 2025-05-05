#pragma once
#include <string>
#include <vector>
#include <chrono>

namespace link16 {
namespace simulation {
namespace metrics {

// 吞吐量计算器
class Throughput {
public:
    // 构造函数
    Throughput();
    
    // 析构函数
    ~Throughput();
    
    // 开始计时
    void start();
    
    // 停止计时
    void stop();
    
    // 重置计时器
    void reset();
    
    // 添加传输的比特数
    void addBits(size_t bits);
    
    // 添加传输的字节数
    void addBytes(size_t bytes);
    
    // 添加传输的数据包数
    void addPackets(size_t packets);
    
    // 计算比特吞吐量(bps)
    double calculateBitThroughput() const;
    
    // 计算字节吞吐量(Bps)
    double calculateByteThroughput() const;
    
    // 计算数据包吞吐量(pps)
    double calculatePacketThroughput() const;
    
    // 获取传输的总比特数
    size_t getTotalBits() const;
    
    // 获取传输的总字节数
    size_t getTotalBytes() const;
    
    // 获取传输的总数据包数
    size_t getTotalPackets() const;
    
    // 获取传输时间(秒)
    double getElapsedTime() const;
    
    // 获取格式化的吞吐量字符串
    std::string getFormattedThroughput() const;

private:
    // 开始时间
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    
    // 结束时间
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    
    // 是否正在计时
    bool running;
    
    // 传输的总比特数
    size_t totalBits;
    
    // 传输的总数据包数
    size_t totalPackets;
};

} // namespace metrics
} // namespace simulation
} // namespace link16
