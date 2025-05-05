#include "BPSKModulator.h"
#include "core/utils/logger.h"
#include <iostream>
#include <cmath>

namespace link16 {
namespace physical {
namespace modulation {

// 构造函数
BPSKModulator::BPSKModulator()
    : sampleRate(1.0e6), symbolRate(1.0e5), samplesPerSymbol(10), filterLength(32) {
}

// 析构函数
BPSKModulator::~BPSKModulator() {
}

// 初始化调制器
bool BPSKModulator::initialize() {
    LOG_INFO("初始化BPSK调制器");
    
    // 设置默认参数
    sampleRate = 1.0e6;     // 1MHz
    symbolRate = 1.0e5;     // 100kHz
    samplesPerSymbol = 10;  // 每符号10个样本
    filterLength = 32;      // 32点滤波器
    
    return true;
}

// 调制二进制数据
std::vector<std::complex<double>> BPSKModulator::modulate(const std::string& bits) {
    LOG_INFO("BPSK调制: " + std::to_string(bits.length()) + " 位");
    
    // 生成基带信号
    std::vector<std::complex<double>> baseband;
    baseband.reserve(bits.length() * samplesPerSymbol);
    
    for (char bit : bits) {
        double amplitude = (bit == '0') ? -1.0 : 1.0;
        
        // 每个符号生成多个样本
        for (int i = 0; i < samplesPerSymbol; ++i) {
            baseband.push_back(std::complex<double>(amplitude, 0.0));
        }
    }
    
    // 应用脉冲整形滤波器
    std::vector<double> filter = generatePulseShapingFilter();
    std::vector<std::complex<double>> filteredSignal = applyFilter(baseband, filter);
    
    return filteredSignal;
}

// 解调复数信号
std::string BPSKModulator::demodulate(const std::vector<std::complex<double>>& signal) {
    LOG_INFO("BPSK解调: " + std::to_string(signal.size()) + " 个样本");
    
    if (signal.empty()) {
        LOG_ERROR("信号为空，无法解调");
        return "";
    }
    
    // 计算每个符号的样本数
    if (samplesPerSymbol <= 0) {
        LOG_ERROR("每符号样本数无效: " + std::to_string(samplesPerSymbol));
        return "";
    }
    
    // 估计符号数量
    size_t numSymbols = signal.size() / samplesPerSymbol;
    
    // 解调
    std::string bits;
    bits.reserve(numSymbols);
    
    for (size_t i = 0; i < numSymbols; ++i) {
        // 计算符号中心位置
        size_t centerIndex = i * samplesPerSymbol + samplesPerSymbol / 2;
        
        // 确保索引有效
        if (centerIndex >= signal.size()) {
            break;
        }
        
        // 判断符号
        double realPart = std::real(signal[centerIndex]);
        bits.push_back((realPart < 0.0) ? '0' : '1');
    }
    
    return bits;
}

// 设置采样率
void BPSKModulator::setSampleRate(double rate) {
    sampleRate = rate;
    LOG_INFO("设置BPSK采样率: " + std::to_string(rate) + " Hz");
}

// 获取采样率
double BPSKModulator::getSampleRate() const {
    return sampleRate;
}

// 设置符号率
void BPSKModulator::setSymbolRate(double rate) {
    symbolRate = rate;
    LOG_INFO("设置BPSK符号率: " + std::to_string(rate) + " Hz");
    
    // 更新每符号样本数
    samplesPerSymbol = static_cast<int>(sampleRate / symbolRate);
    LOG_INFO("更新每符号样本数: " + std::to_string(samplesPerSymbol));
}

// 获取符号率
double BPSKModulator::getSymbolRate() const {
    return symbolRate;
}

// 设置每符号样本数
void BPSKModulator::setSamplesPerSymbol(int samples) {
    samplesPerSymbol = samples;
    LOG_INFO("设置每符号样本数: " + std::to_string(samples));
    
    // 更新符号率
    symbolRate = sampleRate / samplesPerSymbol;
    LOG_INFO("更新符号率: " + std::to_string(symbolRate) + " Hz");
}

// 获取每符号样本数
int BPSKModulator::getSamplesPerSymbol() const {
    return samplesPerSymbol;
}

// 设置滤波器长度
void BPSKModulator::setFilterLength(int length) {
    filterLength = length;
    LOG_INFO("设置滤波器长度: " + std::to_string(length));
}

// 获取滤波器长度
int BPSKModulator::getFilterLength() const {
    return filterLength;
}

// 生成脉冲整形滤波器
std::vector<double> BPSKModulator::generatePulseShapingFilter() const {
    // 生成升余弦滤波器
    std::vector<double> filter(filterLength);
    
    // 滚降系数
    double beta = 0.35;
    
    // 生成滤波器系数
    for (int i = 0; i < filterLength; ++i) {
        double t = i - filterLength / 2;
        
        if (t == 0) {
            filter[i] = 1.0;
        } else if (std::abs(t) == samplesPerSymbol / (2.0 * beta)) {
            filter[i] = (M_PI / 4.0) * std::sin(M_PI / (2.0 * beta));
        } else {
            double numerator = std::sin(M_PI * t / samplesPerSymbol);
            double denominator = M_PI * t / samplesPerSymbol;
            double cosTerm = std::cos(M_PI * beta * t / samplesPerSymbol);
            double denomTerm = 1.0 - std::pow(2.0 * beta * t / samplesPerSymbol, 2.0);
            
            filter[i] = (numerator / denominator) * (cosTerm / denomTerm);
        }
    }
    
    // 归一化
    double sum = 0.0;
    for (double coeff : filter) {
        sum += coeff;
    }
    
    if (sum > 0.0) {
        for (double& coeff : filter) {
            coeff /= sum;
        }
    }
    
    return filter;
}

// 应用脉冲整形滤波器
std::vector<std::complex<double>> BPSKModulator::applyFilter(const std::vector<std::complex<double>>& signal, const std::vector<double>& filter) const {
    if (signal.empty() || filter.empty()) {
        return signal;
    }
    
    // 卷积
    std::vector<std::complex<double>> filteredSignal(signal.size() + filter.size() - 1);
    
    for (size_t i = 0; i < filteredSignal.size(); ++i) {
        std::complex<double> sum(0.0, 0.0);
        
        for (size_t j = 0; j < filter.size(); ++j) {
            if (i >= j && i - j < signal.size()) {
                sum += signal[i - j] * filter[j];
            }
        }
        
        filteredSignal[i] = sum;
    }
    
    return filteredSignal;
}

} // namespace modulation
} // namespace physical
} // namespace link16
