#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace link16 {
namespace coding {
namespace error_correction {

/**
 * @brief Reed-Solomon编码器类
 *
 * Reed-Solomon码是一种前向纠错码，广泛应用于数字通信和存储系统中。
 * 它可以检测和纠正多个随机错误，特别适合于纠正突发错误。
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
     * @param encodedData 编码后的数据缓冲区，大小必须至少为codeLength
     * @return 编码是否成功
     */
    bool encode(const std::string& message, uint8_t* encodedData);

    /**
     * @brief 编码函数(字节数组版本)
     * @param data 要编码的数据
     * @param dataSize 数据大小，必须小于等于dataLength
     * @param encodedData 编码后的数据缓冲区，大小必须至少为codeLength
     * @return 编码是否成功
     */
    bool encode(const uint8_t* data, size_t dataSize, uint8_t* encodedData);

    /**
     * @brief 解码函数
     * @param encodedData 编码后的数据，大小必须为codeLength
     * @param message 解码后的消息
     * @return 解码是否成功
     */
    bool decode(const uint8_t* encodedData, std::string& message);

    /**
     * @brief 解码函数(字节数组版本)
     * @param encodedData 编码后的数据，大小必须为codeLength
     * @param decodedData 解码后的数据缓冲区，大小必须至少为dataLength
     * @param decodedSize 解码后的数据大小
     * @return 解码是否成功
     */
    bool decode(const uint8_t* encodedData, uint8_t* decodedData, size_t& decodedSize);

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

private:
    // 编码长度
    int codeLength;

    // 数据长度
    int dataLength;

    // 纠错能力
    int errorCorrectionCapability;
};

/**
 * @brief 全局RS编码函数
 * @param codeLength 编码长度，必须大于0且小于256
 * @param dataLength 数据长度，必须大于0且小于codeLength
 * @param message 要编码的消息
 * @param encodedData 编码后的数据缓冲区，大小必须至少为codeLength
 * @return 编码是否成功
 */
bool RS(int codeLength, int dataLength, const std::string& message, void* encodedData);

/**
 * @brief 全局RS编码函数(字节数组版本)
 * @param codeLength 编码长度，必须大于0且小于256
 * @param dataLength 数据长度，必须大于0且小于codeLength
 * @param data 要编码的数据
 * @param dataSize 数据大小，必须小于等于dataLength
 * @param encodedData 编码后的数据缓冲区，大小必须至少为codeLength
 * @return 编码是否成功
 */
bool RS(int codeLength, int dataLength, const uint8_t* data, size_t dataSize, void* encodedData);

} // namespace error_correction
} // namespace coding
} // namespace link16
