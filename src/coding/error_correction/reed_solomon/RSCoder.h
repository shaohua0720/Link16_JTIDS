#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace link16 {
namespace coding {
namespace error_correction {

class RSCoder {
public:
    // 构造函数
    RSCoder(int codeLength = 31, int dataLength = 15);
    
    // 析构函数
    ~RSCoder();
    
    // 编码函数
    bool encode(const std::string& message, uint8_t* encodedData);
    
    // 解码函数
    bool decode(const uint8_t* encodedData, std::string& message);
    
    // 设置编码参数
    void setParameters(int codeLength, int dataLength);
    
    // 获取编码长度
    int getCodeLength() const;
    
    // 获取数据长度
    int getDataLength() const;
    
    // 获取纠错能力
    int getErrorCorrectionCapability() const;

private:
    // 编码长度
    int codeLength;
    
    // 数据长度
    int dataLength;
    
    // 纠错能力
    int errorCorrectionCapability;
};

// 全局RS编码函数
bool RS(int codeLength, int dataLength, const std::string& message, void* encodedData);

} // namespace error_correction
} // namespace coding
} // namespace link16
