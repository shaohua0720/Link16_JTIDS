#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

namespace link16 {

// 前向声明
namespace coding {
class CodingProcessor;
}

namespace api {

/**
 * @brief 编码API类，提供Link16系统中的编码、加密和校验功能
 * 
 * 该类是编码功能的公共接口，内部使用CodingProcessor实现具体功能。
 * 采用单例模式，通过getInstance()获取全局唯一实例。
 */
class CodingAPI {
public:
    /**
     * @brief 获取单例实例
     * @return CodingAPI& 全局唯一的CodingAPI实例
     */
    static CodingAPI& getInstance();
    
    /**
     * @brief 初始化编码API
     * @return bool 初始化是否成功
     */
    bool initialize();
    
    /**
     * @brief 关闭编码API，释放资源
     */
    void shutdown();
    
    /**
     * @brief Reed-Solomon编码
     * @param data 原始数据
     * @param encodedData 编码后的数据
     * @param codeLength RS码字长度
     * @param dataLength RS信息字长度
     * @return bool 编码是否成功
     */
    bool rsEncode(const std::string& data, std::string& encodedData, int codeLength = 31, int dataLength = 15);
    
    /**
     * @brief Reed-Solomon解码
     * @param encodedData 编码后的数据
     * @param data 解码后的原始数据
     * @param codeLength RS码字长度
     * @param dataLength RS信息字长度
     * @return bool 解码是否成功
     */
    bool rsDecode(const std::string& encodedData, std::string& data, int codeLength = 31, int dataLength = 15);
    
    /**
     * @brief 计算CRC32校验码
     * @param data 需要计算校验码的数据
     * @return uint32_t CRC32校验码
     */
    uint32_t calculateCRC32(const std::string& data);
    
    /**
     * @brief 验证CRC32校验码
     * @param data 原始数据
     * @param crc 校验码
     * @return bool 校验是否通过
     */
    bool verifyCRC32(const std::string& data, uint32_t crc);
    
    /**
     * @brief AES加密
     * @param plaintext 明文
     * @param key 密钥
     * @param ciphertext 密文
     * @return bool 加密是否成功
     */
    bool aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext);
    
    /**
     * @brief AES解密
     * @param ciphertext 密文
     * @param key 密钥
     * @param plaintext 明文
     * @return bool 解密是否成功
     */
    bool aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext);
    
    /**
     * @brief 生成AES随机密钥
     * @return std::string 生成的密钥
     */
    std::string generateAESKey();
    
    /**
     * @brief 设置AES密钥大小
     * @param keySize 密钥大小(128, 192, 256)
     * @return bool 设置是否成功
     */
    bool setAESKeySize(int keySize);
    
    /**
     * @brief 获取当前AES密钥大小
     * @return int 密钥大小
     */
    int getAESKeySize();
    
    /**
     * @brief 数据交织
     * @param data 原始数据
     * @param interleavedData 交织后的数据
     * @param rows 交织矩阵行数
     * @param cols 交织矩阵列数
     * @return bool 交织是否成功
     */
    bool interleave(const std::string& data, std::string& interleavedData, int rows = 8, int cols = 8);
    
    /**
     * @brief 数据解交织
     * @param interleavedData 交织后的数据
     * @param data 解交织后的原始数据
     * @param rows 交织矩阵行数
     * @param cols 交织矩阵列数
     * @return bool 解交织是否成功
     */
    bool deinterleave(const std::string& interleavedData, std::string& data, int rows = 8, int cols = 8);

    /**
     * @brief 计算BIP校验位
     * @param data 需要计算校验位的数据
     * @return std::string BIP校验位字符串
     */
    std::string calculateBIP(const std::string& data);

    /**
     * @brief 验证BIP校验位
     * @param data 原始数据
     * @param bip BIP校验位字符串
     * @return bool 校验是否通过
     */
    bool verifyBIP(const std::string& data, const std::string& bip);
    
    /**
     * @brief 添加BIP校验位到数据
     * @param data 原始数据
     * @return std::string 添加了BIP校验位的数据
     */
    std::string addBIP(const std::string& data);
    
    /**
     * @brief 从带BIP的数据中提取原始数据
     * @param dataWithBIP 带BIP校验位的数据
     * @return std::string 提取的原始数据
     */
    std::string extractDataFromBIP(const std::string& dataWithBIP);

private:
    /**
     * @brief 私有构造函数(单例模式)
     */
    CodingAPI();
    
    // 禁止拷贝和赋值
    CodingAPI(const CodingAPI&) = delete;
    CodingAPI& operator=(const CodingAPI&) = delete;
    
    // 初始化状态
    bool initialized;
};

/**
 * @brief Reed-Solomon编码全局函数
 * @param data 原始数据
 * @param encodedData 编码后的数据
 * @param codeLength RS码字长度
 * @param dataLength RS信息字长度
 * @return bool 编码是否成功
 */
bool rsEncode(const std::string& data, std::string& encodedData, int codeLength = 31, int dataLength = 15);

/**
 * @brief Reed-Solomon解码全局函数
 * @param encodedData 编码后的数据
 * @param data 解码后的原始数据
 * @param codeLength RS码字长度
 * @param dataLength RS信息字长度
 * @return bool 解码是否成功
 */
bool rsDecode(const std::string& encodedData, std::string& data, int codeLength = 31, int dataLength = 15);

/**
 * @brief 计算CRC32校验码全局函数
 * @param data 需要计算校验码的数据
 * @return uint32_t CRC32校验码
 */
uint32_t calculateCRC32(const std::string& data);

/**
 * @brief 验证CRC32校验码全局函数
 * @param data 原始数据
 * @param crc 校验码
 * @return bool 校验是否通过
 */
bool verifyCRC32(const std::string& data, uint32_t crc);

/**
 * @brief AES加密全局函数
 * @param plaintext 明文
 * @param key 密钥
 * @param ciphertext 密文
 * @return bool 加密是否成功
 */
bool aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext);

/**
 * @brief AES解密全局函数
 * @param ciphertext 密文
 * @param key 密钥
 * @param plaintext 明文
 * @return bool 解密是否成功
 */
bool aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext);

/**
 * @brief 生成AES随机密钥全局函数
 * @return std::string 生成的密钥
 */
std::string generateAESKey();

/**
 * @brief 设置AES密钥大小全局函数
 * @param keySize 密钥大小(128, 192, 256)
 * @return bool 设置是否成功
 */
bool setAESKeySize(int keySize);

/**
 * @brief 获取当前AES密钥大小全局函数
 * @return int 密钥大小
 */
int getAESKeySize();

/**
 * @brief 数据交织全局函数
 * @param data 原始数据
 * @param interleavedData 交织后的数据
 * @param rows 交织矩阵行数
 * @param cols 交织矩阵列数
 * @return bool 交织是否成功
 */
bool interleave(const std::string& data, std::string& interleavedData, int rows = 8, int cols = 8);

/**
 * @brief 数据解交织全局函数
 * @param interleavedData 交织后的数据
 * @param data 解交织后的原始数据
 * @param rows 交织矩阵行数
 * @param cols 交织矩阵列数
 * @return bool 解交织是否成功
 */
bool deinterleave(const std::string& interleavedData, std::string& data, int rows = 8, int cols = 8);

/**
 * @brief 计算BIP校验位全局函数
 * @param data 需要计算校验位的数据
 * @return std::string BIP校验位字符串
 */
std::string calculateBIP(const std::string& data);

/**
 * @brief 验证BIP校验位全局函数
 * @param data 原始数据
 * @param bip BIP校验位字符串
 * @return bool 校验是否通过
 */
bool verifyBIP(const std::string& data, const std::string& bip);

/**
 * @brief 添加BIP校验位到数据全局函数
 * @param data 原始数据
 * @return std::string 添加了BIP校验位的数据
 */
std::string addBIP(const std::string& data);

/**
 * @brief 从带BIP的数据中提取原始数据全局函数
 * @param dataWithBIP 带BIP校验位的数据
 * @return std::string 提取的原始数据
 */
std::string extractDataFromBIP(const std::string& dataWithBIP);

} // namespace api
} // namespace link16
