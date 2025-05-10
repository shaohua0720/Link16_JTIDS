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

private:
    // Reed-Solomon编码器
    std::unique_ptr<ReedSolomon> rsEncoder;
    
    // AES加密器
    std::unique_ptr<AESCrypto> aesEncoder;
    
    // 交织器
    std::unique_ptr<Interleaver> interleaver;
    
    // BIP校验器
    std::unique_ptr<BIPCoder> bipCoder;
    
    // 加密密钥
    std::string encryptionKey;
    
    // 初始化状态
    bool initialized;
};

} // namespace coding
} // namespace link16