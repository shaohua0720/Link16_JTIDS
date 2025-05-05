#include "FrameSynchronizer.h"
#include "core/utils/logger.h"
#include <iostream>
#include <cmath>
#include <numeric>

namespace link16 {
namespace physical {
namespace synchronization {

// 构造函数
FrameSynchronizer::FrameSynchronizer()
    : syncSequence("0101010101010101"), correlationThreshold(0.7) {
}

// 析构函数
FrameSynchronizer::~FrameSynchronizer() {
}

// 初始化帧同步器
bool FrameSynchronizer::initialize() {
    LOG_INFO("初始化帧同步器");
    
    // 设置默认参数
    syncSequence = "0101010101010101";  // 16位同步序列
    correlationThreshold = 0.7;         // 70%相关阈值
    
    // 生成同步信号
    generateSyncSignal();
    
    return true;
}

// 设置同步序列
void FrameSynchronizer::setSyncSequence(const std::string& sequence) {
    syncSequence = sequence;
    LOG_INFO("设置同步序列: " + sequence);
    
    // 重新生成同步信号
    generateSyncSignal();
}

// 获取同步序列
std::string FrameSynchronizer::getSyncSequence() const {
    return syncSequence;
}

// 生成同步序列
std::vector<std::complex<double>> FrameSynchronizer::generateSyncSignal() const {
    return syncSignal;
}

// 检测同步序列
bool FrameSynchronizer::detectSync(const std::vector<std::complex<double>>& signal, size_t& syncPosition) {
    if (signal.size() < syncSignal.size()) {
        LOG_ERROR("信号长度不足，无法检测同步序列");
        return false;
    }
    
    // 寻找最大相关性位置
    double maxCorrelation = 0.0;
    syncPosition = 0;
    
    for (size_t i = 0; i <= signal.size() - syncSignal.size(); ++i) {
        double correlation = calculateCorrelation(signal, i);
        if (correlation > maxCorrelation) {
            maxCorrelation = correlation;
            syncPosition = i;
        }
    }
    
    LOG_INFO("最大相关性: " + std::to_string(maxCorrelation) + ", 位置: " + std::to_string(syncPosition));
    
    // 判断是否超过阈值
    return maxCorrelation >= correlationThreshold;
}

// 计算相关性
double FrameSynchronizer::calculateCorrelation(const std::vector<std::complex<double>>& signal, size_t position) const {
    if (position + syncSignal.size() > signal.size()) {
        return 0.0;
    }
    
    // 计算相关性
    double correlation = 0.0;
    for (size_t i = 0; i < syncSignal.size(); ++i) {
        correlation += std::real(signal[position + i] * std::conj(syncSignal[i]));
    }
    
    // 归一化
    double signalEnergy = 0.0;
    for (size_t i = 0; i < syncSignal.size(); ++i) {
        signalEnergy += std::norm(signal[position + i]);
    }
    
    double syncEnergy = 0.0;
    for (size_t i = 0; i < syncSignal.size(); ++i) {
        syncEnergy += std::norm(syncSignal[i]);
    }
    
    if (signalEnergy > 0.0 && syncEnergy > 0.0) {
        correlation /= std::sqrt(signalEnergy * syncEnergy);
    } else {
        correlation = 0.0;
    }
    
    return correlation;
}

// 设置相关阈值
void FrameSynchronizer::setCorrelationThreshold(double threshold) {
    correlationThreshold = threshold;
    LOG_INFO("设置相关阈值: " + std::to_string(threshold));
}

// 获取相关阈值
double FrameSynchronizer::getCorrelationThreshold() const {
    return correlationThreshold;
}

// 生成同步信号
void FrameSynchronizer::generateSyncSignal() {
    syncSignal.clear();
    syncSignal.reserve(syncSequence.length());
    
    for (char bit : syncSequence) {
        if (bit == '0') {
            syncSignal.push_back(std::complex<double>(-1.0, 0.0));
        } else if (bit == '1') {
            syncSignal.push_back(std::complex<double>(1.0, 0.0));
        } else {
            LOG_WARNING("无效的同步序列位: " + std::string(1, bit));
        }
    }
    
    LOG_INFO("生成同步信号: " + std::to_string(syncSignal.size()) + " 个样本");
}

} // namespace synchronization
} // namespace physical
} // namespace link16
