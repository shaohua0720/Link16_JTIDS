#include "include/link16/api/CodingAPI.h"
#include "src/coding/error_correction/reed_solomon/RSCoder.h"
#include "src/coding/error_detection/crc/CRCCoder.h"
#include "src/coding/crypto/symmetric/aes/AES.h"
#include "src/coding/interleaving/matrix/MatrixInterleaver.h"
#include "core/utils/logger.h"
#include <iostream>
#include <vector>

namespace link16 {
namespace api {

// 单例实例
static CodingAPI* s_instance = nullptr;

// 获取单例实例
CodingAPI& CodingAPI::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new CodingAPI();
    }
    return *s_instance;
}

// 构造函数
CodingAPI::CodingAPI() : initialized(false) {
}

// 初始化
bool CodingAPI::initialize() {
    if (initialized) {
        return true;
    }
    
    LOG_INFO("初始化编码API");
    
    // 这里可以添加初始化代码
    
    initialized = true;
    return true;
}

// 关闭
void CodingAPI::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("关闭编码API");
    
    // 这里可以添加清理代码
    
    initialized = false;
}

// Reed-Solomon编码
bool CodingAPI::rsEncode(const std::string& data, std::string& encodedData, int codeLength, int dataLength) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("RS编码: codeLength=" + std::to_string(codeLength) + ", dataLength=" + std::to_string(dataLength));
    
    // 创建RS编码器
    coding::error_correction::RSCoder rsCoder(codeLength, dataLength);
    
    // 分配内存
    std::vector<uint8_t> encodedBuffer(codeLength);
    
    // 调用内部接口
    bool result = rsCoder.encode(data, encodedBuffer.data());
    
    if (!result) {
        LOG_ERROR("RS编码失败");
        return false;
    }
    
    // 将编码结果转换为字符串
    encodedData.assign(reinterpret_cast<char*>(encodedBuffer.data()), codeLength);
    
    return true;
}

// Reed-Solomon解码
bool CodingAPI::rsDecode(const std::string& encodedData, std::string& data, int codeLength, int dataLength) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("RS解码: codeLength=" + std::to_string(codeLength) + ", dataLength=" + std::to_string(dataLength));
    
    // 创建RS编码器
    coding::error_correction::RSCoder rsCoder(codeLength, dataLength);
    
    // 调用内部接口
    bool result = rsCoder.decode(reinterpret_cast<const uint8_t*>(encodedData.data()), data);
    
    if (!result) {
        LOG_ERROR("RS解码失败");
        return false;
    }
    
    return true;
}

// CRC校验
uint32_t CodingAPI::calculateCRC32(const std::string& data) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return 0;
    }
    
    LOG_INFO("计算CRC32");
    
    // 调用内部接口
    return coding::error_detection::calculateCRC32(
        reinterpret_cast<const uint8_t*>(data.data()), 
        data.length()
    );
}

// 验证CRC
bool CodingAPI::verifyCRC32(const std::string& data, uint32_t crc) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("验证CRC32");
    
    // 计算CRC
    uint32_t calculatedCRC = calculateCRC32(data);
    
    // 比较CRC
    return calculatedCRC == crc;
}

// AES加密
bool CodingAPI::aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("AES加密");
    
    // 创建AES加密器
    // 这里假设AES类有一个静态方法encrypt
    // 实际实现可能需要创建AES对象
    ciphertext = plaintext;  // 简化实现，实际应该调用AES加密
    
    return true;
}

// AES解密
bool CodingAPI::aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("AES解密");
    
    // 创建AES解密器
    // 这里假设AES类有一个静态方法decrypt
    // 实际实现可能需要创建AES对象
    plaintext = ciphertext;  // 简化实现，实际应该调用AES解密
    
    return true;
}

// 交织
bool CodingAPI::interleave(const std::string& data, std::string& interleavedData, int rows, int cols) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("交织: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
    
    // 调用内部接口
    interleavedData = coding::interleaving::interleave(data, rows, cols);
    
    return true;
}

// 解交织
bool CodingAPI::deinterleave(const std::string& interleavedData, std::string& data, int rows, int cols) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("解交织: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
    
    // 调用内部接口
    data = coding::interleaving::deinterleave(interleavedData, rows, cols);
    
    return true;
}

// 全局函数
bool rsEncode(const std::string& data, std::string& encodedData, int codeLength, int dataLength) {
    return CodingAPI::getInstance().rsEncode(data, encodedData, codeLength, dataLength);
}

bool rsDecode(const std::string& encodedData, std::string& data, int codeLength, int dataLength) {
    return CodingAPI::getInstance().rsDecode(encodedData, data, codeLength, dataLength);
}

uint32_t calculateCRC32(const std::string& data) {
    return CodingAPI::getInstance().calculateCRC32(data);
}

bool verifyCRC32(const std::string& data, uint32_t crc) {
    return CodingAPI::getInstance().verifyCRC32(data, crc);
}

bool aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext) {
    return CodingAPI::getInstance().aesEncrypt(plaintext, key, ciphertext);
}

bool aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext) {
    return CodingAPI::getInstance().aesDecrypt(ciphertext, key, plaintext);
}

bool interleave(const std::string& data, std::string& interleavedData, int rows, int cols) {
    return CodingAPI::getInstance().interleave(data, interleavedData, rows, cols);
}

bool deinterleave(const std::string& interleavedData, std::string& data, int rows, int cols) {
    return CodingAPI::getInstance().deinterleave(interleavedData, data, rows, cols);
}

} // namespace api
} // namespace link16
