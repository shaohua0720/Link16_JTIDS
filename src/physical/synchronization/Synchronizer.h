#pragma once

#include <string>
#include <vector>
#include <complex>
#include <memory>

namespace link16 {
namespace physical {
namespace synchronization {

/**
 * @brief 同步器类
 */
class Synchronizer {
public:
    /**
     * @brief 构造函数
     */
    Synchronizer();
    
    /**
     * @brief 析构函数
     */
    ~Synchronizer();
    
    /**
     * @brief 同步函数
     * @param signal 接收到的信号
     * @return 是否同步成功
     */
    bool synchronize(const std::vector<std::complex<double>>& signal);
    
    /**
     * @brief 获取同步状态
     * @return 同步状态
     */
    bool isSynchronized() const;
    
    /**
     * @brief 获取时间偏移
     * @return 时间偏移
     */
    double getTimeOffset() const;
    
    /**
     * @brief 获取频率偏移
     * @return 频率偏移
     */
    double getFrequencyOffset() const;
    
    /**
     * @brief 获取相位偏移
     * @return 相位偏移
     */
    double getPhaseOffset() const;
    
    /**
     * @brief 设置同步阈值
     * @param threshold 同步阈值
     */
    void setThreshold(double threshold);
    
    /**
     * @brief 获取同步阈值
     * @return 同步阈值
     */
    double getThreshold() const;
    
    /**
     * @brief 设置同步序列
     * @param sequence 同步序列
     */
    void setSequence(const std::vector<std::complex<double>>& sequence);
    
    /**
     * @brief 获取同步序列
     * @return 同步序列
     */
    const std::vector<std::complex<double>>& getSequence() const;
    
    /**
     * @brief 生成默认同步序列
     * @return 默认同步序列
     */
    std::vector<std::complex<double>> generateDefaultSequence();
    
    /**
     * @brief 重置同步状态
     */
    void reset();

private:
    // 同步状态
    bool synchronized;
    
    // 时间偏移
    double timeOffset;
    
    // 频率偏移
    double frequencyOffset;
    
    // 相位偏移
    double phaseOffset;
    
    // 同步阈值
    double threshold;
    
    // 同步序列
    std::vector<std::complex<double>> sequence;
    
    // 内部实现
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace synchronization
} // namespace physical
} // namespace link16