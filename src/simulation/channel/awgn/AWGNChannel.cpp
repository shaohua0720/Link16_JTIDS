#include "AWGNChannel.h"
#include <cmath>

namespace link16 {
namespace simulation {
namespace channel {

// 构造函数
AWGNChannel::AWGNChannel(double snr) : ChannelModel(snr) {
}

// 析构函数
AWGNChannel::~AWGNChannel() {
}

// 处理信号
std::vector<std::complex<double>> AWGNChannel::process(const std::vector<std::complex<double>>& input) {
    // 计算信号功率
    double signalPower = calculateSignalPower(input);
    
    // 计算噪声功率
    double noisePower = signalPower / std::pow(10.0, snr / 10.0);
    
    // 添加噪声
    return addNoise(input, noisePower);
}

// 获取信道名称
std::string AWGNChannel::getName() const {
    return "AWGN";
}

// 获取信道描述
std::string AWGNChannel::getDescription() const {
    return "加性高斯白噪声信道，SNR = " + std::to_string(snr) + " dB";
}

} // namespace channel
} // namespace simulation
} // namespace link16
