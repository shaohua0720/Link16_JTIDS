#pragma once
#include <vector>
#include <complex>
#include <string>

namespace link16 {
namespace physical {
namespace signal_processing {

// FIR滤波器类
class FIRFilter {
public:
    // 滤波器类型
    enum class FilterType {
        LOWPASS,    // 低通滤波器
        HIGHPASS,   // 高通滤波器
        BANDPASS,   // 带通滤波器
        BANDSTOP    // 带阻滤波器
    };
    
    // 窗函数类型
    enum class WindowType {
        RECTANGULAR,    // 矩形窗
        HAMMING,        // 汉明窗
        HANNING,        // 汉宁窗
        BLACKMAN,       // 布莱克曼窗
        KAISER          // 凯撒窗
    };
    
    // 构造函数
    FIRFilter();
    
    // 析构函数
    ~FIRFilter();
    
    // 初始化滤波器
    bool initialize(FilterType type, int numTaps, double cutoffFreq, double samplingRate);
    
    // 初始化带通/带阻滤波器
    bool initialize(FilterType type, int numTaps, double lowCutoffFreq, double highCutoffFreq, double samplingRate);
    
    // 设置窗函数
    void setWindow(WindowType windowType, double param = 0.0);
    
    // 设置滤波器系数
    void setCoefficients(const std::vector<double>& coeffs);
    
    // 获取滤波器系数
    std::vector<double> getCoefficients() const;
    
    // 滤波实数信号
    std::vector<double> filter(const std::vector<double>& input);
    
    // 滤波复数信号
    std::vector<std::complex<double>> filter(const std::vector<std::complex<double>>& input);
    
    // 获取滤波器类型
    FilterType getFilterType() const;
    
    // 获取滤波器阶数
    int getNumTaps() const;
    
    // 获取截止频率
    double getCutoffFrequency() const;
    
    // 获取低截止频率(带通/带阻滤波器)
    double getLowCutoffFrequency() const;
    
    // 获取高截止频率(带通/带阻滤波器)
    double getHighCutoffFrequency() const;
    
    // 获取采样率
    double getSamplingRate() const;
    
    // 获取窗函数类型
    WindowType getWindowType() const;
    
    // 获取频率响应
    std::vector<std::complex<double>> getFrequencyResponse(int numPoints = 1024) const;
    
    // 重置滤波器状态
    void reset();

private:
    // 滤波器类型
    FilterType filterType;
    
    // 窗函数类型
    WindowType windowType;
    
    // 滤波器系数
    std::vector<double> coefficients;
    
    // 滤波器状态(用于实数信号)
    std::vector<double> stateReal;
    
    // 滤波器状态(用于复数信号)
    std::vector<std::complex<double>> stateComplex;
    
    // 滤波器阶数
    int numTaps;
    
    // 截止频率
    double cutoffFreq;
    
    // 低截止频率(带通/带阻滤波器)
    double lowCutoffFreq;
    
    // 高截止频率(带通/带阻滤波器)
    double highCutoffFreq;
    
    // 采样率
    double samplingRate;
    
    // 窗函数参数
    double windowParam;
    
    // 设计低通滤波器
    void designLowpass(int numTaps, double cutoffFreq, double samplingRate);
    
    // 设计高通滤波器
    void designHighpass(int numTaps, double cutoffFreq, double samplingRate);
    
    // 设计带通滤波器
    void designBandpass(int numTaps, double lowCutoffFreq, double highCutoffFreq, double samplingRate);
    
    // 设计带阻滤波器
    void designBandstop(int numTaps, double lowCutoffFreq, double highCutoffFreq, double samplingRate);
    
    // 应用窗函数
    void applyWindow();
    
    // 计算矩形窗
    std::vector<double> rectangularWindow(int length) const;
    
    // 计算汉明窗
    std::vector<double> hammingWindow(int length) const;
    
    // 计算汉宁窗
    std::vector<double> hanningWindow(int length) const;
    
    // 计算布莱克曼窗
    std::vector<double> blackmanWindow(int length) const;
    
    // 计算凯撒窗
    std::vector<double> kaiserWindow(int length, double beta) const;
};

} // namespace signal_processing
} // namespace physical
} // namespace link16
