#include "fileUtils.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

namespace link16 {
namespace utils {

// 检查文件是否存在
bool FileUtils::fileExists(const std::string& filePath) {
    return fs::exists(filePath);
}

// 创建目录
bool FileUtils::createDirectory(const std::string& dirPath) {
    std::error_code ec;
    bool result = fs::create_directories(dirPath, ec);
    if (ec) {
        std::cerr << "创建目录失败: " << dirPath << " - " << ec.message() << std::endl;
        return false;
    }
    return result;
}

// 复制文件
bool FileUtils::copyFile(const std::string& sourcePath, const std::string& destPath) {
    std::error_code ec;
    fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing, ec);
    if (ec) {
        std::cerr << "复制文件失败: " << sourcePath << " -> " << destPath << " - " << ec.message() << std::endl;
        return false;
    }
    return true;
}

// 移动文件
bool FileUtils::moveFile(const std::string& sourcePath, const std::string& destPath) {
    std::error_code ec;
    fs::rename(sourcePath, destPath, ec);
    if (ec) {
        std::cerr << "移动文件失败: " << sourcePath << " -> " << destPath << " - " << ec.message() << std::endl;
        return false;
    }
    return true;
}

// 写入文本文件
bool FileUtils::writeTextFile(const std::string& filePath, const std::string& content, bool append) {
    std::ofstream file;
    if (append) {
        file.open(filePath, std::ios::out | std::ios::app);
    } else {
        file.open(filePath, std::ios::out | std::ios::trunc);
    }
    
    if (!file.is_open()) {
        std::cerr << "无法打开文件进行写入: " << filePath << std::endl;
        return false;
    }
    
    file << content;
    file.close();
    return true;
}

// 读取文本文件
std::string FileUtils::readTextFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "无法打开文件进行读取: " << filePath << std::endl;
        return "";
    }
    
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

// 写入二进制文件
bool FileUtils::writeBinaryFile(const std::string& filePath, const char* data, size_t size) {
    std::ofstream file(filePath, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件进行二进制写入: " << filePath << std::endl;
        return false;
    }
    
    file.write(data, size);
    file.close();
    return true;
}

// 读取二进制文件
std::vector<char> FileUtils::readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "无法打开文件进行二进制读取: " << filePath << std::endl;
        return std::vector<char>();
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        file.close();
        return buffer;
    }
    
    file.close();
    return std::vector<char>();
}

// 获取文件名
std::string FileUtils::getFileName(const std::string& filePath) {
    return fs::path(filePath).filename().string();
}

// 获取文件扩展名
std::string FileUtils::getFileExtension(const std::string& filePath) {
    return fs::path(filePath).extension().string();
}

// 获取目录路径
std::string FileUtils::getDirectoryPath(const std::string& filePath) {
    return fs::path(filePath).parent_path().string();
}

// 合并路径
std::string FileUtils::combinePaths(const std::string& path1, const std::string& path2) {
    fs::path combined = fs::path(path1) / fs::path(path2);
    return combined.string();
}

} // namespace utils
} // namespace link16
