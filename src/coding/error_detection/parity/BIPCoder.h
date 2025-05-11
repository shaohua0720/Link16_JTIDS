#pragma once
#include <string>
#include <bitset>
#include <cstdint>

namespace link16 {
namespace coding {
namespace error_detection {

/**
 * @brief BIP校验编码器类
 * 
 * BIP (Bit Integrity Parity) 是Link16协议中使用的一种校验机制，
 * 通常是5位校验码，用于检测数据传输过程中的错误。
 */
class BIPCoder {
public:
    /**
     * @brief 构造函数
     */
    BIPCoder();
    
    /**
     * @brief 析构函数
     */
    ~BIPCoder();
    
    /**
     * @brief 计算BIP校验位
     * 
     * @param data 输入数据
     * @return std::bitset<5> 5位BIP校验码
     */
    std::bitset<5> calculateBIP(const std::string& data);
    
    /**
     * @brief 验证BIP校验位
     * 
     * @param data 原始数据
     * @param bip BIP校验码
     * @return true 校验通过
     * @return false 校验失败
     */
    bool verifyBIP(const std::string& data, const std::bitset<5>& bip);
    
    /**
     * @brief 添加BIP校验位
     * 
     * @param data 原始数据
     * @return std::string 添加了BIP校验位的数据
     */
    std::string addBIP(const std::string& data);
    
    /**
     * @brief 从带BIP的数据中提取原始数据
     * 
     * @param dataWithBIP 带BIP校验位的数据
     * @return std::string 提取出的原始数据
     */
    std::string extractData(const std::string& dataWithBIP);
};

/**
 * @brief 全局BIP计算函数
 * 
 * @param data 输入数据
 * @return std::bitset<5> 5位BIP校验码
 */
std::bitset<5> BIP(const std::string& data);

/**
 * @brief 验证带BIP的数据是否正确
 * 
 * @param dataWithBIP 带BIP校验位的数据
 * @return true 校验通过
 * @return false 校验失败
 */
bool validateBIP(const std::string& dataWithBIP);

/**
 * @brief 修复带BIP的数据中的单比特错误（如果可能）
 * 
 * @param dataWithBIP 带BIP校验位的数据
 * @return std::string 修复后的数据（如果无法修复则返回原始数据）
 */
std::string correctSingleBitError(const std::string& dataWithBIP);

} // namespace error_detection
} // namespace coding
} // namespace link16
