#include "RayleighChannel.h"
#include <random>
#include <cmath>

namespace link16 {
namespace simulation {
namespace channel {

// 构造函数
RayleighChannel::RayleighChannel(double snr, double dopplerShift) 
    : ChannelModel(snr), dopplerShift(dopplerShift) {
}

// 析构函数
RayleighChannel::~RayleighChannel() {
}

// 处理信号
std::vector<std::complex<double>> RayleighChannel::process(const std::vector<std::complex<double>>& input) {
    // 计算信号功率
    double signalPower = calculateSignalPower(input);
    
    // 生成瑞利衰落系数
    std::vector<std::complex<double>> fadingCoefficients = generateRayleighFading(input.size());
    
    // 应用衰落
    std::vector<std::complex<double>> fadedSignal(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        fadedSignal[i] = input[i] * fadingCoefficients[i];
    }
    
    // 计算衰落后的信号功率
    double fadedPower = calculateSignalPower(fadedSignal);
    
    // 归一化，保持信号功率不变
    if (fadedPower > 0.0) {
        double scale = std::sqrt(signalPower / fadedPower);
        for (auto& sample : fadedSignal) {
            sample *= scale;
        }
    }
    
    // 计算噪声功率
    double noisePower = signalPower / std::pow(10.0, snr / 10.0);
    
    // 添加噪声
    return addNoise(fadedSignal, noisePower);
}

// 设置信道参数
void RayleighChannel::setParameter(const std::string& name, double value) {
    if (name == "doppler_shift") {
        dopplerShift = value;
    } else {
        ChannelModel::setParameter(name, value);
    }
}

// 获取信道参数
double RayleighChannel::getParameter(const std::string& name) const {
    if (name == "doppler_shift") {
        return dopplerShift;
    } else {
        return ChannelModel::getParameter(name);
    }
}

// 获取信道名称
std::string RayleighChannel::getName() const {
    return "Rayleigh";
}

// 获取信道描述
std::string RayleighChannel::getDescription() const {
    return "瑞利衰落信道，SNR = " + std::to_string(snr) + " dB，多普勒频移 = " + std::to_string(dopplerShift) + " Hz";
}

// 设置多普勒频移
void RayleighChannel::setDopplerShift(double shift) {
    dopplerShift = shift;
}

// 获取多普勒频移
double RayleighChannel::getDopplerShift() const {
    return dopplerShift;
}

// 生成瑞利衰落系数
std::vector<std::complex<double>> RayleighChannel::generateRayleighFading(size_t length) {
    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0 / std::sqrt(2.0));
    
    // 生成瑞利衰落系数
    std::vector<std::complex<double>> coefficients(length);
    for (size_t i = 0; i < length; ++i) {
        double real = dist(gen);
        double imag = dist(gen);
        coefficients[i] = std::complex<double>(real, imag);
    }
    
    // 如果有多普勒频移，应用频移
    if (dopplerShift != 0.0) {
        // 简化实现，实际应该使用Jake's模型或其他更复杂的模型
        for (size_t i = 0; i < length; ++i) {
            double phase = 2.0 * M_PI * dopplerShift * i / length;
            std::complex<double> phasor(std::cos(phase), std::sin(phase));
            coefficients[i] *= phasor;
        }
    }
    
    return coefficients;
}

} // namespace channel
} // namespace simulation
} // namespace link16
