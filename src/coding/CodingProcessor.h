#pragma once

#include <string>
#include <memory>
#include <vector>

namespace link16 {
namespace coding {

// 前向声明
class ReedSolomon;
class AESCrypto;
class Interleaver;
class BIPCoder;

/**
 * @brief 编码层处理器接口
 */
class CodingProcessor {
public:
    // 构造函数
    CodingProcessor();
    
    // 析构函数
    virtual ~CodingProcessor();
    
    // 初始化
    bool initialize();
    
    // 关闭
    void shutdown();
    
    // 编码数据
    bool encodeData(const std::string& rawData, std::string& encodedData);
    
    // 解码数据
    bool decodeData(const std::string& encodedData, std::string& rawData);
    
    // Reed-Solomon编码
    bool rsEncode(const std::string& data, std::string& encodedData, int codeLength = 31, int dataLength = 15);
    
    // Reed-Solomon解码
    bool rsDecode(const std::string& encodedData, std::string& data, int codeLength = 31, int dataLength = 15);
    
    // AES加密
    bool aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext);
    
    // AES解密
    bool aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext);
    
    // 交织
    bool interleave(const std::string& data, std::string& interleavedData, int rows = 8, int cols = 8);
    
    // 解交织
    bool deinterleave(const std::string& interleavedData, std::string& data, int rows = 8, int cols = 8);
    
    // BIP校验计算
    bool calculateBIP(const std::string& data, std::vector<bool>& bip);
    
    // BIP校验验证
    bool verifyBIP(const std::string& data, const std::vector<bool>& bip);
    
    // 设置加密密钥
    void setEncryptionKey(const std::string& key);
    
    // 获取加密密钥
    const std::string& getEncryptionKey() const;
    
    // 新增方法，支持CodingAPI的功能
    
    // 生成AES随机密钥
    std::string generateAESKey();
    
    // 设置AES密钥大小
    bool setAESKeySize(int keySize);
    
    // 获取AES密钥大小
    int getAESKeySize();
    
    // CRC校验计算
    uint32_t calculateCRC32(const std::string& data);
    
    // CRC校验验证
    bool verifyCRC32(const std::string& data, uint32_t crc);

private:
    // 初始化状态
    bool initialized;
    
    // 加密密钥
    std::string encryptionKey;
    
    // 组件实例
    std::unique_ptr<error_correction::RSCoder> rsEncoder;
    std::unique_ptr<crypto::AESCrypto> aesEncoder;
    std::unique_ptr<interleaving::Interleaver> interleaver;
    std::unique_ptr<error_detection::BIPCoder> bipCoder;
};

} // namespace coding
} // namespace link16
