#include "CodingProcessor.h"
#include "error_correction/reed_solomon/RSCoder.h"
#include "crypto/symmetric/aes/AESCrypto.h"
#include "interleaving/matrix/MatrixInterleaver.h"
#include "error_detection/parity/BIPCoder.h"
#include "core/utils/logger.h"

namespace link16 {
namespace coding {

// 构造函数
CodingProcessor::CodingProcessor() : initialized(false) {
}

// 析构函数
CodingProcessor::~CodingProcessor() {
    shutdown();
}

// 初始化
bool CodingProcessor::initialize() {
    if (initialized) {
        return true;
    }
    
    try {
        // 创建Reed-Solomon编码器
        rsEncoder = std::make_unique<error_correction::RSCoder>();
        
        // 创建AES加密器
        aesEncoder = std::make_unique<crypto::AESCrypto>();
        
        // 创建交织器
        interleaver = std::make_unique<interleaving::MatrixInterleaver>();
        
        // 创建BIP校验器
        bipCoder = std::make_unique<error_detection::BIPCoder>();
        
        initialized = true;
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("CodingProcessor初始化失败: " + std::string(e.what()));
        return false;
    }
}

// 关闭
void CodingProcessor::shutdown() {
    if (!initialized) {
        return;
    }
    
    // 释放资源
    rsEncoder.reset();
    aesEncoder.reset();
    interleaver.reset();
    bipCoder.reset();
    
    initialized = false;
}

// 编码数据
bool CodingProcessor::encodeData(const std::string& rawData, std::string& encodedData) {
    if (!initialized) {
        return false;
    }
    
    try {
        std::string processedData = rawData;
        std::string tempData;
        
        // 1. 计算BIP校验
        std::vector<bool> bip;
        if (!calculateBIP(processedData, bip)) {
            return false;
        }
        
        // 2. Reed-Solomon编码
        if (!rsEncode(processedData, tempData)) {
            return false;
        }
        processedData = tempData;
        
        // 3. 加密（如果有密钥）
        if (!encryptionKey.empty()) {
            if (!aesEncrypt(processedData, encryptionKey, tempData)) {
                return false;
            }
            processedData = tempData;
        }
        
        // 4. 交织
        if (!interleave(processedData, encodedData)) {
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("数据编码失败: " + std::string(e.what()));
        return false;
    }
}

// 解码数据
bool CodingProcessor::decodeData(const std::string& encodedData, std::string& rawData) {
    if (!initialized) {
        return false;
    }
    
    try {
        std::string processedData = encodedData;
        std::string tempData;
        
        // 1. 解交织
        if (!deinterleave(processedData, tempData)) {
            return false;
        }
        processedData = tempData;
        
        // 2. 解密（如果有密钥）
        if (!encryptionKey.empty()) {
            if (!aesDecrypt(processedData, encryptionKey, tempData)) {
                return false;
            }
            processedData = tempData;
        }
        
        // 3. Reed-Solomon解码
        if (!rsDecode(processedData, tempData)) {
            return false;
        }
        processedData = tempData;
        
        // 4. 验证BIP校验
        std::vector<bool> bip;
        if (!calculateBIP(processedData, bip)) {
            return false;
        }
        
        rawData = processedData;
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("数据解码失败: " + std::string(e.what()));
        return false;
    }
}

// Reed-Solomon编码
bool CodingProcessor::rsEncode(const std::string& data, std::string& encodedData, int codeLength, int dataLength) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 设置RS编码参数
        rsEncoder->setParameters(codeLength, dataLength);
        
        // 执行编码
        encodedData = rsEncoder->encode(data);
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("RS编码失败: " + std::string(e.what()));
        return false;
    }
}

// Reed-Solomon解码
bool CodingProcessor::rsDecode(const std::string& encodedData, std::string& data, int codeLength, int dataLength) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 设置RS编码参数
        rsEncoder->setParameters(codeLength, dataLength);
        
        // 执行解码
        data = rsEncoder->decode(encodedData);
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("RS解码失败: " + std::string(e.what()));
        return false;
    }
}

// AES加密
bool CodingProcessor::aesEncrypt(const std::string& plaintext, const std::string& key, std::string& ciphertext) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 执行加密
        ciphertext = aesEncoder->encrypt(plaintext, key);
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("AES加密失败: " + std::string(e.what()));
        return false;
    }
}

// AES解密
bool CodingProcessor::aesDecrypt(const std::string& ciphertext, const std::string& key, std::string& plaintext) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 执行解密
        plaintext = aesEncoder->decrypt(ciphertext, key);
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("AES解密失败: " + std::string(e.what()));
        return false;
    }
}

// 交织
bool CodingProcessor::interleave(const std::string& data, std::string& interleavedData, int rows, int cols) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 设置交织参数
        dynamic_cast<interleaving::MatrixInterleaver*>(interleaver.get())->setDimensions(rows, cols);
        
        // 执行交织
        interleavedData = interleaver->interleave(data);
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("数据交织失败: " + std::string(e.what()));
        return false;
    }
}

// 解交织
bool CodingProcessor::deinterleave(const std::string& interleavedData, std::string& data, int rows, int cols) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 设置交织参数
        dynamic_cast<interleaving::MatrixInterleaver*>(interleaver.get())->setDimensions(rows, cols);
        
        // 执行解交织
        data = interleaver->deinterleave(interleavedData);
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("数据解交织失败: " + std::string(e.what()));
        return false;
    }
}

// BIP校验计算
bool CodingProcessor::calculateBIP(const std::string& data, std::vector<bool>& bip) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 计算BIP校验位
        std::bitset<5> bipBits = bipCoder->calculateBIP(data);
        
        // 转换为布尔向量
        bip.resize(5);
        for (int i = 0; i < 5; ++i) {
            bip[i] = bipBits[i];
        }
        
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("BIP校验计算失败: " + std::string(e.what()));
        return false;
    }
}

// BIP校验验证
bool CodingProcessor::verifyBIP(const std::string& data, const std::vector<bool>& bip) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 转换为bitset
        std::bitset<5> bipBits;
        for (int i = 0; i < 5 && i < bip.size(); ++i) {
            bipBits[i] = bip[i];
        }
        
        // 验证BIP校验
        return bipCoder->verifyBIP(data, bipBits);
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("BIP校验验证失败: " + std::string(e.what()));
        return false;
    }
}

// 设置加密密钥
void CodingProcessor::setEncryptionKey(const std::string& key) {
    encryptionKey = key;
}

// 获取加密密钥
const std::string& CodingProcessor::getEncryptionKey() const {
    return encryptionKey;
}

// 生成AES随机密钥
std::string CodingProcessor::generateAESKey() {
    if (!initialized) {
        return "";
    }
    
    try {
        // 生成随机密钥
        return aesEncoder->generateRandomKey();
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("生成AES随机密钥失败: " + std::string(e.what()));
        return "";
    }
}

// 设置AES密钥大小
bool CodingProcessor::setAESKeySize(int keySize) {
    if (!initialized) {
        return false;
    }
    
    try {
        // 验证密钥大小
        if (keySize != 128 && keySize != 192 && keySize != 256) {
            return false;
        }
        
        // 设置密钥大小
        aesEncoder->setKeySize(keySize);
        return true;
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("设置AES密钥大小失败: " + std::string(e.what()));
        return false;
    }
}

// 获取AES密钥大小
int CodingProcessor::getAESKeySize() {
    if (!initialized) {
        return 0;
    }
    
    try {
        // 获取密钥大小
        return aesEncoder->getKeySize();
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("获取AES密钥大小失败: " + std::string(e.what()));
        return 0;
    }
}

// CRC校验计算
uint32_t CodingProcessor::calculateCRC32(const std::string& data) {
    if (!initialized) {
        return 0;
    }
    
    try {
        // 计算CRC
        return error_detection::calculateCRC32(
            reinterpret_cast<const uint8_t*>(data.data()), 
            data.length()
        );
    } catch (const std::exception& e) {
        // 记录错误
        // Logger::error("计算CRC32失败: " + std::string(e.what()));
        return 0;
    }
}

} // namespace coding
} // namespace link16
