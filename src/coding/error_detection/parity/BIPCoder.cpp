#include "BIPCoder.h"
#include <iostream>

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
    // 这里只是一个接口，实际实现将在后续完成
    // 在实际实现中，将根据Link16协议计算BIP校验位
    
    // 为了编译通过，先返回一个固定值
    return std::bitset<5>("00000");
}

} // namespace error_detection
} // namespace coding
} // namespace link16
