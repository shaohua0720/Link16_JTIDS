#include "QPSKModulator.h"
#include "core/utils/logger.h"
#include <iostream>
#include <cmath>

namespace link16 {
namespace physical {
namespace modulation {

// 构造函数
QPSKModulator::QPSKModulator()
    : sampleRate(1.0e6), symbolRate(5.0e4), samplesPerSymbol(20), filterLength(32) {
}

// 析构函数
QPSKModulator::~QPSKModulator() {
}

// 初始化调制器
bool QPSKModulator::initialize() {
    LOG_INFO("初始化QPSK调制器");
    
    // 设置默认参数
    sampleRate = 1.0e6;     // 1MHz
    symbolRate = 5.0e4;     // 50kHz
    samplesPerSymbol = 20;  // 每符号20个样本
    filterLength = 32;      // 32点滤波器
    
    return true;
}

// 调制二进制数据
std::vector<std::complex<double>> QPSKModulator::modulate(const std::string& bits) {
    LOG_INFO("QPSK调制: " + std::to_string(bits.length()) + " 位");
    
    // 将二进制数据转换为符号
    std::vector<std::complex<double>> symbols = bitsToSymbols(bits);
    
    // 生成基带信号
    std::vector<std::complex<double>> baseband;
    baseband.reserve(symbols.size() * samplesPerSymbol);
    
    for (const auto& symbol : symbols) {
        // 每个符号生成多个样本
        for (int i = 0; i < samplesPerSymbol; ++i) {
            baseband.push_back(symbol);
        }
    }
    
    // 应用脉冲整形滤波器
    std::vector<double> filter = generatePulseShapingFilter();
    std::vector<std::complex<double>> filteredSignal = applyFilter(baseband, filter);
    
    return filteredSignal;
}

// 解调复数信号
std::string QPSKModulator::demodulate(const std::vector<std::complex<double>>& signal) {
    LOG_INFO("QPSK解调: " + std::to_string(signal.size()) + " 个样本");
    
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
    
    // 提取符号
    std::vector<std::complex<double>> symbols;
    symbols.reserve(numSymbols);
    
    for (size_t i = 0; i < numSymbols; ++i) {
        // 计算符号中心位置
        size_t centerIndex = i * samplesPerSymbol + samplesPerSymbol / 2;
        
        // 确保索引有效
        if (centerIndex >= signal.size()) {
            break;
        }
        
        // 提取符号
        symbols.push_back(signal[centerIndex]);
    }
    
    // 将符号转换为二进制数据
    return symbolsToBits(symbols);
}

// 设置采样率
void QPSKModulator::setSampleRate(double rate) {
    sampleRate = rate;
    LOG_INFO("设置QPSK采样率: " + std::to_string(rate) + " Hz");
}

// 获取采样率
double QPSKModulator::getSampleRate() const {
    return sampleRate;
}

// 设置符号率
void QPSKModulator::setSymbolRate(double rate) {
    symbolRate = rate;
    LOG_INFO("设置QPSK符号率: " + std::to_string(rate) + " Hz");
    
    // 更新每符号样本数
    samplesPerSymbol = static_cast<int>(sampleRate / symbolRate);
    LOG_INFO("更新每符号样本数: " + std::to_string(samplesPerSymbol));
}

// 获取符号率
double QPSKModulator::getSymbolRate() const {
    return symbolRate;
}

// 设置每符号样本数
void QPSKModulator::setSamplesPerSymbol(int samples) {
    samplesPerSymbol = samples;
    LOG_INFO("设置每符号样本数: " + std::to_string(samples));
    
    // 更新符号率
    symbolRate = sampleRate / samplesPerSymbol;
    LOG_INFO("更新符号率: " + std::to_string(symbolRate) + " Hz");
}

// 获取每符号样本数
int QPSKModulator::getSamplesPerSymbol() const {
    return samplesPerSymbol;
}

// 设置滤波器长度
void QPSKModulator::setFilterLength(int length) {
    filterLength = length;
    LOG_INFO("设置滤波器长度: " + std::to_string(length));
}

// 获取滤波器长度
int QPSKModulator::getFilterLength() const {
    return filterLength;
}

// 生成脉冲整形滤波器
std::vector<double> QPSKModulator::generatePulseShapingFilter() const {
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
std::vector<std::complex<double>> QPSKModulator::applyFilter(const std::vector<std::complex<double>>& signal, const std::vector<double>& filter) const {
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

// 将二进制数据转换为符号
std::vector<std::complex<double>> QPSKModulator::bitsToSymbols(const std::string& bits) const {
    // QPSK: 每2位对应一个符号
    std::vector<std::complex<double>> symbols;
    symbols.reserve((bits.length() + 1) / 2);
    
    for (size_t i = 0; i < bits.length(); i += 2) {
        // 获取2位
        char bit1 = bits[i];
        char bit2 = (i + 1 < bits.length()) ? bits[i + 1] : '0';
        
        // 映射到复平面
        double real = (bit1 == '0') ? -1.0 : 1.0;
        double imag = (bit2 == '0') ? -1.0 : 1.0;
        
        symbols.push_back(std::complex<double>(real, imag));
    }
    
    return symbols;
}

// 将符号转换为二进制数据
std::string QPSKModulator::symbolsToBits(const std::vector<std::complex<double>>& symbols) const {
    std::string bits;
    bits.reserve(symbols.size() * 2);
    
    for (const auto& symbol : symbols) {
        // 解码实部
        bits.push_back((std::real(symbol) < 0.0) ? '0' : '1');
        
        // 解码虚部
        bits.push_back((std::imag(symbol) < 0.0) ? '0' : '1');
    }
    
    return bits;
}

} // namespace modulation
} // namespace physical
} // namespace link16
