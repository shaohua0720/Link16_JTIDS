#include "BIPCoder.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>

namespace link16 {
namespace coding {
namespace error_detection {

// 构造函数
BIPCoder::BIPCoder() {
}

// 析构函数
BIPCoder::~BIPCoder() {
}

// 计算BIP校验位
std::bitset<5> BIPCoder::calculateBIP(const std::string& data) {
    return BIP(data);
}

// 验证BIP校验位
bool BIPCoder::verifyBIP(const std::string& data, const std::bitset<5>& bip) {
    std::bitset<5> calculatedBIP = calculateBIP(data);
    return calculatedBIP == bip;
}

// 添加BIP校验位
std::string BIPCoder::addBIP(const std::string& data) {
    std::bitset<5> bip = calculateBIP(data);
    return data + bip.to_string();
}

// 从带BIP的数据中提取原始数据
std::string BIPCoder::extractData(const std::string& dataWithBIP) {
    if (dataWithBIP.length() < 5) {
        std::cerr << "数据长度不足，无法提取BIP" << std::endl;
        return dataWithBIP;
    }
    
    return dataWithBIP.substr(0, dataWithBIP.length() - 5);
}

// 全局BIP计算函数
std::bitset<5> BIP(const std::string& data) {
    // 如果数据为空，返回全0
    if (data.empty()) {
        return std::bitset<5>("00000");
    }
    
    // 将字符串转换为位序列
    std::vector<bool> bits;
    for (char c : data) {
        // 对每个字符，提取其二进制表示
        std::bitset<8> charBits(c);
        for (int i = 7; i >= 0; --i) {
            bits.push_back(charBits[i]);
        }
    }
    
    // 初始化5个校验位
    std::bitset<5> bip;
    
    // 计算每个校验位
    // 在Link16中，通常使用奇校验，即每组中1的个数加上校验位应为奇数
    for (size_t i = 0; i < 5; ++i) {
        // 计算当前组中1的个数
        int count = 0;
        for (size_t j = i; j < bits.size(); j += 5) {
            if (bits[j]) {
                count++;
            }
        }
        
        // 设置校验位，使总数为奇数
        bip[i] = (count % 2 == 0) ? 1 : 0;
    }
    
    return bip;
}

// 验证带BIP的数据是否正确
bool validateBIP(const std::string& dataWithBIP) {
    if (dataWithBIP.length() < 5) {
        std::cerr << "数据长度不足，无法验证BIP" << std::endl;
        return false;
    }
    
    // 提取数据和BIP
    std::string data = dataWithBIP.substr(0, dataWithBIP.length() - 5);
    std::string bipStr = dataWithBIP.substr(dataWithBIP.length() - 5);
    std::bitset<5> receivedBIP(bipStr);
    
    // 计算BIP并验证
    std::bitset<5> calculatedBIP = BIP(data);
    return calculatedBIP == receivedBIP;
}

// 修复带BIP的数据中的单比特错误（如果可能）
std::string correctSingleBitError(const std::string& dataWithBIP) {
    if (dataWithBIP.length() < 5) {
        std::cerr << "数据长度不足，无法进行错误修正" << std::endl;
        return dataWithBIP;
    }
    
    // 提取数据和BIP
    std::string data = dataWithBIP.substr(0, dataWithBIP.length() - 5);
    std::string bipStr = dataWithBIP.substr(dataWithBIP.length() - 5);
    std::bitset<5> receivedBIP(bipStr);
    
    // 计算BIP
    std::bitset<5> calculatedBIP = BIP(data);
    
    // 如果BIP匹配，数据无错误
    if (calculatedBIP == receivedBIP) {
        return dataWithBIP;
    }
    
    // 计算错误位置的校验位模式
    std::bitset<5> errorPattern = calculatedBIP ^ receivedBIP;
    
    // 将数据转换为位序列
    std::vector<bool> bits;
    for (char c : data) {
        std::bitset<8> charBits(c);
        for (int i = 7; i >= 0; --i) {
            bits.push_back(charBits[i]);
        }
    }
    
    // 尝试找到错误位置
    for (size_t i = 0; i < bits.size(); ++i) {
        // 计算当前位置对应的校验位模式
        std::bitset<5> posPattern;
        for (size_t j = 0; j < 5; ++j) {
            if ((i % 5) == j) {
                posPattern[j] = 1;
            }
        }
        
        // 如果模式匹配，翻转该位
        if (posPattern == errorPattern) {
            bits[i] = !bits[i];
            
            // 重建修正后的数据
            std::string correctedData;
            for (size_t j = 0; j < bits.size(); j += 8) {
                std::bitset<8> charBits;
                for (size_t k = 0; k < 8 && (j + k) < bits.size(); ++k) {
                    charBits[7 - k] = bits[j + k];
                }
                correctedData.push_back(static_cast<char>(charBits.to_ulong()));
            }
            
            // 添加原始BIP
            return correctedData + bipStr;
        }
    }
    
    // 如果无法修正，返回原始数据
    std::cerr << "无法修正错误，可能存在多位错误" << std::endl;
    return dataWithBIP;
}

} // namespace error_detection
} // namespace coding
} // namespace link16
