#pragma once
#include <vector>
#include <complex>
#include <string>
#include <cstdint>

namespace link16 {
namespace physical {
namespace synchronization {

class FrameSynchronizer {
public:
    // 构造函数
    FrameSynchronizer();
    
    // 析构函数
    ~FrameSynchronizer();
    
    // 初始化帧同步器
    bool initialize();
    
    // 设置同步序列
    void setSyncSequence(const std::string& sequence);
    
    // 获取同步序列
    std::string getSyncSequence() const;
    
    // 生成同步序列
    std::vector<std::complex<double>> generateSyncSignal() const;
    
    // 检测同步序列
    bool detectSync(const std::vector<std::complex<double>>& signal, size_t& syncPosition);
    
    // 计算相关性
    double calculateCorrelation(const std::vector<std::complex<double>>& signal, size_t position) const;
    
    // 设置相关阈值
    void setCorrelationThreshold(double threshold);
    
    // 获取相关阈值
    double getCorrelationThreshold() const;

private:
    // 同步序列
    std::string syncSequence;
    
    // 相关阈值
    double correlationThreshold;
    
    // 同步信号
    std::vector<std::complex<double>> syncSignal;
    
    // 生成同步信号
    void generateSyncSignal();
};

} // namespace synchronization
} // namespace physical
} // namespace link16
