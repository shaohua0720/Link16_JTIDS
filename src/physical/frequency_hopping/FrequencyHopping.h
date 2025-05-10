#pragma once

#include <string>
#include <vector>
#include <complex>
#include <memory>

namespace link16 {
namespace physical {
namespace frequency_hopping {

/**
 * @brief 跳频器类
 */
class FrequencyHopping {
public:
    /**
     * @brief 构造函数
     * @param pattern 跳频模式，可以是"fixed"、"random"、"pattern1"、"pattern2"等
     */
    FrequencyHopping(const std::string& pattern = "fixed");
    
    /**
     * @brief 析构函数
     */
    ~FrequencyHopping();
    
    /**
     * @brief 跳频函数
     * @param inputSignal 输入信号
     * @return 跳频后的信号
     */
    std::vector<std::complex<double>> hop(const std::vector<std::complex<double>>& inputSignal);
    
    /**
     * @brief 解跳频函数
     * @param hoppedSignal 跳频后的信号
     * @return 解跳频后的信号
     */
    std::vector<std::complex<double>> deHop(const std::vector<std::complex<double>>& hoppedSignal);
    
    /**
     * @brief 设置跳频模式
     * @param pattern 跳频模式，可以是"fixed"、"random"、"pattern1"、"pattern2"等
     */
    void setPattern(const std::string& pattern);
    
    /**
     * @brief 获取跳频模式
     * @return 跳频模式
     */
    const std::string& getPattern() const;
    
    /**
     * @brief 设置跳频序列
     * @param sequence 跳频序列
     */
    void setSequence(const std::vector<double>& sequence);
    
    /**
     * @brief 获取跳频序列
     * @return 跳频序列
     */
    const std::vector<double>& getSequence() const;
    
    /**
     * @brief 设置跳频速率
     * @param rate 跳频速率，单位为跳/秒
     */
    void setRate(double rate);
    
    /**
     * @brief 获取跳频速率
     * @return 跳频速率
     */
    double getRate() const;
    
    /**
     * @brief 设置中心频率
     * @param frequency 中心频率
     */
    void setCenterFrequency(double frequency);
    
    /**
     * @brief 获取中心频率
     * @return 中心频率
     */
    double getCenterFrequency() const;
    
    /**
     * @brief 设置带宽
     * @param bandwidth 带宽
     */
    void setBandwidth(double bandwidth);
    
    /**
     * @brief 获取带宽
     * @return 带宽
     */
    double getBandwidth() const;
    
    /**
     * @brief 生成随机跳频序列
     * @param length 序列长度
     * @return 随机跳频序列
     */
    std::vector<double> generateRandomSequence(int length);
    
    /**
     * @brief 加载预定义的跳频序列
     * @param patternName 模式名称
     * @return 是否加载成功
     */
    bool loadPredefinedSequence(const std::string& patternName);
    
    /**
     * @brief 保存当前跳频序列
     * @param filename 文件名
     * @return 是否保存成功
     */
    bool saveSequence(const std::string& filename);
    
    /**
     * @brief 从文件加载跳频序列
     * @param filename 文件名
     * @return 是否加载成功
     */
    bool loadSequence(const std::string& filename);

private:
    // 跳频模式
    std::string pattern;
    
    // 跳频序列
    std::vector<double> sequence;
    
    // 跳频速率
    double rate;
    
    // 中心频率
    double centerFrequency;
    
    // 带宽
    double bandwidth;
    
    // 内部实现
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace frequency_hopping
} // namespace physical
} // namespace link16