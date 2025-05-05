#include "MatrixInterleaver.h"
#include "core/utils/logger.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

namespace link16 {
namespace coding {
namespace interleaving {

// 构造函数
MatrixInterleaver::MatrixInterleaver(int rows, int cols)
    : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        this->rows = 8;
        this->cols = 8;
    }
}

// 析构函数
MatrixInterleaver::~MatrixInterleaver() {
}

// 交织
std::string MatrixInterleaver::interleave(const std::string& data) {
    return interleave(data, rows, cols);
}

// 解交织
std::string MatrixInterleaver::deinterleave(const std::string& interleavedData) {
    return deinterleave(interleavedData, rows, cols);
}

// 设置交织参数
void MatrixInterleaver::setParameters(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        return;
    }

    this->rows = rows;
    this->cols = cols;
    LOG_INFO("设置矩阵交织器参数: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
}

// 获取行数
int MatrixInterleaver::getRows() const {
    return rows;
}

// 获取列数
int MatrixInterleaver::getCols() const {
    return cols;
}

// 全局交织函数
std::string interleave(const std::string& data, int rows, int cols) {
    if (data.empty()) {
        return data;
    }

    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        return data;
    }

    LOG_INFO("执行矩阵交织: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols) +
             ", 数据长度=" + std::to_string(data.length()));

    // 计算需要的矩阵数量
    size_t matrixSize = rows * cols;
    size_t numMatrices = (data.length() + matrixSize - 1) / matrixSize;
    size_t totalSize = numMatrices * matrixSize;

    // 创建填充后的数据
    std::string paddedData = data;
    if (paddedData.length() < totalSize) {
        // 使用0填充
        paddedData.append(totalSize - paddedData.length(), '0');
    }

    // 创建结果字符串
    std::string result;
    result.reserve(totalSize);

    // 对每个矩阵进行交织
    for (size_t m = 0; m < numMatrices; ++m) {
        // 创建矩阵
        std::vector<std::vector<char>> matrix(rows, std::vector<char>(cols));

        // 按行填充矩阵
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                size_t index = m * matrixSize + i * cols + j;
                matrix[i][j] = paddedData[index];
            }
        }

        // 按列读取矩阵
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; i < rows; ++i) {
                result.push_back(matrix[i][j]);
            }
        }
    }

    return result;
}

// 全局解交织函数
std::string deinterleave(const std::string& interleavedData, int rows, int cols) {
    if (interleavedData.empty()) {
        return interleavedData;
    }

    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵解交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        return interleavedData;
    }

    LOG_INFO("执行矩阵解交织: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols) +
             ", 数据长度=" + std::to_string(interleavedData.length()));

    // 检查数据长度是否是矩阵大小的整数倍
    size_t matrixSize = rows * cols;
    if (interleavedData.length() % matrixSize != 0) {
        LOG_WARNING("交织数据长度不是矩阵大小的整数倍，可能导致解交织错误");
    }

    // 计算矩阵数量
    size_t numMatrices = interleavedData.length() / matrixSize;

    // 创建结果字符串
    std::string result;
    result.reserve(interleavedData.length());

    // 对每个矩阵进行解交织
    for (size_t m = 0; m < numMatrices; ++m) {
        // 创建矩阵
        std::vector<std::vector<char>> matrix(rows, std::vector<char>(cols));

        // 按列填充矩阵
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; i < rows; ++i) {
                size_t index = m * matrixSize + j * rows + i;
                matrix[i][j] = interleavedData[index];
            }
        }

        // 按行读取矩阵
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.push_back(matrix[i][j]);
            }
        }
    }

    return result;
}

// 交织二进制数据
std::vector<bool> MatrixInterleaver::interleave(const std::vector<bool>& data) {
    return interleave(data, rows, cols);
}

// 解交织二进制数据
std::vector<bool> MatrixInterleaver::deinterleave(const std::vector<bool>& interleavedData) {
    return deinterleave(interleavedData, rows, cols);
}

// 交织字节数据
std::vector<uint8_t> MatrixInterleaver::interleave(const std::vector<uint8_t>& data) {
    return interleave(data, rows, cols);
}

// 解交织字节数据
std::vector<uint8_t> MatrixInterleaver::deinterleave(const std::vector<uint8_t>& interleavedData) {
    return deinterleave(interleavedData, rows, cols);
}

// 全局交织函数(二进制版本)
std::vector<bool> interleave(const std::vector<bool>& data, int rows, int cols) {
    if (data.empty()) {
        return data;
    }

    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        return data;
    }

    LOG_INFO("执行矩阵交织(二进制): rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols) +
             ", 数据长度=" + std::to_string(data.size()));

    // 计算需要的矩阵数量
    size_t matrixSize = rows * cols;
    size_t numMatrices = (data.size() + matrixSize - 1) / matrixSize;
    size_t totalSize = numMatrices * matrixSize;

    // 创建填充后的数据
    std::vector<bool> paddedData = data;
    if (paddedData.size() < totalSize) {
        // 使用false填充
        paddedData.resize(totalSize, false);
    }

    // 创建结果向量
    std::vector<bool> result;
    result.reserve(totalSize);

    // 对每个矩阵进行交织
    for (size_t m = 0; m < numMatrices; ++m) {
        // 创建矩阵
        std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols));

        // 按行填充矩阵
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                size_t index = m * matrixSize + i * cols + j;
                matrix[i][j] = paddedData[index];
            }
        }

        // 按列读取矩阵
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; i < rows; ++i) {
                result.push_back(matrix[i][j]);
            }
        }
    }

    return result;
}

// 全局解交织函数(二进制版本)
std::vector<bool> deinterleave(const std::vector<bool>& interleavedData, int rows, int cols) {
    if (interleavedData.empty()) {
        return interleavedData;
    }

    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵解交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        return interleavedData;
    }

    LOG_INFO("执行矩阵解交织(二进制): rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols) +
             ", 数据长度=" + std::to_string(interleavedData.size()));

    // 检查数据长度是否是矩阵大小的整数倍
    size_t matrixSize = rows * cols;
    if (interleavedData.size() % matrixSize != 0) {
        LOG_WARNING("交织数据长度不是矩阵大小的整数倍，可能导致解交织错误");
    }

    // 计算矩阵数量
    size_t numMatrices = interleavedData.size() / matrixSize;

    // 创建结果向量
    std::vector<bool> result;
    result.reserve(interleavedData.size());

    // 对每个矩阵进行解交织
    for (size_t m = 0; m < numMatrices; ++m) {
        // 创建矩阵
        std::vector<std::vector<bool>> matrix(rows, std::vector<bool>(cols));

        // 按列填充矩阵
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; i < rows; ++i) {
                size_t index = m * matrixSize + j * rows + i;
                matrix[i][j] = interleavedData[index];
            }
        }

        // 按行读取矩阵
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.push_back(matrix[i][j]);
            }
        }
    }

    return result;
}

// 全局交织函数(字节版本)
std::vector<uint8_t> interleave(const std::vector<uint8_t>& data, int rows, int cols) {
    if (data.empty()) {
        return data;
    }

    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        return data;
    }

    LOG_INFO("执行矩阵交织(字节): rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols) +
             ", 数据长度=" + std::to_string(data.size()));

    // 计算需要的矩阵数量
    size_t matrixSize = rows * cols;
    size_t numMatrices = (data.size() + matrixSize - 1) / matrixSize;
    size_t totalSize = numMatrices * matrixSize;

    // 创建填充后的数据
    std::vector<uint8_t> paddedData = data;
    if (paddedData.size() < totalSize) {
        // 使用0填充
        paddedData.resize(totalSize, 0);
    }

    // 创建结果向量
    std::vector<uint8_t> result;
    result.reserve(totalSize);

    // 对每个矩阵进行交织
    for (size_t m = 0; m < numMatrices; ++m) {
        // 创建矩阵
        std::vector<std::vector<uint8_t>> matrix(rows, std::vector<uint8_t>(cols));

        // 按行填充矩阵
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                size_t index = m * matrixSize + i * cols + j;
                matrix[i][j] = paddedData[index];
            }
        }

        // 按列读取矩阵
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; i < rows; ++i) {
                result.push_back(matrix[i][j]);
            }
        }
    }

    return result;
}

// 全局解交织函数(字节版本)
std::vector<uint8_t> deinterleave(const std::vector<uint8_t>& interleavedData, int rows, int cols) {
    if (interleavedData.empty()) {
        return interleavedData;
    }

    if (rows <= 0 || cols <= 0) {
        LOG_ERROR("矩阵解交织器参数无效: rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols));
        return interleavedData;
    }

    LOG_INFO("执行矩阵解交织(字节): rows=" + std::to_string(rows) + ", cols=" + std::to_string(cols) +
             ", 数据长度=" + std::to_string(interleavedData.size()));

    // 检查数据长度是否是矩阵大小的整数倍
    size_t matrixSize = rows * cols;
    if (interleavedData.size() % matrixSize != 0) {
        LOG_WARNING("交织数据长度不是矩阵大小的整数倍，可能导致解交织错误");
    }

    // 计算矩阵数量
    size_t numMatrices = interleavedData.size() / matrixSize;

    // 创建结果向量
    std::vector<uint8_t> result;
    result.reserve(interleavedData.size());

    // 对每个矩阵进行解交织
    for (size_t m = 0; m < numMatrices; ++m) {
        // 创建矩阵
        std::vector<std::vector<uint8_t>> matrix(rows, std::vector<uint8_t>(cols));

        // 按列填充矩阵
        for (int j = 0; j < cols; ++j) {
            for (int i = 0; i < rows; ++i) {
                size_t index = m * matrixSize + j * rows + i;
                matrix[i][j] = interleavedData[index];
            }
        }

        // 按行读取矩阵
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.push_back(matrix[i][j]);
            }
        }
    }

    return result;
}

} // namespace interleaving
} // namespace coding
} // namespace link16
