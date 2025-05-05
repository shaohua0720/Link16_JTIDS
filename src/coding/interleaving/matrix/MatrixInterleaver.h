#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace link16 {
namespace coding {
namespace interleaving {

/**
 * @brief 矩阵交织器类
 *
 * 矩阵交织器是一种常用的交织方法，它将数据按行填入矩阵，然后按列读出，
 * 从而实现数据的交织，增强抗干扰能力。
 */
class MatrixInterleaver {
public:
    /**
     * @brief 构造函数
     * @param rows 矩阵行数
     * @param cols 矩阵列数
     */
    MatrixInterleaver(int rows = 8, int cols = 8);

    /**
     * @brief 析构函数
     */
    ~MatrixInterleaver();

    /**
     * @brief 交织数据
     * @param data 原始数据
     * @return 交织后的数据
     */
    std::string interleave(const std::string& data);

    /**
     * @brief 解交织数据
     * @param interleavedData 交织后的数据
     * @return 解交织后的数据
     */
    std::string deinterleave(const std::string& interleavedData);

    /**
     * @brief 交织二进制数据
     * @param data 原始二进制数据
     * @return 交织后的二进制数据
     */
    std::vector<bool> interleave(const std::vector<bool>& data);

    /**
     * @brief 解交织二进制数据
     * @param interleavedData 交织后的二进制数据
     * @return 解交织后的二进制数据
     */
    std::vector<bool> deinterleave(const std::vector<bool>& interleavedData);

    /**
     * @brief 交织字节数据
     * @param data 原始字节数据
     * @return 交织后的字节数据
     */
    std::vector<uint8_t> interleave(const std::vector<uint8_t>& data);

    /**
     * @brief 解交织字节数据
     * @param interleavedData 交织后的字节数据
     * @return 解交织后的字节数据
     */
    std::vector<uint8_t> deinterleave(const std::vector<uint8_t>& interleavedData);

    /**
     * @brief 设置交织参数
     * @param rows 矩阵行数
     * @param cols 矩阵列数
     */
    void setParameters(int rows, int cols);

    /**
     * @brief 获取行数
     * @return 矩阵行数
     */
    int getRows() const;

    /**
     * @brief 获取列数
     * @return 矩阵列数
     */
    int getCols() const;

private:
    // 行数
    int rows;

    // 列数
    int cols;
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
