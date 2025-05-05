#include "FIRFilter.h"
#include "core/utils/logger.h"
#include <cmath>
#include <algorithm>
#include <numeric>

namespace link16 {
namespace physical {
namespace signal_processing {

// 构造函数
FIRFilter::FIRFilter() 
    : filterType(FilterType::LOWPASS), windowType(WindowType::HAMMING),
      numTaps(0), cutoffFreq(0.0), lowCutoffFreq(0.0), highCutoffFreq(0.0),
      samplingRate(0.0), windowParam(0.0) {
}

// 析构函数
FIRFilter::~FIRFilter() {
}

// 初始化滤波器
bool FIRFilter::initialize(FilterType type, int numTaps, double cutoffFreq, double samplingRate) {
    if (numTaps <= 0 || cutoffFreq <= 0.0 || samplingRate <= 0.0) {
        LOG_ERROR("FIR滤波器参数无效");
        return false;
    }
    
    if (cutoffFreq >= samplingRate / 2.0) {
        LOG_ERROR("截止频率必须小于采样率的一半");
        return false;
    }
    
    this->filterType = type;
    this->numTaps = numTaps;
    this->cutoffFreq = cutoffFreq;
    this->samplingRate = samplingRate;
    
    // 设计滤波器
    switch (type) {
        case FilterType::LOWPASS:
            designLowpass(numTaps, cutoffFreq, samplingRate);
            break;
        case FilterType::HIGHPASS:
            designHighpass(numTaps, cutoffFreq, samplingRate);
            break;
        default:
            LOG_ERROR("不支持的滤波器类型");
            return false;
    }
    
    // 应用窗函数
    applyWindow();
    
    // 初始化状态
    stateReal.resize(numTaps - 1, 0.0);
    stateComplex.resize(numTaps - 1, std::complex<double>(0.0, 0.0));
    
    LOG_INFO("FIR滤波器初始化成功");
    return true;
}

// 初始化带通/带阻滤波器
bool FIRFilter::initialize(FilterType type, int numTaps, double lowCutoffFreq, double highCutoffFreq, double samplingRate) {
    if (numTaps <= 0 || lowCutoffFreq <= 0.0 || highCutoffFreq <= 0.0 || samplingRate <= 0.0) {
        LOG_ERROR("FIR滤波器参数无效");
        return false;
    }
    
    if (lowCutoffFreq >= highCutoffFreq) {
        LOG_ERROR("低截止频率必须小于高截止频率");
        return false;
    }
    
    if (highCutoffFreq >= samplingRate / 2.0) {
        LOG_ERROR("高截止频率必须小于采样率的一半");
        return false;
    }
    
    this->filterType = type;
    this->numTaps = numTaps;
    this->lowCutoffFreq = lowCutoffFreq;
    this->highCutoffFreq = highCutoffFreq;
    this->samplingRate = samplingRate;
    
    // 设计滤波器
    switch (type) {
        case FilterType::BANDPASS:
            designBandpass(numTaps, lowCutoffFreq, highCutoffFreq, samplingRate);
            break;
        case FilterType::BANDSTOP:
            designBandstop(numTaps, lowCutoffFreq, highCutoffFreq, samplingRate);
            break;
        default:
            LOG_ERROR("不支持的滤波器类型");
            return false;
    }
    
    // 应用窗函数
    applyWindow();
    
    // 初始化状态
    stateReal.resize(numTaps - 1, 0.0);
    stateComplex.resize(numTaps - 1, std::complex<double>(0.0, 0.0));
    
    LOG_INFO("FIR滤波器初始化成功");
    return true;
}

// 设置窗函数
void FIRFilter::setWindow(WindowType windowType, double param) {
    this->windowType = windowType;
    this->windowParam = param;
    
    // 重新应用窗函数
    applyWindow();
}

// 设置滤波器系数
void FIRFilter::setCoefficients(const std::vector<double>& coeffs) {
    coefficients = coeffs;
    numTaps = static_cast<int>(coeffs.size());
    
    // 重置状态
    stateReal.resize(numTaps - 1, 0.0);
    stateComplex.resize(numTaps - 1, std::complex<double>(0.0, 0.0));
}

// 获取滤波器系数
std::vector<double> FIRFilter::getCoefficients() const {
    return coefficients;
}

// 滤波实数信号
std::vector<double> FIRFilter::filter(const std::vector<double>& input) {
    if (coefficients.empty()) {
        LOG_ERROR("滤波器系数为空");
        return input;
    }
    
    std::vector<double> output(input.size());
    
    for (size_t i = 0; i < input.size(); ++i) {
        // 移动状态
        for (int j = numTaps - 2; j > 0; --j) {
            stateReal[j] = stateReal[j - 1];
        }
        stateReal[0] = input[i];
        
        // 计算输出
        double sum = 0.0;
        for (int j = 0; j < numTaps - 1; ++j) {
            sum += coefficients[j] * stateReal[j];
        }
        sum += coefficients[numTaps - 1] * input[i];
        
        output[i] = sum;
    }
    
    return output;
}

// 滤波复数信号
std::vector<std::complex<double>> FIRFilter::filter(const std::vector<std::complex<double>>& input) {
    if (coefficients.empty()) {
        LOG_ERROR("滤波器系数为空");
        return input;
    }
    
    std::vector<std::complex<double>> output(input.size());
    
    for (size_t i = 0; i < input.size(); ++i) {
        // 移动状态
        for (int j = numTaps - 2; j > 0; --j) {
            stateComplex[j] = stateComplex[j - 1];
        }
        stateComplex[0] = input[i];
        
        // 计算输出
        std::complex<double> sum(0.0, 0.0);
        for (int j = 0; j < numTaps - 1; ++j) {
            sum += coefficients[j] * stateComplex[j];
        }
        sum += coefficients[numTaps - 1] * input[i];
        
        output[i] = sum;
    }
    
    return output;
}

// 获取滤波器类型
FIRFilter::FilterType FIRFilter::getFilterType() const {
    return filterType;
}

// 获取滤波器阶数
int FIRFilter::getNumTaps() const {
    return numTaps;
}

// 获取截止频率
double FIRFilter::getCutoffFrequency() const {
    return cutoffFreq;
}

// 获取低截止频率(带通/带阻滤波器)
double FIRFilter::getLowCutoffFrequency() const {
    return lowCutoffFreq;
}

// 获取高截止频率(带通/带阻滤波器)
double FIRFilter::getHighCutoffFrequency() const {
    return highCutoffFreq;
}

// 获取采样率
double FIRFilter::getSamplingRate() const {
    return samplingRate;
}

// 获取窗函数类型
FIRFilter::WindowType FIRFilter::getWindowType() const {
    return windowType;
}

// 获取频率响应
std::vector<std::complex<double>> FIRFilter::getFrequencyResponse(int numPoints) const {
    std::vector<std::complex<double>> response(numPoints);
    
    for (int i = 0; i < numPoints; ++i) {
        double freq = static_cast<double>(i) / numPoints * samplingRate / 2.0;
        std::complex<double> sum(0.0, 0.0);
        
        for (int j = 0; j < numTaps; ++j) {
            double phase = -2.0 * M_PI * freq * j / samplingRate;
            sum += coefficients[j] * std::complex<double>(std::cos(phase), std::sin(phase));
        }
        
        response[i] = sum;
    }
    
    return response;
}

// 重置滤波器状态
void FIRFilter::reset() {
    std::fill(stateReal.begin(), stateReal.end(), 0.0);
    std::fill(stateComplex.begin(), stateComplex.end(), std::complex<double>(0.0, 0.0));
}

// 设计低通滤波器
void FIRFilter::designLowpass(int numTaps, double cutoffFreq, double samplingRate) {
    // 归一化截止频率
    double omega = 2.0 * M_PI * cutoffFreq / samplingRate;
    
    // 分配系数空间
    coefficients.resize(numTaps);
    
    // 计算滤波器系数
    int halfTaps = numTaps / 2;
    for (int i = 0; i < numTaps; ++i) {
        int n = i - halfTaps;
        if (n == 0) {
            // 中心点
            coefficients[i] = omega / M_PI;
        } else {
            // 其他点
            coefficients[i] = std::sin(omega * n) / (M_PI * n);
        }
    }
}

// 设计高通滤波器
void FIRFilter::designHighpass(int numTaps, double cutoffFreq, double samplingRate) {
    // 先设计低通滤波器
    designLowpass(numTaps, cutoffFreq, samplingRate);
    
    // 将低通转换为高通
    int halfTaps = numTaps / 2;
    for (int i = 0; i < numTaps; ++i) {
        int n = i - halfTaps;
        if (n != 0) {
            coefficients[i] = -coefficients[i];
        } else {
            coefficients[i] = 1.0 - coefficients[i];
        }
    }
}

// 设计带通滤波器
void FIRFilter::designBandpass(int numTaps, double lowCutoffFreq, double highCutoffFreq, double samplingRate) {
    // 归一化截止频率
    double omegaLow = 2.0 * M_PI * lowCutoffFreq / samplingRate;
    double omegaHigh = 2.0 * M_PI * highCutoffFreq / samplingRate;
    
    // 分配系数空间
    coefficients.resize(numTaps);
    
    // 计算滤波器系数
    int halfTaps = numTaps / 2;
    for (int i = 0; i < numTaps; ++i) {
        int n = i - halfTaps;
        if (n == 0) {
            // 中心点
            coefficients[i] = (omegaHigh - omegaLow) / M_PI;
        } else {
            // 其他点
            coefficients[i] = (std::sin(omegaHigh * n) - std::sin(omegaLow * n)) / (M_PI * n);
        }
    }
}

// 设计带阻滤波器
void FIRFilter::designBandstop(int numTaps, double lowCutoffFreq, double highCutoffFreq, double samplingRate) {
    // 先设计带通滤波器
    designBandpass(numTaps, lowCutoffFreq, highCutoffFreq, samplingRate);
    
    // 将带通转换为带阻
    int halfTaps = numTaps / 2;
    for (int i = 0; i < numTaps; ++i) {
        int n = i - halfTaps;
        if (n != 0) {
            coefficients[i] = -coefficients[i];
        } else {
            coefficients[i] = 1.0 - coefficients[i];
        }
    }
}

// 应用窗函数
void FIRFilter::applyWindow() {
    std::vector<double> window;
    
    // 根据窗函数类型计算窗函数
    switch (windowType) {
        case WindowType::RECTANGULAR:
            window = rectangularWindow(numTaps);
            break;
        case WindowType::HAMMING:
            window = hammingWindow(numTaps);
            break;
        case WindowType::HANNING:
            window = hanningWindow(numTaps);
            break;
        case WindowType::BLACKMAN:
            window = blackmanWindow(numTaps);
            break;
        case WindowType::KAISER:
            window = kaiserWindow(numTaps, windowParam);
            break;
        default:
            LOG_ERROR("不支持的窗函数类型");
            return;
    }
    
    // 应用窗函数
    for (int i = 0; i < numTaps; ++i) {
        coefficients[i] *= window[i];
    }
    
    // 归一化系数
    double sum = std::accumulate(coefficients.begin(), coefficients.end(), 0.0);
    if (sum != 0.0) {
        for (int i = 0; i < numTaps; ++i) {
            coefficients[i] /= sum;
        }
    }
}

// 计算矩形窗
std::vector<double> FIRFilter::rectangularWindow(int length) const {
    return std::vector<double>(length, 1.0);
}

// 计算汉明窗
std::vector<double> FIRFilter::hammingWindow(int length) const {
    std::vector<double> window(length);
    for (int i = 0; i < length; ++i) {
        window[i] = 0.54 - 0.46 * std::cos(2.0 * M_PI * i / (length - 1));
    }
    return window;
}

// 计算汉宁窗
std::vector<double> FIRFilter::hanningWindow(int length) const {
    std::vector<double> window(length);
    for (int i = 0; i < length; ++i) {
        window[i] = 0.5 * (1.0 - std::cos(2.0 * M_PI * i / (length - 1)));
    }
    return window;
}

// 计算布莱克曼窗
std::vector<double> FIRFilter::blackmanWindow(int length) const {
    std::vector<double> window(length);
    for (int i = 0; i < length; ++i) {
        window[i] = 0.42 - 0.5 * std::cos(2.0 * M_PI * i / (length - 1)) + 0.08 * std::cos(4.0 * M_PI * i / (length - 1));
    }
    return window;
}

// 计算凯撒窗
std::vector<double> FIRFilter::kaiserWindow(int length, double beta) const {
    std::vector<double> window(length);
    
    // 贝塞尔函数I0的近似计算
    auto bessel_i0 = [](double x) {
        double sum = 1.0;
        double term = 1.0;
        double x2 = x * x;
        
        for (int i = 1; i <= 20; ++i) {
            term *= x2 / (4.0 * i * i);
            sum += term;
            if (term < 1e-10 * sum) {
                break;
            }
        }
        
        return sum;
    };
    
    double denom = bessel_i0(beta);
    for (int i = 0; i < length; ++i) {
        double x = 2.0 * i / (length - 1) - 1.0;
        window[i] = bessel_i0(beta * std::sqrt(1.0 - x * x)) / denom;
    }
    
    return window;
}

} // namespace signal_processing
} // namespace physical
} // namespace link16
