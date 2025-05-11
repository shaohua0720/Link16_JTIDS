#include "AESCrypto.h"
#include "AES.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <vector>

namespace link16 {
namespace coding {
namespace crypto {

// 私有实现类
class AESCrypto::Impl {
public:
    Impl(int keySize) : keySize(keySize) {
        // 根据密钥大小选择AES密钥长度
        switch (keySize) {
            case 128:
                aesKeyLength = AESKeyLength::AES_128;
                break;
            case 192:
                aesKeyLength = AESKeyLength::AES_192;
                break;
            case 256:
            default:
                aesKeyLength = AESKeyLength::AES_256;
                keySize = 256;
                break;
        }
        
        // 创建AES实例
        aes = new AES(aesKeyLength);
    }
    
    ~Impl() {
        delete aes;
    }
    
    // 字符串转换为字节向量
    std::vector<unsigned char> stringToVector(const std::string& str) {
        std::vector<unsigned char> vec(str.begin(), str.end());
        return vec;
    }
    
    // 字节向量转换为字符串
    std::string vectorToString(const std::vector<unsigned char>& vec) {
        return std::string(vec.begin(), vec.end());
    }
    
    // 十六进制字符串转换为字节向量
    std::vector<unsigned char> hexToVector(const std::string& hex) {
        std::vector<unsigned char> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);
            bytes.push_back(byte);
        }
        return bytes;
    }
    
    // 字节向量转换为十六进制字符串
    std::string vectorToHex(const std::vector<unsigned char>& vec) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (unsigned char b : vec) {
            ss << std::setw(2) << static_cast<int>(b);
        }
        return ss.str();
    }
    
    // 加密函数
    std::string encrypt(const std::string& plaintext, const std::string& key) {
        // 将密钥转换为向量
        std::vector<unsigned char> keyVec;
        if (key.length() * 8 >= keySize) {
            // 如果密钥长度足够，直接使用
            keyVec = stringToVector(key.substr(0, keySize / 8));
        } else {
            // 如果密钥长度不够，填充到所需长度
            keyVec = stringToVector(key);
            while (keyVec.size() * 8 < keySize) {
                keyVec.push_back(0);
            }
        }
        
        // 将明文转换为向量
        std::vector<unsigned char> plaintextVec = stringToVector(plaintext);
        
        // 填充明文到16字节的倍数
        size_t paddingSize = 16 - (plaintextVec.size() % 16);
        for (size_t i = 0; i < paddingSize; ++i) {
            plaintextVec.push_back(static_cast<unsigned char>(paddingSize));
        }
        
        // 使用AES加密
        std::vector<unsigned char> ciphertextVec = aes->EncryptECB(plaintextVec, keyVec);
        
        // 将密文转换为十六进制字符串
        return vectorToHex(ciphertextVec);
    }
    
    // 解密函数
    std::string decrypt(const std::string& ciphertext, const std::string& key) {
        // 将密钥转换为向量
        std::vector<unsigned char> keyVec;
        if (key.length() * 8 >= keySize) {
            // 如果密钥长度足够，直接使用
            keyVec = stringToVector(key.substr(0, keySize / 8));
        } else {
            // 如果密钥长度不够，填充到所需长度
            keyVec = stringToVector(key);
            while (keyVec.size() * 8 < keySize) {
                keyVec.push_back(0);
            }
        }
        
        // 将十六进制密文转换为向量
        std::vector<unsigned char> ciphertextVec = hexToVector(ciphertext);
        
        // 使用AES解密
        std::vector<unsigned char> plaintextVec = aes->DecryptECB(ciphertextVec, keyVec);
        
        // 移除填充
        if (!plaintextVec.empty()) {
            unsigned char paddingSize = plaintextVec.back();
            if (paddingSize > 0 && paddingSize <= 16) {
                // 验证填充
                bool validPadding = true;
                for (size_t i = 1; i <= paddingSize && i <= plaintextVec.size(); ++i) {
                    if (plaintextVec[plaintextVec.size() - i] != paddingSize) {
                        validPadding = false;
                        break;
                    }
                }
                
                if (validPadding) {
                    plaintextVec.resize(plaintextVec.size() - paddingSize);
                }
            }
        }
        
        // 将明文向量转换为字符串
        return vectorToString(plaintextVec);
    }
    
    // 生成随机密钥
    std::string generateRandomKey() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        std::vector<unsigned char> keyVec;
        for (int i = 0; i < keySize / 8; ++i) {
            keyVec.push_back(static_cast<unsigned char>(dis(gen)));
        }
        
        return vectorToHex(keyVec);
    }
    
    // 检查密钥是否有效
    bool isValidKey(const std::string& key) {
        // 检查密钥长度
        if (key.empty()) {
            return false;
        }
        
        // 如果是十六进制字符串，检查是否为有效的十六进制
        for (char c : key) {
            if (!isxdigit(c)) {
                return false;
            }
        }
        
        // 检查密钥长度是否足够
        return (key.length() * 4 >= keySize);
    }
    
private:
    int keySize;
    AESKeyLength aesKeyLength;
    AES* aes;
};

// AESCrypto实现

AESCrypto::AESCrypto(int keySize) : keySize(keySize), pImpl(new Impl(keySize)) {
}

AESCrypto::~AESCrypto() {
}

std::string AESCrypto::encrypt(const std::string& plaintext, const std::string& key) {
    return pImpl->encrypt(plaintext, key);
}

std::string AESCrypto::decrypt(const std::string& ciphertext, const std::string& key) {
    return pImpl->decrypt(ciphertext, key);
}

void AESCrypto::setKey(const std::string& key) {
    this->key = key;
}

const std::string& AESCrypto::getKey() const {
    return key;
}

void AESCrypto::setKeySize(int keySize) {
    if (keySize != this->keySize) {
        this->keySize = keySize;
        pImpl.reset(new Impl(keySize));
    }
}

int AESCrypto::getKeySize() const {
    return keySize;
}

std::string AESCrypto::generateRandomKey() {
    return pImpl->generateRandomKey();
}

bool AESCrypto::isValidKey(const std::string& key) {
    return pImpl->isValidKey(key);
}

} // namespace crypto
} // namespace coding
} // namespace link16