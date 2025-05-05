#pragma once
#include <string>
#include <bitset>
#include <cstdint>

namespace link16 {
namespace coding {
namespace error_detection {

class BIPCoder {
public:
    // 构造函数
    BIPCoder();
    
    // 析构函数
    ~BIPCoder();
    
    // 计算BIP校验位
    std::bitset<5> calculateBIP(const std::string& data);
    
    // 验证BIP校验位
    bool verifyBIP(const std::string& data, const std::bitset<5>& bip);
    
    // 添加BIP校验位
    std::string addBIP(const std::string& data);
    
    // 从带BIP的数据中提取原始数据
    std::string extractData(const std::string& dataWithBIP);
};

// 全局BIP计算函数
std::bitset<5> BIP(const std::string& data);

} // namespace error_detection
} // namespace coding
} // namespace link16
