#include "include/link16/api/CodingAPI.h"
#include "src/coding/CodingProcessor.h"
#include "core/utils/logger.h"
#include <iostream>
#include <memory>

namespace link16 {
namespace api {

// 单例实例
static CodingAPI* s_instance = nullptr;
// CodingProcessor实例
static std::unique_ptr<coding::CodingProcessor> s_processor = nullptr;

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
    
    // 初始化CodingProcessor
    s_processor = std::make_unique<coding::CodingProcessor>();
    if (!s_processor->initialize()) {
        LOG_ERROR("CodingProcessor初始化失败");
        return false;
    }
    
    initialized = true;
    return true;
}

// 关闭
void CodingAPI::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("关闭编码API");
    
    // 关闭CodingProcessor
    if (s_processor) {
        s_processor->shutdown();
        s_processor.reset();
    }
    
    initialized = false;
}

// Reed-Solomon编码
bool CodingAPI::rsEncode(const std::string& data, std::string& encodedData, int codeLength, int dataLength) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("RS编码: codeLength=" + std::to_string(codeLength) + ", dataLength=" + std::to_string(dataLength));
    
    // 调用CodingProcessor
    bool result = s_processor->rsEncode(data, encodedData, codeLength, dataLength);
    
    if (!result) {
        LOG_ERROR("RS编码失败");
    }
    
    return result;
}

// Reed-Solomon解码
bool CodingAPI::rsDecode(const std::string& encodedData, std::string& data, int codeLength, int dataLength) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("RS解码: codeLength=" + std::to_string(codeLength) + ", dataLength=" + std::to_string(dataLength));
    
    // 调用CodingProcessor
    bool result = s_processor->rsDecode(encodedData, data, codeLength, dataLength);
    
    if (!result) {
        LOG_ERROR("RS解码失败");
    }
    
    return result;
}

// CRC校验
uint32_t CodingAPI::calculateCRC32(const std::string& data) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return 0;
    }
    
    LOG_INFO("计算CRC32");
    
    // CRC功能在CodingProcessor中不存在，需要直接调用底层
    // 这里可以考虑将CRC功能也添加到CodingProcessor中
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
    
    // 调用CodingProcessor
    bool result = s_processor->aesEncrypt(plaintext, key, ciphertext);
    
    if (!result) {
        LOG_ERROR("AES加密失败");
    }
    
    return result;
}

// AES解密
bool CodingAPI::aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("AES解密");
    
    // 调用CodingProcessor
    bool result = s_processor->aesDecrypt(ciphertext, key, plaintext);
    
    if (!result) {
        LOG_ERROR("AES解密失败");
    }
    
    return result;
}

// 生成AES随机密钥
std::string CodingAPI::generateAESKey() {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return "";
    }
    
    LOG_INFO("生成AES随机密钥");
    
    // 这个功能在CodingProcessor中不存在，需要添加或直接调用底层
    // 这里假设我们添加了这个功能到CodingProcessor
    return s_processor->generateAESKey();
}

// 设置AES密钥大小
bool CodingAPI::setAESKeySize(int keySize) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("设置AES密钥大小: " + std::to_string(keySize));
    
    // 这个功能在CodingProcessor中不存在，需要添加或直接调用底层
    // 这里假设我们添加了这个功能到CodingProcessor
    return s_processor->setAESKeySize(keySize);
}

// 获取AES密钥大小
int CodingAPI::getAESKeySize() {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return 0;
    }
    
    // 这个功能在CodingProcessor中不存在，需要添加或直接调用底层
    // 这里假设我们添加了这个功能到CodingProcessor
    return s_processor->getAESKeySize();
}

// 交织
bool CodingAPI::interleave(const std::string& data, std::string& interleavedData, int rows, int cols) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("交织: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
    
    // 调用CodingProcessor
    bool result = s_processor->interleave(data, interleavedData, rows, cols);
    
    if (!result) {
        LOG_ERROR("交织失败");
    }
    
    return result;
}

// 解交织
bool CodingAPI::deinterleave(const std::string& interleavedData, std::string& data, int rows, int cols) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("解交织: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
    
    // 调用CodingProcessor
    bool result = s_processor->deinterleave(interleavedData, data, rows, cols);
    
    if (!result) {
        LOG_ERROR("解交织失败");
    }
    
    return result;
}

// 计算BIP校验位
std::string CodingAPI::calculateBIP(const std::string& data) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return "";
    }
    
    LOG_INFO("计算BIP校验位");
    
    // CodingProcessor使用vector<bool>返回BIP，这里需要转换
    std::vector<bool> bip;
    if (!s_processor->calculateBIP(data, bip)) {
        LOG_ERROR("计算BIP失败");
        return "";
    }
    
    // 转换为字符串
    std::string bipStr;
    for (bool bit : bip) {
        bipStr += (bit ? '1' : '0');
    }
    
    return bipStr;
}

// 验证BIP校验位
bool CodingAPI::verifyBIP(const std::string& data, const std::string& bipStr) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return false;
    }
    
    LOG_INFO("验证BIP校验位");
    
    // 将字符串转换为vector<bool>
    std::vector<bool> bip;
    for (char c : bipStr) {
        bip.push_back(c == '1');
    }
    
    // 调用CodingProcessor
    return s_processor->verifyBIP(data, bip);
}

// 添加BIP校验位
std::string CodingAPI::addBIP(const std::string& data) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return data;
    }
    
    LOG_INFO("添加BIP校验位");
    
    // 这个功能在CodingProcessor中不存在，需要添加或组合现有功能
    // 这里我们使用现有功能组合实现
    std::vector<bool> bip;
    if (!s_processor->calculateBIP(data, bip)) {
        LOG_ERROR("计算BIP失败");
        return data;
    }
    
    // 将BIP添加到数据末尾
    // 这里简化处理，实际可能需要更复杂的逻辑
    std::string result = data;
    for (bool bit : bip) {
        result += (bit ? '1' : '0');
    }
    
    return result;
}

// 从带BIP的数据中提取原始数据
std::string CodingAPI::extractDataFromBIP(const std::string& dataWithBIP) {
    if (!initialized) {
        LOG_ERROR("编码API未初始化");
        return dataWithBIP;
    }
    
    LOG_INFO("从带BIP的数据中提取原始数据");
    
    // 这个功能在CodingProcessor中不存在，需要添加或组合现有功能
    // 这里简化处理，假设BIP位于数据末尾
    if (dataWithBIP.length() < 5) {
        LOG_ERROR("数据长度不足");
        return dataWithBIP;
    }
    
    // 提取数据部分
    return dataWithBIP.substr(0, dataWithBIP.length() - 5);
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

std::string generateAESKey() {
    return CodingAPI::getInstance().generateAESKey();
}

bool setAESKeySize(int keySize) {
    return CodingAPI::getInstance().setAESKeySize(keySize);
}

int getAESKeySize() {
    return CodingAPI::getInstance().getAESKeySize();
}

bool interleave(const std::string& data, std::string& interleavedData, int rows, int cols) {
    return CodingAPI::getInstance().interleave(data, interleavedData, rows, cols);
}

bool deinterleave(const std::string& interleavedData, std::string& data, int rows, int cols) {
    return CodingAPI::getInstance().deinterleave(interleavedData, data, rows, cols);
}

std::string calculateBIP(const std::string& data) {
    return CodingAPI::getInstance().calculateBIP(data);
}

bool verifyBIP(const std::string& data, const std::string& bipStr) {
    return CodingAPI::getInstance().verifyBIP(data, bipStr);
}

std::string addBIP(const std::string& data) {
    return CodingAPI::getInstance().addBIP(data);
}

std::string extractDataFromBIP(const std::string& dataWithBIP) {
    return CodingAPI::getInstance().extractDataFromBIP(dataWithBIP);
}

} // namespace api
} // namespace link16
