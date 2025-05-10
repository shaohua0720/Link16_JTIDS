#pragma once

#include <string>
#include <vector>
#include <complex>
#include <memory>

namespace link16 {
namespace physical {
namespace modulation {

/**
 * @brief DPSK调制器类
 */
class DPSKModulator {
public:
    /**
     * @brief 构造函数
     * @param order 调制阶数，可以是2(DBPSK)、4(DQPSK)、8(D8PSK)
     */
    DPSKModulator(int order = 2);
    
    /**
     * @brief 析构函数
     */
    ~DPSKModulator();
    
    /**
     * @brief 调制函数
     * @param bitStream 比特流
     * @return 调制后的符号
     */
    std::vector<std::complex<double>> modulate(const std::string& bitStream);
    
    /**
     * @brief 解调函数
     * @param symbols 调制后的符号
     * @return 解调后的比特流
     */
    std::string demodulate(const std::vector<std::complex<double>>& symbols);
    
    /**
     * @brief 设置调制阶数
     * @param order 调制阶数，可以是2(DBPSK)、4(DQPSK)、8(D8PSK)
     */
    void setOrder(int order);
    
    /**
     * @brief 获取调制阶数
     * @return 调制阶数
     */
    int getOrder() const;
    
    /**
     * @brief 设置采样率
     * @param sampleRate 采样率
     */
    void setSampleRate(double sampleRate);
    
    /**
     * @brief 获取采样率
     * @return 采样率
     */
    double getSampleRate() const;
    
    /**
     * @brief 设置符号率
     * @param symbolRate 符号率
     */
    void setSymbolRate(double symbolRate);
    
    /**
     * @brief 获取符号率
     * @return 符号率
     */
    double getSymbolRate() const;
    
    /**
     * @brief 设置载波频率
     * @param carrierFrequency 载波频率
     */
    void setCarrierFrequency(double carrierFrequency);
    
    /**
     * @brief 获取载波频率
     * @return 载波频率
     */
    double getCarrierFrequency() const;
    
    /**
     * @brief 设置滤波器类型
     * @param filterType 滤波器类型，可以是"none"、"rrc"(根升余弦)、"gaussian"
     */
    void setFilterType(const std::string& filterType);
    
    /**
     * @brief 获取滤波器类型
     * @return 滤波器类型
     */
    const std::string& getFilterType() const;
    
    /**
     * @brief 设置滤波器参数
     * @param alpha 滤波器参数，对于根升余弦滤波器，这是滚降因子
     */
    void setFilterParameter(double alpha);
    
    /**
     * @brief 获取滤波器参数
     * @return 滤波器参数
     */
    double getFilterParameter() const;

private:
    // 调制阶数
    int order;
    
    // 采样率
    double sampleRate;
    
    // 符号率
    double symbolRate;
    
    // 载波频率
    double carrierFrequency;
    
    // 滤波器类型
    std::string filterType;
    
    // 滤波器参数
    double filterParameter;
    
    // 内部实现
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace modulation
} // namespace physical
} // namespace link16