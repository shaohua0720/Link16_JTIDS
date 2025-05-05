#pragma once
#include <vector>
#include <complex>
#include <string>

namespace link16 {
namespace physical {
namespace modulation {

class BPSKModulator {
public:
    // 构造函数
    BPSKModulator();
    
    // 析构函数
    ~BPSKModulator();
    
    // 初始化调制器
    bool initialize();
    
    // 调制二进制数据
    std::vector<std::complex<double>> modulate(const std::string& bits);
    
    // 解调复数信号
    std::string demodulate(const std::vector<std::complex<double>>& signal);
    
    // 设置采样率
    void setSampleRate(double rate);
    
    // 获取采样率
    double getSampleRate() const;
    
    // 设置符号率
    void setSymbolRate(double rate);
    
    // 获取符号率
    double getSymbolRate() const;
    
    // 设置每符号样本数
    void setSamplesPerSymbol(int samples);
    
    // 获取每符号样本数
    int getSamplesPerSymbol() const;
    
    // 设置滤波器长度
    void setFilterLength(int length);
    
    // 获取滤波器长度
    int getFilterLength() const;

private:
    // 采样率
    double sampleRate;
    
    // 符号率
    double symbolRate;
    
    // 每符号样本数
    int samplesPerSymbol;
    
    // 滤波器长度
    int filterLength;
    
    // 生成脉冲整形滤波器
    std::vector<double> generatePulseShapingFilter() const;
    
    // 应用脉冲整形滤波器
    std::vector<std::complex<double>> applyFilter(const std::vector<std::complex<double>>& signal, const std::vector<double>& filter) const;
};

} // namespace modulation
} // namespace physical
} // namespace link16
