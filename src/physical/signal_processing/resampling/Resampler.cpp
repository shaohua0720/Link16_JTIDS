#include "Resampler.h"
#include "core/utils/logger.h"
#include <cmath>
#include <algorithm>

namespace link16 {
namespace physical {
namespace signal_processing {

// 构造函数
Resampler::Resampler() 
    : inputRate(0.0), outputRate(0.0), ratio(1.0), method(InterpolationMethod::SINC),
      numTaps(64), cutoffFactor(0.5), position(0.0) {
}

// 析构函数
Resampler::~Resampler() {
}

// 初始化
bool Resampler::initialize(double inputRate, double outputRate, InterpolationMethod method) {
    if (inputRate <= 0.0 || outputRate <= 0.0) {
        LOG_ERROR("采样率必须为正数");
        return false;
    }
    
    this->inputRate = inputRate;
    this->outputRate = outputRate;
    this->ratio = outputRate / inputRate;
    this->method = method;
    
    // 设计滤波器
    designFilter();
    
    // 初始化缓存
    int bufferSize = numTaps * 2;
    bufferReal.resize(bufferSize, 0.0);
    bufferComplex.resize(bufferSize, std::complex<double>(0.0, 0.0));
    position = 0.0;
    
    LOG_INFO("重采样器初始化成功，输入采样率: " + std::to_string(inputRate) + 
             " Hz，输出采样率: " + std::to_string(outputRate) + " Hz，比率: " + 
             std::to_string(ratio));
    
    return true;
}

// 设置插值方法
void Resampler::setInterpolationMethod(InterpolationMethod method) {
    this->method = method;
}

// 设置滤波器参数
void Resampler::setFilterParams(int numTaps, double cutoffFactor) {
    if (numTaps <= 0 || cutoffFactor <= 0.0 || cutoffFactor > 1.0) {
        LOG_ERROR("滤波器参数无效");
        return;
    }
    
    this->numTaps = numTaps;
    this->cutoffFactor = cutoffFactor;
    
    // 重新设计滤波器
    designFilter();
    
    // 重新初始化缓存
    int bufferSize = numTaps * 2;
    bufferReal.resize(bufferSize, 0.0);
    bufferComplex.resize(bufferSize, std::complex<double>(0.0, 0.0));
    position = 0.0;
}

// 重采样实数信号
std::vector<double> Resampler::resample(const std::vector<double>& input) {
    if (input.empty()) {
        return input;
    }
    
    if (std::abs(ratio - 1.0) < 1e-6) {
        // 采样率相同，直接返回输入
        return input;
    }
    
    // 估计输出大小
    size_t outputSize = static_cast<size_t>(std::ceil(input.size() * ratio));
    std::vector<double> output(outputSize);
    
    // 重置位置
    position = 0.0;
    
    // 清空缓存
    std::fill(bufferReal.begin(), bufferReal.end(), 0.0);
    
    // 将输入数据复制到缓存
    size_t bufferSize = bufferReal.size();
    for (size_t i = 0; i < std::min(input.size(), bufferSize); ++i) {
        bufferReal[i] = input[i];
    }
    
    // 重采样
    size_t outputIndex = 0;
    while (position < input.size() && outputIndex < outputSize) {
        // 插值
        double value = 0.0;
        switch (method) {
            case InterpolationMethod::NEAREST:
                value = interpolateNearest(bufferReal, position);
                break;
            case InterpolationMethod::LINEAR:
                value = interpolateLinear(bufferReal, position);
                break;
            case InterpolationMethod::CUBIC:
                value = interpolateCubic(bufferReal, position);
                break;
            case InterpolationMethod::SINC:
                value = interpolateSinc(bufferReal, position);
                break;
        }
        
        output[outputIndex++] = value;
        
        // 更新位置
        position += 1.0 / ratio;
        
        // 如果位置超过了缓存的一半，移动缓存
        if (position >= bufferSize / 2) {
            // 移动缓存
            size_t shift = static_cast<size_t>(std::floor(position));
            for (size_t i = 0; i < bufferSize - shift; ++i) {
                bufferReal[i] = bufferReal[i + shift];
            }
            
            // 填充新数据
            for (size_t i = bufferSize - shift; i < bufferSize; ++i) {
                size_t inputIndex = i + input.size() - bufferSize;
                if (inputIndex < input.size()) {
                    bufferReal[i] = input[inputIndex];
                } else {
                    bufferReal[i] = 0.0;
                }
            }
            
            // 更新位置
            position -= shift;
        }
    }
    
    // 调整输出大小
    output.resize(outputIndex);
    
    return output;
}

// 重采样复数信号
std::vector<std::complex<double>> Resampler::resample(const std::vector<std::complex<double>>& input) {
    if (input.empty()) {
        return input;
    }
    
    if (std::abs(ratio - 1.0) < 1e-6) {
        // 采样率相同，直接返回输入
        return input;
    }
    
    // 估计输出大小
    size_t outputSize = static_cast<size_t>(std::ceil(input.size() * ratio));
    std::vector<std::complex<double>> output(outputSize);
    
    // 重置位置
    position = 0.0;
    
    // 清空缓存
    std::fill(bufferComplex.begin(), bufferComplex.end(), std::complex<double>(0.0, 0.0));
    
    // 将输入数据复制到缓存
    size_t bufferSize = bufferComplex.size();
    for (size_t i = 0; i < std::min(input.size(), bufferSize); ++i) {
        bufferComplex[i] = input[i];
    }
    
    // 重采样
    size_t outputIndex = 0;
    while (position < input.size() && outputIndex < outputSize) {
        // 插值
        std::complex<double> value(0.0, 0.0);
        switch (method) {
            case InterpolationMethod::NEAREST:
                value = interpolateNearest(bufferComplex, position);
                break;
            case InterpolationMethod::LINEAR:
                value = interpolateLinear(bufferComplex, position);
                break;
            case InterpolationMethod::CUBIC:
                value = interpolateCubic(bufferComplex, position);
                break;
            case InterpolationMethod::SINC:
                value = interpolateSinc(bufferComplex, position);
                break;
        }
        
        output[outputIndex++] = value;
        
        // 更新位置
        position += 1.0 / ratio;
        
        // 如果位置超过了缓存的一半，移动缓存
        if (position >= bufferSize / 2) {
            // 移动缓存
            size_t shift = static_cast<size_t>(std::floor(position));
            for (size_t i = 0; i < bufferSize - shift; ++i) {
                bufferComplex[i] = bufferComplex[i + shift];
            }
            
            // 填充新数据
            for (size_t i = bufferSize - shift; i < bufferSize; ++i) {
                size_t inputIndex = i + input.size() - bufferSize;
                if (inputIndex < input.size()) {
                    bufferComplex[i] = input[inputIndex];
                } else {
                    bufferComplex[i] = std::complex<double>(0.0, 0.0);
                }
            }
            
            // 更新位置
            position -= shift;
        }
    }
    
    // 调整输出大小
    output.resize(outputIndex);
    
    return output;
}

// 获取输入采样率
double Resampler::getInputRate() const {
    return inputRate;
}

// 获取输出采样率
double Resampler::getOutputRate() const {
    return outputRate;
}

// 获取采样率比
double Resampler::getRatio() const {
    return ratio;
}

// 获取插值方法
Resampler::InterpolationMethod Resampler::getInterpolationMethod() const {
    return method;
}

// 重置重采样器状态
void Resampler::reset() {
    std::fill(bufferReal.begin(), bufferReal.end(), 0.0);
    std::fill(bufferComplex.begin(), bufferComplex.end(), std::complex<double>(0.0, 0.0));
    position = 0.0;
}

// 设计滤波器
void Resampler::designFilter() {
    // 计算截止频率
    double cutoffFreq = std::min(inputRate, outputRate) / 2.0 * cutoffFactor;
    
    // 初始化滤波器
    filter.initialize(FIRFilter::FilterType::LOWPASS, numTaps, cutoffFreq, std::max(inputRate, outputRate));
    
    // 设置窗函数
    filter.setWindow(FIRFilter::WindowType::HAMMING);
}

// 最近邻插值(实数)
double Resampler::interpolateNearest(const std::vector<double>& buffer, double position) const {
    int index = static_cast<int>(std::round(position));
    if (index < 0 || index >= static_cast<int>(buffer.size())) {
        return 0.0;
    }
    return buffer[index];
}

// 线性插值(实数)
double Resampler::interpolateLinear(const std::vector<double>& buffer, double position) const {
    int index = static_cast<int>(std::floor(position));
    double frac = position - index;
    
    if (index < 0 || index + 1 >= static_cast<int>(buffer.size())) {
        return 0.0;
    }
    
    return buffer[index] * (1.0 - frac) + buffer[index + 1] * frac;
}

// 三次插值(实数)
double Resampler::interpolateCubic(const std::vector<double>& buffer, double position) const {
    int index = static_cast<int>(std::floor(position));
    double frac = position - index;
    
    if (index < 1 || index + 2 >= static_cast<int>(buffer.size())) {
        return interpolateLinear(buffer, position);
    }
    
    double p0 = buffer[index - 1];
    double p1 = buffer[index];
    double p2 = buffer[index + 1];
    double p3 = buffer[index + 2];
    
    double a = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
    double b = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
    double c = -0.5 * p0 + 0.5 * p2;
    double d = p1;
    
    return ((a * frac + b) * frac + c) * frac + d;
}

// Sinc插值(实数)
double Resampler::interpolateSinc(const std::vector<double>& buffer, double position) const {
    int index = static_cast<int>(std::floor(position));
    double frac = position - index;
    
    if (index < 0 || index >= static_cast<int>(buffer.size())) {
        return 0.0;
    }
    
    // 使用Lanczos窗口的Sinc插值
    int windowSize = 4;
    double sum = 0.0;
    double weightSum = 0.0;
    
    for (int i = -windowSize + 1; i <= windowSize; ++i) {
        int idx = index + i;
        if (idx < 0 || idx >= static_cast<int>(buffer.size())) {
            continue;
        }
        
        double x = frac - i;
        double weight = 0.0;
        
        if (x != 0.0) {
            weight = std::sin(M_PI * x) / (M_PI * x);
            if (std::abs(x) < windowSize) {
                weight *= std::sin(M_PI * x / windowSize) / (M_PI * x / windowSize);
            } else {
                weight = 0.0;
            }
        } else {
            weight = 1.0;
        }
        
        sum += buffer[idx] * weight;
        weightSum += weight;
    }
    
    if (weightSum != 0.0) {
        return sum / weightSum;
    } else {
        return 0.0;
    }
}

// 最近邻插值(复数)
std::complex<double> Resampler::interpolateNearest(const std::vector<std::complex<double>>& buffer, double position) const {
    int index = static_cast<int>(std::round(position));
    if (index < 0 || index >= static_cast<int>(buffer.size())) {
        return std::complex<double>(0.0, 0.0);
    }
    return buffer[index];
}

// 线性插值(复数)
std::complex<double> Resampler::interpolateLinear(const std::vector<std::complex<double>>& buffer, double position) const {
    int index = static_cast<int>(std::floor(position));
    double frac = position - index;
    
    if (index < 0 || index + 1 >= static_cast<int>(buffer.size())) {
        return std::complex<double>(0.0, 0.0);
    }
    
    return buffer[index] * (1.0 - frac) + buffer[index + 1] * frac;
}

// 三次插值(复数)
std::complex<double> Resampler::interpolateCubic(const std::vector<std::complex<double>>& buffer, double position) const {
    int index = static_cast<int>(std::floor(position));
    double frac = position - index;
    
    if (index < 1 || index + 2 >= static_cast<int>(buffer.size())) {
        return interpolateLinear(buffer, position);
    }
    
    std::complex<double> p0 = buffer[index - 1];
    std::complex<double> p1 = buffer[index];
    std::complex<double> p2 = buffer[index + 1];
    std::complex<double> p3 = buffer[index + 2];
    
    std::complex<double> a = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
    std::complex<double> b = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
    std::complex<double> c = -0.5 * p0 + 0.5 * p2;
    std::complex<double> d = p1;
    
    return ((a * frac + b) * frac + c) * frac + d;
}

// Sinc插值(复数)
std::complex<double> Resampler::interpolateSinc(const std::vector<std::complex<double>>& buffer, double position) const {
    int index = static_cast<int>(std::floor(position));
    double frac = position - index;
    
    if (index < 0 || index >= static_cast<int>(buffer.size())) {
        return std::complex<double>(0.0, 0.0);
    }
    
    // 使用Lanczos窗口的Sinc插值
    int windowSize = 4;
    std::complex<double> sum(0.0, 0.0);
    double weightSum = 0.0;
    
    for (int i = -windowSize + 1; i <= windowSize; ++i) {
        int idx = index + i;
        if (idx < 0 || idx >= static_cast<int>(buffer.size())) {
            continue;
        }
        
        double x = frac - i;
        double weight = 0.0;
        
        if (x != 0.0) {
            weight = std::sin(M_PI * x) / (M_PI * x);
            if (std::abs(x) < windowSize) {
                weight *= std::sin(M_PI * x / windowSize) / (M_PI * x / windowSize);
            } else {
                weight = 0.0;
            }
        } else {
            weight = 1.0;
        }
        
        sum += buffer[idx] * weight;
        weightSum += weight;
    }
    
    if (weightSum != 0.0) {
        return sum / weightSum;
    } else {
        return std::complex<double>(0.0, 0.0);
    }
}

} // namespace signal_processing
} // namespace physical
} // namespace link16
