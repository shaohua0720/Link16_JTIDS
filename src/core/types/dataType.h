#pragma once
#include <bitset>
#include <cstdint>  // 包含标准整数类型定义

using namespace std;

// 使用标准类型定义
typedef std::bitset<5> symbol;  // 5bit = 1Symbol, 注意sizeof(symbol) = 4

#ifdef _WIN32
constexpr auto FILENAME = R"(data.txt)";  // Windows路径
#else
constexpr auto FILENAME = R"(data.txt)";  // Linux/macOS路径
#endif

const unsigned int BLOCK_BYTES_LENGTH = 16 * sizeof(unsigned char);

enum RS_Length
{
	code_16_7 = 16,
	data_16_7 = 7,
	fec_16_7 = 9,
	code_31_15 = 31,
	data_31_15 = 15,
	fec_31_15 = 16,
};
