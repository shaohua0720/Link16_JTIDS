#include "RSCoder.h"
#include "core/utils/logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>
#include <random>

using namespace link16::coding::error_correction;

// 测试基本编解码功能
void testBasicCoding() {
    std::cout << "测试基本编解码功能..." << std::endl;
    
    // 创建RS编码器
    RSCoder coder(31, 15);
    
    // 测试数据
    std::string message = "Hello, World!";
    
    // 编码
    uint8_t encodedData[31];
    bool encodeResult = coder.encode(message, encodedData);
    
    // 验证编码结果
    assert(encodeResult);
    
    // 解码
    std::string decodedMessage;
    bool decodeResult = coder.decode(encodedData, decodedMessage);
    
    // 验证解码结果
    assert(decodeResult);
    assert(message == decodedMessage);
    
    std::cout << "原始消息: " << message << std::endl;
    std::cout << "解码消息: " << decodedMessage << std::endl;
    
    std::cout << "基本编解码测试通过!" << std::endl;
}

// 测试错误纠正功能
void testErrorCorrection() {
    std::cout << "测试错误纠正功能..." << std::endl;
    
    // 创建RS编码器
    RSCoder coder(31, 15);
    
    // 测试数据
    std::string message = "Hello, World!";
    
    // 编码
    uint8_t encodedData[31];
    bool encodeResult = coder.encode(message, encodedData);
    assert(encodeResult);
    
    // 引入错误
    int errorCount = coder.getErrorCorrectionCapability();
    std::cout << "引入 " << errorCount << " 个错误..." << std::endl;
    
    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 30);
    std::uniform_int_distribution<> valueDis(0, 255);
    
    // 记录原始数据
    uint8_t originalData[31];
    std::memcpy(originalData, encodedData, 31);
    
    // 引入随机错误
    for (int i = 0; i < errorCount; ++i) {
        int position = dis(gen);
        uint8_t originalValue = encodedData[position];
        uint8_t newValue;
        do {
            newValue = static_cast<uint8_t>(valueDis(gen));
        } while (newValue == originalValue);
        
        encodedData[position] = newValue;
        std::cout << "位置 " << position << ": " << static_cast<int>(originalValue) 
                  << " -> " << static_cast<int>(newValue) << std::endl;
    }
    
    // 解码
    std::string decodedMessage;
    bool decodeResult = coder.decode(encodedData, decodedMessage);
    
    // 验证解码结果
    assert(decodeResult);
    assert(message == decodedMessage);
    
    std::cout << "原始消息: " << message << std::endl;
    std::cout << "解码消息: " << decodedMessage << std::endl;
    
    std::cout << "错误纠正测试通过!" << std::endl;
}

// 测试不同参数
void testDifferentParameters() {
    std::cout << "测试不同参数..." << std::endl;
    
    // 测试不同的参数组合
    struct TestCase {
        int codeLength;
        int dataLength;
        std::string message;
    };
    
    TestCase testCases[] = {
        {31, 15, "Hello, World!"},
        {31, 23, "Hello, World!"},
        {31, 27, "Hello, World!"},
        {63, 31, "This is a longer message for testing Reed-Solomon coding."},
        {63, 47, "This is a longer message for testing Reed-Solomon coding."},
        {63, 55, "This is a longer message for testing Reed-Solomon coding."}
    };
    
    for (const auto& testCase : testCases) {
        std::cout << "测试参数: codeLength=" << testCase.codeLength 
                  << ", dataLength=" << testCase.dataLength << std::endl;
        
        // 创建RS编码器
        RSCoder coder(testCase.codeLength, testCase.dataLength);
        
        // 编码
        std::vector<uint8_t> encodedData(testCase.codeLength);
        bool encodeResult = coder.encode(testCase.message, encodedData.data());
        
        // 验证编码结果
        assert(encodeResult);
        
        // 解码
        std::string decodedMessage;
        bool decodeResult = coder.decode(encodedData.data(), decodedMessage);
        
        // 验证解码结果
        assert(decodeResult);
        assert(testCase.message == decodedMessage);
        
        std::cout << "参数测试通过: codeLength=" << testCase.codeLength 
                  << ", dataLength=" << testCase.dataLength << std::endl;
    }
    
    std::cout << "不同参数测试通过!" << std::endl;
}

// 测试边界情况
void testEdgeCases() {
    std::cout << "测试边界情况..." << std::endl;
    
    // 测试空消息
    {
        RSCoder coder(31, 15);
        std::string message = "";
        uint8_t encodedData[31];
        bool encodeResult = coder.encode(message, encodedData);
        assert(!encodeResult);  // 应该失败，因为消息为空
    }
    
    // 测试无效参数
    {
        RSCoder coder(0, 0);  // 应该使用默认值
        assert(coder.getCodeLength() == 31);
        assert(coder.getDataLength() == 15);
    }
    
    // 测试参数设置
    {
        RSCoder coder;
        coder.setParameters(63, 47);
        assert(coder.getCodeLength() == 63);
        assert(coder.getDataLength() == 47);
        assert(coder.getErrorCorrectionCapability() == 8);
    }
    
    std::cout << "边界情况测试通过!" << std::endl;
}

// 测试全局RS函数
void testGlobalRSFunction() {
    std::cout << "测试全局RS函数..." << std::endl;
    
    // 测试数据
    std::string message = "Hello, World!";
    
    // 编码
    uint8_t encodedData[31];
    bool encodeResult = RS(31, 15, message, encodedData);
    
    // 验证编码结果
    assert(encodeResult);
    
    // 创建RS编码器进行解码
    RSCoder coder(31, 15);
    std::string decodedMessage;
    bool decodeResult = coder.decode(encodedData, decodedMessage);
    
    // 验证解码结果
    assert(decodeResult);
    assert(message == decodedMessage);
    
    std::cout << "全局RS函数测试通过!" << std::endl;
}

// 主函数
int main() {
    // 初始化日志
    // link16::core::utils::Logger::getInstance().initialize();
    
    // 运行测试
    testBasicCoding();
    testErrorCorrection();
    testDifferentParameters();
    testEdgeCases();
    testGlobalRSFunction();
    
    std::cout << "所有测试通过!" << std::endl;
    
    return 0;
}
