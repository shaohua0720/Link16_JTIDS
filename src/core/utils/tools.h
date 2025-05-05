#pragma once
#include <string>
#include <vector>
#include <bitset>
#include <cstdint>

namespace link16 {
namespace utils {

class Tools {
public:
    // 文件操作
    static bool saveMessage(const std::string& msg, const std::string& filePath);
    static bool deleteFile(const std::string& filePath);
    static std::string readMessage(const std::string& filePath);
    
    // 字符串处理
    static std::string generateRandomBinary(int length);
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
    static std::string stringToBitString(const std::string& str);
    static std::string bitStringToString(const std::string& bitStr);
    
    // 数据转换
    static uint8_t* stringToCharArray(const std::string& strData, int charLength);
    
    // 调试辅助
    static void printHex(const uint8_t* data, size_t length);
    static void printBinary(const uint8_t* data, size_t length);
};

} // namespace utils
} // namespace link16
