#include "tools.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

namespace link16 {
namespace utils {

// 保存消息到文件
bool Tools::saveMessage(const std::string& msg, const std::string& filePath) {
    std::ofstream fout(filePath, std::ios::app);
    if (!fout.is_open()) {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return false;
    }
    
    fout << msg;
    fout.close();
    std::cout << "消息已成功保存到文件: " << filePath << std::endl;
    return true;
}

// 删除文件
bool Tools::deleteFile(const std::string& filePath) {
    if (std::remove(filePath.c_str()) == 0) {
        std::cout << "文件删除成功: " << filePath << std::endl;
        return true;
    } else {
        std::cerr << "文件删除失败: " << filePath << std::endl;
        return false;
    }
}

// 从文件读取消息
std::string Tools::readMessage(const std::string& filePath) {
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        std::cerr << "无法打开文件: " << filePath << std::endl;
        return "";
    }
    
    std::string buffer;
    std::string line;
    while (std::getline(fin, line)) {
        buffer += line;
    }
    
    fin.close();
    return buffer;
}

// 生成随机二进制字符串
std::string Tools::generateRandomBinary(int length) {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
    
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += std::to_string(std::rand() % 2);
    }
    return result;
}

// 分割字符串
std::vector<std::string> Tools::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

// 字符串转二进制字符串
std::string Tools::stringToBitString(const std::string& str) {
    std::string result;
    for (char c : str) {
        std::bitset<8> bits(c);
        result += bits.to_string();
    }
    return result;
}

// 二进制字符串转字符串
std::string Tools::bitStringToString(const std::string& bitStr) {
    std::string result;
    for (size_t i = 0; i < bitStr.length(); i += 8) {
        if (i + 8 <= bitStr.length()) {
            std::bitset<8> bits(bitStr.substr(i, 8));
            result += static_cast<char>(bits.to_ulong());
        }
    }
    return result;
}

// 字符串转字符数组
uint8_t* Tools::stringToCharArray(const std::string& strData, int charLength) {
    if (strData.length() % 8 != 0) {
        std::cerr << "输入字符串长度有误，需8bit整数倍" << std::endl;
        return nullptr;
    }
    
    uint8_t* data = new uint8_t[charLength]();
    for (int i = 0; i < charLength; ++i) {
        if (i * 8 + 8 <= strData.length()) {
            std::bitset<8> bits(strData.substr(i * 8, 8));
            data[i] = static_cast<uint8_t>(bits.to_ulong());
        }
    }
    
    return data;
}

// 打印十六进制数据
void Tools::printHex(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::dec << std::endl;
}

// 打印二进制数据
void Tools::printBinary(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        std::cout << std::bitset<8>(data[i]) << " ";
        if ((i + 1) % 8 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

} // namespace utils
} // namespace link16
