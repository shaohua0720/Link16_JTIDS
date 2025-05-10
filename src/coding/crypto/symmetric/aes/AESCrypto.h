#pragma once

#include <string>
#include <vector>
#include <memory>

namespace link16 {
namespace coding {
namespace crypto {

/**
 * @brief AES加密器类
 */
class AESCrypto {
public:
    /**
     * @brief 构造函数
     * @param keySize 密钥大小，可以是128、192或256
     */
    AESCrypto(int keySize = 128);
    
    /**
     * @brief 析构函数
     */
    ~AESCrypto();
    
    /**
     * @brief 加密函数
     * @param plaintext 明文
     * @param key 密钥
     * @return 密文
     */
    std::string encrypt(const std::string& plaintext, const std::string& key);
    
    /**
     * @brief 解密函数
     * @param ciphertext 密文
     * @param key 密钥
     * @return 明文
     */
    std::string decrypt(const std::string& ciphertext, const std::string& key);
    
    /**
     * @brief 设置密钥
     * @param key 密钥
     */
    void setKey(const std::string& key);
    
    /**
     * @brief 获取密钥
     * @return 密钥
     */
    const std::string& getKey() const;
    
    /**
     * @brief 设置密钥大小
     * @param keySize 密钥大小，可以是128、192或256
     */
    void setKeySize(int keySize);
    
    /**
     * @brief 获取密钥大小
     * @return 密钥大小
     */
    int getKeySize() const;
    
    /**
     * @brief 生成随机密钥
     * @return 随机密钥
     */
    std::string generateRandomKey();
    
    /**
     * @brief 检查密钥是否有效
     * @param key 密钥
     * @return 密钥是否有效
     */
    bool isValidKey(const std::string& key);

private:
    // 密钥
    std::string key;
    
    // 密钥大小
    int keySize;
    
    // 内部实现
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace crypto
} // namespace coding
} // namespace link16