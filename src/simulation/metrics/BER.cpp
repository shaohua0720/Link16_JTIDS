#include "BER.h"
#include "core/utils/logger.h"
#include <cmath>
#include <numeric>
#include <algorithm>

namespace link16 {
namespace simulation {
namespace metrics {

// 构造函数
BER::BER() {
}

// 析构函数
BER::~BER() {
}

// 计算误码率
double BER::calculate(const std::string& original, const std::string& received) {
    // 将字符串转换为比特向量
    std::vector<bool> originalBits = stringToBits(original);
    std::vector<bool> receivedBits = stringToBits(received);
    
    return calculate(originalBits, receivedBits);
}

// 计算误码率(比特向量版本)
double BER::calculate(const std::vector<bool>& original, const std::vector<bool>& received) {
    // 检查长度是否一致
    if (original.size() != received.size()) {
        LOG_WARNING("原始数据和接收数据长度不一致，将使用较短的长度");
    }
    
    // 使用较短的长度
    size_t length = std::min(original.size(), received.size());
    
    if (length == 0) {
        LOG_ERROR("数据长度为0，无法计算误码率");
        return 0.0;
    }
    
    // 计算错误比特数
    size_t errorCount = 0;
    for (size_t i = 0; i < length; ++i) {
        if (original[i] != received[i]) {
            errorCount++;
        }
    }
    
    // 计算误码率
    return static_cast<double>(errorCount) / length;
}

// 计算误码率(字符串向量版本)
std::vector<double> BER::calculateBatch(const std::vector<std::string>& originals, const std::vector<std::string>& receiveds) {
    // 检查长度是否一致
    if (originals.size() != receiveds.size()) {
        LOG_WARNING("原始数据和接收数据数量不一致，将使用较短的数量");
    }
    
    // 使用较短的数量
    size_t count = std::min(originals.size(), receiveds.size());
    
    // 计算每对数据的误码率
    std::vector<double> results;
    results.reserve(count);
    
    for (size_t i = 0; i < count; ++i) {
        results.push_back(calculate(originals[i], receiveds[i]));
    }
    
    return results;
}

// 计算误码率(比特向量向量版本)
std::vector<double> BER::calculateBatch(const std::vector<std::vector<bool>>& originals, const std::vector<std::vector<bool>>& receiveds) {
    // 检查长度是否一致
    if (originals.size() != receiveds.size()) {
        LOG_WARNING("原始数据和接收数据数量不一致，将使用较短的数量");
    }
    
    // 使用较短的数量
    size_t count = std::min(originals.size(), receiveds.size());
    
    // 计算每对数据的误码率
    std::vector<double> results;
    results.reserve(count);
    
    for (size_t i = 0; i < count; ++i) {
        results.push_back(calculate(originals[i], receiveds[i]));
    }
    
    return results;
}

// 计算平均误码率
double BER::calculateAverage(const std::vector<double>& bers) {
    if (bers.empty()) {
        LOG_ERROR("误码率数组为空，无法计算平均值");
        return 0.0;
    }
    
    // 计算平均值
    double sum = std::accumulate(bers.begin(), bers.end(), 0.0);
    return sum / bers.size();
}

// 计算理论AWGN信道BPSK调制的误码率
double BER::calculateTheoretical_AWGN_BPSK(double snr) {
    // 将dB转换为线性比例
    double snr_linear = std::pow(10.0, snr / 10.0);
    
    // 计算理论误码率
    return 0.5 * std::erfc(std::sqrt(snr_linear / 2.0));
}

// 计算理论瑞利信道BPSK调制的误码率
double BER::calculateTheoretical_Rayleigh_BPSK(double snr) {
    // 将dB转换为线性比例
    double snr_linear = std::pow(10.0, snr / 10.0);
    
    // 计算理论误码率
    return 0.5 * (1.0 - std::sqrt(snr_linear / (1.0 + snr_linear)));
}

// 将字符串转换为比特向量
std::vector<bool> BER::stringToBits(const std::string& str) {
    std::vector<bool> bits;
    bits.reserve(str.length() * 8);
    
    for (char c : str) {
        for (int i = 7; i >= 0; --i) {
            bits.push_back((c >> i) & 1);
        }
    }
    
    return bits;
}

// 将比特向量转换为字符串
std::string BER::bitsToString(const std::vector<bool>& bits) {
    // 确保比特数是8的倍数
    size_t paddedLength = ((bits.size() + 7) / 8) * 8;
    std::vector<bool> paddedBits = bits;
    paddedBits.resize(paddedLength, false);
    
    std::string str;
    str.reserve(paddedLength / 8);
    
    for (size_t i = 0; i < paddedLength; i += 8) {
        char c = 0;
        for (size_t j = 0; j < 8; ++j) {
            c = (c << 1) | (paddedBits[i + j] ? 1 : 0);
        }
        str.push_back(c);
    }
    
    return str;
}

} // namespace metrics
} // namespace simulation
} // namespace link16
