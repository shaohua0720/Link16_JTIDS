#pragma once
#include <string>
#include <vector>

namespace link16 {
namespace simulation {
namespace metrics {

// 误码率计算器
class BER {
public:
    // 构造函数
    BER();
    
    // 析构函数
    ~BER();
    
    // 计算误码率
    double calculate(const std::string& original, const std::string& received);
    
    // 计算误码率(比特向量版本)
    double calculate(const std::vector<bool>& original, const std::vector<bool>& received);
    
    // 计算误码率(字符串向量版本)
    std::vector<double> calculateBatch(const std::vector<std::string>& originals, const std::vector<std::string>& receiveds);
    
    // 计算误码率(比特向量向量版本)
    std::vector<double> calculateBatch(const std::vector<std::vector<bool>>& originals, const std::vector<std::vector<bool>>& receiveds);
    
    // 计算平均误码率
    double calculateAverage(const std::vector<double>& bers);
    
    // 计算理论AWGN信道BPSK调制的误码率
    double calculateTheoretical_AWGN_BPSK(double snr);
    
    // 计算理论瑞利信道BPSK调制的误码率
    double calculateTheoretical_Rayleigh_BPSK(double snr);
    
    // 将字符串转换为比特向量
    std::vector<bool> stringToBits(const std::string& str);
    
    // 将比特向量转换为字符串
    std::string bitsToString(const std::vector<bool>& bits);
};

} // namespace metrics
} // namespace simulation
} // namespace link16
