#include "RSCoder.h"
#include "core/utils/logger.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <stdexcept>
#include "lib/schifra/schifra_galois_field.hpp"
#include "lib/schifra/schifra_galois_field_polynomial.hpp"
#include "lib/schifra/schifra_sequential_root_generator_polynomial_creator.hpp"
#include "lib/schifra/schifra_reed_solomon_encoder.hpp"
#include "lib/schifra/schifra_reed_solomon_decoder.hpp"
#include "lib/schifra/schifra_reed_solomon_block.hpp"
#include "lib/schifra/schifra_error_processes.hpp"

namespace link16 {
namespace coding {
namespace error_correction {

// 检查参数是否有效
bool RSCoder::isValidParameters(int codeLength, int dataLength) {
    return (codeLength > 0 && dataLength > 0 && dataLength < codeLength && codeLength < 256);
}

// 构造函数
RSCoder::RSCoder(int codeLength, int dataLength)
    : codeLength(codeLength), dataLength(dataLength) {
    // 检查参数有效性
    if (!isValidParameters(codeLength, dataLength)) {
        LOG_ERROR("RS编码器参数无效: codeLength=" + std::to_string(codeLength) +
                 ", dataLength=" + std::to_string(dataLength));
        // 使用默认值
        this->codeLength = 31;
        this->dataLength = 15;
    }

    errorCorrectionCapability = (this->codeLength - this->dataLength) / 2;
    LOG_INFO("创建RS编码器: codeLength=" + std::to_string(this->codeLength) +
             ", dataLength=" + std::to_string(this->dataLength) +
             ", 纠错能力=" + std::to_string(errorCorrectionCapability));
}

// 析构函数
RSCoder::~RSCoder() {
}

// 编码函数
bool RSCoder::encode(const std::string& message, uint8_t* encodedData) {
    if (!encodedData) {
        LOG_ERROR("编码数据缓冲区为空");
        return false;
    }

    if (message.empty()) {
        LOG_ERROR("消息为空");
        return false;
    }

    LOG_INFO("RS编码: 消息长度=" + std::to_string(message.length()) +
             ", codeLength=" + std::to_string(codeLength) +
             ", dataLength=" + std::to_string(dataLength));

    try {
        // 定义RS编码参数
        const std::size_t field_descriptor                = 8;
        const std::size_t generator_polynomial_index      = 0;
        const std::size_t generator_polynomial_root_count = codeLength - dataLength;

        // 创建Galois域
        typedef schifra::galois::field<field_descriptor> field_type;
        typedef schifra::galois::field_polynomial<field_type> field_polynomial_type;

        // 创建Reed-Solomon编码器和解码器
        typedef schifra::reed_solomon::encoder<field_type, dataLength, codeLength> encoder_type;
        typedef schifra::reed_solomon::decoder<field_type, dataLength, codeLength> decoder_type;

        // 创建Reed-Solomon块
        typedef schifra::reed_solomon::block<field_type, dataLength, codeLength> block_type;

        // 创建Galois域
        const field_type field;

        // 创建生成多项式
        field_polynomial_type generator_polynomial(field);

        // 创建生成多项式
        if (!schifra::make_sequential_root_generator_polynomial(
                field,
                generator_polynomial_index,
                generator_polynomial_root_count,
                generator_polynomial)) {
            LOG_ERROR("创建生成多项式失败");
            return false;
        }

        // 创建编码器
        const encoder_type encoder(field, generator_polynomial);

        // 创建编码块
        block_type block;

        // 填充数据
        for (std::size_t i = 0; i < dataLength; ++i) {
            if (i < message.length()) {
                block.data[i] = static_cast<uint8_t>(message[i]);
            } else {
                block.data[i] = 0; // 填充0
            }
        }

        // 编码
        if (!encoder.encode(block)) {
            LOG_ERROR("RS编码失败");
            return false;
        }

        // 复制编码结果
        for (std::size_t i = 0; i < codeLength; ++i) {
            encodedData[i] = block[i];
        }

        LOG_INFO("RS编码成功");
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("RS编码异常: " + std::string(e.what()));
        return false;
    }
}

// 解码函数
bool RSCoder::decode(const uint8_t* encodedData, std::string& message) {
    if (!encodedData) {
        LOG_ERROR("编码数据缓冲区为空");
        return false;
    }

    LOG_INFO("RS解码: codeLength=" + std::to_string(codeLength) +
             ", dataLength=" + std::to_string(dataLength));

    try {
        // 定义RS编码参数
        const std::size_t field_descriptor                = 8;
        const std::size_t generator_polynomial_index      = 0;
        const std::size_t generator_polynomial_root_count = codeLength - dataLength;

        // 创建Galois域
        typedef schifra::galois::field<field_descriptor> field_type;
        typedef schifra::galois::field_polynomial<field_type> field_polynomial_type;

        // 创建Reed-Solomon编码器和解码器
        typedef schifra::reed_solomon::decoder<field_type, dataLength, codeLength> decoder_type;

        // 创建Reed-Solomon块
        typedef schifra::reed_solomon::block<field_type, dataLength, codeLength> block_type;

        // 创建Galois域
        const field_type field;

        // 创建生成多项式
        field_polynomial_type generator_polynomial(field);

        // 创建生成多项式
        if (!schifra::make_sequential_root_generator_polynomial(
                field,
                generator_polynomial_index,
                generator_polynomial_root_count,
                generator_polynomial)) {
            LOG_ERROR("创建生成多项式失败");
            return false;
        }

        // 创建解码器
        const decoder_type decoder(field, generator_polynomial_index);

        // 创建编码块
        block_type block;

        // 填充数据
        for (std::size_t i = 0; i < codeLength; ++i) {
            block[i] = encodedData[i];
        }

        // 解码
        if (!decoder.decode(block)) {
            LOG_ERROR("RS解码失败");
            return false;
        }

        // 提取解码结果
        message.clear();
        for (std::size_t i = 0; i < dataLength; ++i) {
            // 跳过填充的0
            if (block.data[i] != 0) {
                message.push_back(static_cast<char>(block.data[i]));
            }
        }

        LOG_INFO("RS解码成功: 解码后消息长度=" + std::to_string(message.length()));
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("RS解码异常: " + std::string(e.what()));
        return false;
    }
}

// 编码函数(字节数组版本)
bool RSCoder::encode(const uint8_t* data, size_t dataSize, uint8_t* encodedData) {
    if (!encodedData) {
        LOG_ERROR("编码数据缓冲区为空");
        return false;
    }

    if (!data) {
        LOG_ERROR("输入数据为空");
        return false;
    }

    if (dataSize == 0) {
        LOG_ERROR("输入数据大小为0");
        return false;
    }

    if (dataSize > static_cast<size_t>(dataLength)) {
        LOG_ERROR("输入数据大小超过数据长度: " + std::to_string(dataSize) +
                 " > " + std::to_string(dataLength));
        return false;
    }

    LOG_INFO("RS编码(字节数组): 数据大小=" + std::to_string(dataSize) +
             ", codeLength=" + std::to_string(codeLength) +
             ", dataLength=" + std::to_string(dataLength));

    try {
        // 定义RS编码参数
        const std::size_t field_descriptor                = 8;
        const std::size_t generator_polynomial_index      = 0;
        const std::size_t generator_polynomial_root_count = codeLength - dataLength;

        // 创建Galois域
        typedef schifra::galois::field<field_descriptor> field_type;
        typedef schifra::galois::field_polynomial<field_type> field_polynomial_type;

        // 创建Reed-Solomon编码器和解码器
        typedef schifra::reed_solomon::encoder<field_type, dataLength, codeLength> encoder_type;

        // 创建Reed-Solomon块
        typedef schifra::reed_solomon::block<field_type, dataLength, codeLength> block_type;

        // 创建Galois域
        const field_type field;

        // 创建生成多项式
        field_polynomial_type generator_polynomial(field);

        // 创建生成多项式
        if (!schifra::make_sequential_root_generator_polynomial(
                field,
                generator_polynomial_index,
                generator_polynomial_root_count,
                generator_polynomial)) {
            LOG_ERROR("创建生成多项式失败");
            return false;
        }

        // 创建编码器
        const encoder_type encoder(field, generator_polynomial);

        // 创建编码块
        block_type block;

        // 填充数据
        for (std::size_t i = 0; i < dataLength; ++i) {
            if (i < dataSize) {
                block.data[i] = data[i];
            } else {
                block.data[i] = 0; // 填充0
            }
        }

        // 编码
        if (!encoder.encode(block)) {
            LOG_ERROR("RS编码失败");
            return false;
        }

        // 复制编码结果
        for (std::size_t i = 0; i < codeLength; ++i) {
            encodedData[i] = block[i];
        }

        LOG_INFO("RS编码成功");
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("RS编码异常: " + std::string(e.what()));
        return false;
    }
}

// 解码函数(字节数组版本)
bool RSCoder::decode(const uint8_t* encodedData, uint8_t* decodedData, size_t& decodedSize) {
    if (!encodedData) {
        LOG_ERROR("编码数据缓冲区为空");
        return false;
    }

    if (!decodedData) {
        LOG_ERROR("解码数据缓冲区为空");
        return false;
    }

    LOG_INFO("RS解码(字节数组): codeLength=" + std::to_string(codeLength) +
             ", dataLength=" + std::to_string(dataLength));

    try {
        // 定义RS编码参数
        const std::size_t field_descriptor                = 8;
        const std::size_t generator_polynomial_index      = 0;
        const std::size_t generator_polynomial_root_count = codeLength - dataLength;

        // 创建Galois域
        typedef schifra::galois::field<field_descriptor> field_type;
        typedef schifra::galois::field_polynomial<field_type> field_polynomial_type;

        // 创建Reed-Solomon编码器和解码器
        typedef schifra::reed_solomon::decoder<field_type, dataLength, codeLength> decoder_type;

        // 创建Reed-Solomon块
        typedef schifra::reed_solomon::block<field_type, dataLength, codeLength> block_type;

        // 创建Galois域
        const field_type field;

        // 创建生成多项式
        field_polynomial_type generator_polynomial(field);

        // 创建生成多项式
        if (!schifra::make_sequential_root_generator_polynomial(
                field,
                generator_polynomial_index,
                generator_polynomial_root_count,
                generator_polynomial)) {
            LOG_ERROR("创建生成多项式失败");
            return false;
        }

        // 创建解码器
        const decoder_type decoder(field, generator_polynomial_index);

        // 创建编码块
        block_type block;

        // 填充数据
        for (std::size_t i = 0; i < codeLength; ++i) {
            block[i] = encodedData[i];
        }

        // 解码
        if (!decoder.decode(block)) {
            LOG_ERROR("RS解码失败");
            return false;
        }

        // 提取解码结果
        decodedSize = 0;
        for (std::size_t i = 0; i < dataLength; ++i) {
            decodedData[i] = block.data[i];
            if (block.data[i] != 0) {
                decodedSize = i + 1;
            }
        }

        LOG_INFO("RS解码成功: 解码后数据大小=" + std::to_string(decodedSize));
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("RS解码异常: " + std::string(e.what()));
        return false;
    }
}

// 设置编码参数
void RSCoder::setParameters(int codeLength, int dataLength) {
    // 检查参数有效性
    if (!isValidParameters(codeLength, dataLength)) {
        LOG_ERROR("RS编码器参数无效: codeLength=" + std::to_string(codeLength) +
                 ", dataLength=" + std::to_string(dataLength));
        return;
    }

    this->codeLength = codeLength;
    this->dataLength = dataLength;
    this->errorCorrectionCapability = (codeLength - dataLength) / 2;

    LOG_INFO("设置RS编码器参数: codeLength=" + std::to_string(codeLength) +
             ", dataLength=" + std::to_string(dataLength) +
             ", 纠错能力=" + std::to_string(errorCorrectionCapability));
}

// 获取编码长度
int RSCoder::getCodeLength() const {
    return codeLength;
}

// 获取数据长度
int RSCoder::getDataLength() const {
    return dataLength;
}

// 获取纠错能力
int RSCoder::getErrorCorrectionCapability() const {
    return errorCorrectionCapability;
}

// 全局RS编码函数
bool RS(int codeLength, int dataLength, const std::string& message, void* encodedData) {
    if (!encodedData) {
        LOG_ERROR("编码数据缓冲区为空");
        return false;
    }

    // 创建RS编码器
    RSCoder coder(codeLength, dataLength);

    // 编码
    return coder.encode(message, static_cast<uint8_t*>(encodedData));
}

// 全局RS编码函数(字节数组版本)
bool RS(int codeLength, int dataLength, const uint8_t* data, size_t dataSize, void* encodedData) {
    if (!encodedData) {
        LOG_ERROR("编码数据缓冲区为空");
        return false;
    }

    if (!data) {
        LOG_ERROR("输入数据为空");
        return false;
    }

    // 创建RS编码器
    RSCoder coder(codeLength, dataLength);

    // 编码
    return coder.encode(data, dataSize, static_cast<uint8_t*>(encodedData));
}

} // namespace error_correction
} // namespace coding
} // namespace link16
