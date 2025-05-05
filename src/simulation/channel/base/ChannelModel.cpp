#include "ChannelModel.h"
#include <random>
#include <cmath>
#include <stdexcept>

namespace link16 {
namespace simulation {
namespace channel {

// 构造函数
ChannelModel::ChannelModel(double snr) : snr(snr) {
}

// 析构函数
ChannelModel::~ChannelModel() {
}

// 设置信噪比(dB)
void ChannelModel::setSnr(double snr) {
    this->snr = snr;
}

// 获取信噪比(dB)
double ChannelModel::getSnr() const {
    return snr;
}

// 设置信道参数
void ChannelModel::setParameter(const std::string& name, double value) {
    throw std::runtime_error("参数 '" + name + "' 在基类中未定义");
}

// 获取信道参数
double ChannelModel::getParameter(const std::string& name) const {
    throw std::runtime_error("参数 '" + name + "' 在基类中未定义");
}

// 添加高斯白噪声
std::vector<std::complex<double>> ChannelModel::addNoise(const std::vector<std::complex<double>>& signal, double noiseLevel) {
    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, std::sqrt(noiseLevel / 2.0));
    
    // 添加噪声
    std::vector<std::complex<double>> result = signal;
    for (auto& sample : result) {
        double noiseReal = dist(gen);
        double noiseImag = dist(gen);
        sample += std::complex<double>(noiseReal, noiseImag);
    }
    
    return result;
}

// 计算信号功率
double ChannelModel::calculateSignalPower(const std::vector<std::complex<double>>& signal) {
    double power = 0.0;
    for (const auto& sample : signal) {
        power += std::norm(sample);
    }
    return power / signal.size();
}

} // namespace channel
} // namespace simulation
} // namespace link16
