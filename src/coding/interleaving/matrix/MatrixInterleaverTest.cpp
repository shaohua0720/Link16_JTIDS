#include "MatrixInterleaver.h"
#include "core/utils/logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace link16::coding::interleaving;

// 测试字符串交织
void testStringInterleaving() {
    std::cout << "测试字符串交织..." << std::endl;
    
    // 创建交织器
    MatrixInterleaver interleaver(4, 4);
    
    // 测试数据
    std::string data = "ABCDEFGHIJKLMNOP";
    
    // 交织
    std::string interleavedData = interleaver.interleave(data);
    
    // 打印结果
    std::cout << "原始数据: " << data << std::endl;
    std::cout << "交织后数据: " << interleavedData << std::endl;
    
    // 解交织
    std::string deinterleavedData = interleaver.deinterleave(interleavedData);
    
    // 打印结果
    std::cout << "解交织后数据: " << deinterleavedData << std::endl;
    
    // 验证结果
    assert(data == deinterleavedData);
    std::cout << "字符串交织测试通过!" << std::endl;
}

// 测试二进制交织
void testBinaryInterleaving() {
    std::cout << "测试二进制交织..." << std::endl;
    
    // 创建交织器
    MatrixInterleaver interleaver(4, 4);
    
    // 测试数据
    std::vector<bool> data = {
        true, false, true, false, true, false, true, false,
        true, false, true, false, true, false, true, false
    };
    
    // 交织
    std::vector<bool> interleavedData = interleaver.interleave(data);
    
    // 打印结果
    std::cout << "原始数据: ";
    for (bool bit : data) {
        std::cout << (bit ? "1" : "0");
    }
    std::cout << std::endl;
    
    std::cout << "交织后数据: ";
    for (bool bit : interleavedData) {
        std::cout << (bit ? "1" : "0");
    }
    std::cout << std::endl;
    
    // 解交织
    std::vector<bool> deinterleavedData = interleaver.deinterleave(interleavedData);
    
    // 打印结果
    std::cout << "解交织后数据: ";
    for (bool bit : deinterleavedData) {
        std::cout << (bit ? "1" : "0");
    }
    std::cout << std::endl;
    
    // 验证结果
    assert(data == deinterleavedData);
    std::cout << "二进制交织测试通过!" << std::endl;
}

// 测试字节交织
void testByteInterleaving() {
    std::cout << "测试字节交织..." << std::endl;
    
    // 创建交织器
    MatrixInterleaver interleaver(4, 4);
    
    // 测试数据
    std::vector<uint8_t> data = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
    };
    
    // 交织
    std::vector<uint8_t> interleavedData = interleaver.interleave(data);
    
    // 打印结果
    std::cout << "原始数据: ";
    for (uint8_t byte : data) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
    
    std::cout << "交织后数据: ";
    for (uint8_t byte : interleavedData) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
    
    // 解交织
    std::vector<uint8_t> deinterleavedData = interleaver.deinterleave(interleavedData);
    
    // 打印结果
    std::cout << "解交织后数据: ";
    for (uint8_t byte : deinterleavedData) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
    
    // 验证结果
    assert(data == deinterleavedData);
    std::cout << "字节交织测试通过!" << std::endl;
}

// 测试不同大小的矩阵
void testDifferentMatrixSizes() {
    std::cout << "测试不同大小的矩阵..." << std::endl;
    
    // 测试数据
    std::string data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    // 测试不同大小的矩阵
    for (int rows = 2; rows <= 5; ++rows) {
        for (int cols = 2; cols <= 5; ++cols) {
            // 创建交织器
            MatrixInterleaver interleaver(rows, cols);
            
            // 交织
            std::string interleavedData = interleaver.interleave(data);
            
            // 解交织
            std::string deinterleavedData = interleaver.deinterleave(interleavedData);
            
            // 验证结果
            assert(data.substr(0, rows * cols * (data.length() / (rows * cols))) == 
                   deinterleavedData.substr(0, rows * cols * (data.length() / (rows * cols))));
            
            std::cout << "矩阵大小 " << rows << "x" << cols << " 测试通过!" << std::endl;
        }
    }
}

// 测试边界情况
void testEdgeCases() {
    std::cout << "测试边界情况..." << std::endl;
    
    // 创建交织器
    MatrixInterleaver interleaver(4, 4);
    
    // 测试空数据
    std::string emptyData = "";
    std::string interleavedEmptyData = interleaver.interleave(emptyData);
    std::string deinterleavedEmptyData = interleaver.deinterleave(interleavedEmptyData);
    assert(emptyData == deinterleavedEmptyData);
    std::cout << "空数据测试通过!" << std::endl;
    
    // 测试小于矩阵大小的数据
    std::string smallData = "ABC";
    std::string interleavedSmallData = interleaver.interleave(smallData);
    std::string deinterleavedSmallData = interleaver.deinterleave(interleavedSmallData);
    // 注意：由于填充，解交织后的数据可能比原始数据长
    assert(deinterleavedSmallData.substr(0, smallData.length()) == smallData);
    std::cout << "小数据测试通过!" << std::endl;
    
    // 测试无效的矩阵大小
    MatrixInterleaver invalidInterleaver(0, 0);
    assert(invalidInterleaver.getRows() == 8);  // 应该使用默认值
    assert(invalidInterleaver.getCols() == 8);  // 应该使用默认值
    std::cout << "无效矩阵大小测试通过!" << std::endl;
}

// 主函数
int main() {
    // 初始化日志
    // link16::core::utils::Logger::getInstance().initialize();
    
    // 运行测试
    testStringInterleaving();
    testBinaryInterleaving();
    testByteInterleaving();
    testDifferentMatrixSizes();
    testEdgeCases();
    
    std::cout << "所有测试通过!" << std::endl;
    
    return 0;
}
