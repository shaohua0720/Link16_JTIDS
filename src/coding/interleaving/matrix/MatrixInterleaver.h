#pragma once

#include <string>
#include <vector>
#include <memory>

namespace link16 {
namespace coding {
namespace interleaving {

/**
 * @brief 矩阵交织器类
 */
class MatrixInterleaver {
public:
    /**
     * @brief 构造函数
     * @param rows 行数
     * @param cols 列数
     */
    MatrixInterleaver(int rows = 8, int cols = 8);
    
    /**
     * @brief 析构函数
     */
    ~MatrixInterleaver();
    
    /**
     * @brief 交织函数
     * @param data 要交织的数据
     * @return 交织后的数据
     */
    std::string interleave(const std::string& data);
    
    /**
     * @brief 解交织函数
     * @param interleavedData 交织后的数据
     * @return 解交织后的数据
     */
    std::string deinterleave(const std::string& interleavedData);
    
    /**
     * @brief 设置交织参数
     * @param rows 行数
     * @param cols 列数
     */
    void setParameters(int rows, int cols);
    
    /**
     * @brief 获取行数
     * @return 行数
     */
    int getRows() const;
    
    /**
     * @brief 获取列数
     * @return 列数
     */
    int getCols() const;
    
    /**
     * @brief 获取交织大小
     * @return 交织大小，即行数乘以列数
     */
    int getInterleaverSize() const;
    
    /**
     * @brief 检查参数是否有效
     * @param rows 行数
     * @param cols 列数
     * @return 参数是否有效
     */
    static bool isValidParameters(int rows, int cols);
    
    /**
     * @brief 计算交织后的数据长度
     * @param dataLength 原始数据长度
     * @return 交织后的数据长度
     */
    int calculateInterleavedLength(int dataLength) const;
    
    /**
     * @brief 计算原始数据长度
     * @param interleavedLength 交织后的数据长度
     * @return 原始数据长度
     */
    int calculateOriginalLength(int interleavedLength) const;

private:
    // 行数
    int rows;
    
    // 列数
    int cols;
    
    // 内部实现
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

/**
 * @brief 全局交织函数
 * @param data 原始数据
 * @param rows 矩阵行数
 * @param cols 矩阵列数
 * @return 交织后的数据
 */
std::string interleave(const std::string& data, int rows, int cols);

/**
 * @brief 全局解交织函数
 * @param interleavedData 交织后的数据
 * @param rows 矩阵行数
 * @param cols 矩阵列数
 * @return 解交织后的数据
 */
std::string deinterleave(const std::string& interleavedData, int rows, int cols);

/**
 * @brief 全局交织函数(二进制版本)
 * @param data 原始二进制数据
 * @param rows 矩阵行数
 * @param cols 矩阵列数
 * @return 交织后的二进制数据
 */
std::vector<bool> interleave(const std::vector<bool>& data, int rows, int cols);

/**
 * @brief 全局解交织函数(二进制版本)
 * @param interleavedData 交织后的二进制数据
 * @param rows 矩阵行数
 * @param cols 矩阵列数
 * @return 解交织后的二进制数据
 */
std::vector<bool> deinterleave(const std::vector<bool>& interleavedData, int rows, int cols);

/**
 * @brief 全局交织函数(字节版本)
 * @param data 原始字节数据
 * @param rows 矩阵行数
 * @param cols 矩阵列数
 * @return 交织后的字节数据
 */
std::vector<uint8_t> interleave(const std::vector<uint8_t>& data, int rows, int cols);

/**
 * @brief 全局解交织函数(字节版本)
 * @param interleavedData 交织后的字节数据
 * @param rows 矩阵行数
 * @param cols 矩阵列数
 * @return 解交织后的字节数据
 */
std::vector<uint8_t> deinterleave(const std::vector<uint8_t>& interleavedData, int rows, int cols);

} // namespace interleaving
} // namespace coding
} // namespace link16
