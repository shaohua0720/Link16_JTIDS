#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

namespace link16 {
namespace coding {
namespace error_correction {

/**
 * @brief Reed-Solomon编码器类
 */
class RSCoder {
public:
    /**
     * @brief 构造函数
     * @param codeLength 编码长度，必须大于0且小于256
     * @param dataLength 数据长度，必须大于0且小于codeLength
     */
    RSCoder(int codeLength = 31, int dataLength = 15);
    
    /**
     * @brief 析构函数
     */
    ~RSCoder();
    
    /**
     * @brief 编码函数
     * @param message 要编码的消息
     * @return 编码后的数据
     */
    std::string encode(const std::string& message);
    
    /**
     * @brief 解码函数
     * @param encodedData 编码后的数据
     * @return 解码后的消息
     */
    std::string decode(const std::string& encodedData);
    
    /**
     * @brief 设置编码参数
     * @param codeLength 编码长度，必须大于0且小于256
     * @param dataLength 数据长度，必须大于0且小于codeLength
     */
    void setParameters(int codeLength, int dataLength);
    
    /**
     * @brief 获取编码长度
     * @return 编码长度
     */
    int getCodeLength() const;
    
    /**
     * @brief 获取数据长度
     * @return 数据长度
     */
    int getDataLength() const;
    
    /**
     * @brief 获取纠错能力
     * @return 纠错能力，即可以纠正的错误符号数量
     */
    int getErrorCorrectionCapability() const;
    
    /**
     * @brief 检查参数是否有效
     * @param codeLength 编码长度
     * @param dataLength 数据长度
     * @return 参数是否有效
     */
    static bool isValidParameters(int codeLength, int dataLength);
    
    /**
     * @brief 添加错误
     * @param encodedData 编码后的数据
     * @param errorCount 错误数量
     * @return 添加错误后的数据
     */
    std::string addErrors(const std::string& encodedData, int errorCount);
    
    /**
     * @brief 计算错误率
     * @param originalData 原始数据
     * @param recoveredData 恢复后的数据
     * @return 错误率
     */
    double calculateErrorRate(const std::string& originalData, const std::string& recoveredData);

private:
    // 编码长度
    int codeLength;
    
    // 数据长度
    int dataLength;
    
    // 纠错能力
    int errorCorrectionCapability;
    
    // 内部实现
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace error_correction
} // namespace coding
} // namespace link16
