#pragma once
#include <vector>
#include <complex>
#include <string>
#include "../filter/FIRFilter.h"

namespace link16 {
namespace physical {
namespace signal_processing {

// 采样率转换类
class Resampler {
public:
    // 插值方法
    enum class InterpolationMethod {
        NEAREST,    // 最近邻插值
        LINEAR,     // 线性插值
        CUBIC,      // 三次插值
        SINC        // Sinc插值
    };
    
    // 构造函数
    Resampler();
    
    // 析构函数
    ~Resampler();
    
    // 初始化
    bool initialize(double inputRate, double outputRate, InterpolationMethod method = InterpolationMethod::SINC);
    
    // 设置插值方法
    void setInterpolationMethod(InterpolationMethod method);
    
    // 设置滤波器参数
    void setFilterParams(int numTaps, double cutoffFactor = 0.5);
    
    // 重采样实数信号
    std::vector<double> resample(const std::vector<double>& input);
    
    // 重采样复数信号
    std::vector<std::complex<double>> resample(const std::vector<std::complex<double>>& input);
    
    // 获取输入采样率
    double getInputRate() const;
    
    // 获取输出采样率
    double getOutputRate() const;
    
    // 获取采样率比
    double getRatio() const;
    
    // 获取插值方法
    InterpolationMethod getInterpolationMethod() const;
    
    // 重置重采样器状态
    void reset();

private:
    // 输入采样率
    double inputRate;
    
    // 输出采样率
    double outputRate;
    
    // 采样率比
    double ratio;
    
    // 插值方法
    InterpolationMethod method;
    
    // 滤波器
    FIRFilter filter;
    
    // 滤波器阶数
    int numTaps;
    
    // 截止频率因子
    double cutoffFactor;
    
    // 缓存(用于实数信号)
    std::vector<double> bufferReal;
    
    // 缓存(用于复数信号)
    std::vector<std::complex<double>> bufferComplex;
    
    // 缓存位置
    double position;
    
    // 设计滤波器
    void designFilter();
    
    // 最近邻插值(实数)
    double interpolateNearest(const std::vector<double>& buffer, double position) const;
    
    // 线性插值(实数)
    double interpolateLinear(const std::vector<double>& buffer, double position) const;
    
    // 三次插值(实数)
    double interpolateCubic(const std::vector<double>& buffer, double position) const;
    
    // Sinc插值(实数)
    double interpolateSinc(const std::vector<double>& buffer, double position) const;
    
    // 最近邻插值(复数)
    std::complex<double> interpolateNearest(const std::vector<std::complex<double>>& buffer, double position) const;
    
    // 线性插值(复数)
    std::complex<double> interpolateLinear(const std::vector<std::complex<double>>& buffer, double position) const;
    
    // 三次插值(复数)
    std::complex<double> interpolateCubic(const std::vector<std::complex<double>>& buffer, double position) const;
    
    // Sinc插值(复数)
    std::complex<double> interpolateSinc(const std::vector<std::complex<double>>& buffer, double position) const;
};

} // namespace signal_processing
} // namespace physical
} // namespace link16
