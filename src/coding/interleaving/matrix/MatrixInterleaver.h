#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace link16 {
namespace coding {
namespace interleaving {

class MatrixInterleaver {
public:
    // 构造函数
    MatrixInterleaver(int rows = 8, int cols = 8);
    
    // 析构函数
    ~MatrixInterleaver();
    
    // 交织
    std::string interleave(const std::string& data);
    
    // 解交织
    std::string deinterleave(const std::string& interleavedData);
    
    // 设置交织参数
    void setParameters(int rows, int cols);
    
    // 获取行数
    int getRows() const;
    
    // 获取列数
    int getCols() const;

private:
    // 行数
    int rows;
    
    // 列数
    int cols;
};

// 全局交织函数
std::string interleave(const std::string& data, int rows, int cols);

// 全局解交织函数
std::string deinterleave(const std::string& interleavedData, int rows, int cols);

} // namespace interleaving
} // namespace coding
} // namespace link16
