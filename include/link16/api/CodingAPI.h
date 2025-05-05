#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace link16 {
namespace api {

// 编码API
class CodingAPI {
public:
    // 获取单例实例
    static CodingAPI& getInstance();
    
    // 初始化
    bool initialize();
    
    // 关闭
    void shutdown();
    
    // Reed-Solomon编码
    bool rsEncode(const std::string& data, std::string& encodedData, int codeLength = 31, int dataLength = 15);
    
    // Reed-Solomon解码
    bool rsDecode(const std::string& encodedData, std::string& data, int codeLength = 31, int dataLength = 15);
    
    // CRC校验
    uint32_t calculateCRC32(const std::string& data);
    
    // 验证CRC
    bool verifyCRC32(const std::string& data, uint32_t crc);
    
    // AES加密
    bool aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext);
    
    // AES解密
    bool aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext);
    
    // 交织
    bool interleave(const std::string& data, std::string& interleavedData, int rows = 8, int cols = 8);
    
    // 解交织
    bool deinterleave(const std::string& interleavedData, std::string& data, int rows = 8, int cols = 8);

private:
    // 私有构造函数(单例模式)
    CodingAPI();
    
    // 禁止拷贝和赋值
    CodingAPI(const CodingAPI&) = delete;
    CodingAPI& operator=(const CodingAPI&) = delete;
    
    // 初始化状态
    bool initialized;
};

// 全局函数
bool rsEncode(const std::string& data, std::string& encodedData, int codeLength = 31, int dataLength = 15);
bool rsDecode(const std::string& encodedData, std::string& data, int codeLength = 31, int dataLength = 15);
uint32_t calculateCRC32(const std::string& data);
bool verifyCRC32(const std::string& data, uint32_t crc);
bool aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext);
bool aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext);
bool interleave(const std::string& data, std::string& interleavedData, int rows = 8, int cols = 8);
bool deinterleave(const std::string& interleavedData, std::string& data, int rows = 8, int cols = 8);

} // namespace api
} // namespace link16
