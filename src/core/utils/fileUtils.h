#pragma once
#include <string>
#include <vector>

namespace link16 {
namespace utils {

class FileUtils {
public:
    // 文件操作
    static bool fileExists(const std::string& filePath);
    static bool createDirectory(const std::string& dirPath);
    static bool copyFile(const std::string& sourcePath, const std::string& destPath);
    static bool moveFile(const std::string& sourcePath, const std::string& destPath);
    
    // 文件读写
    static bool writeTextFile(const std::string& filePath, const std::string& content, bool append = false);
    static std::string readTextFile(const std::string& filePath);
    static bool writeBinaryFile(const std::string& filePath, const char* data, size_t size);
    static std::vector<char> readBinaryFile(const std::string& filePath);
    
    // 路径操作
    static std::string getFileName(const std::string& filePath);
    static std::string getFileExtension(const std::string& filePath);
    static std::string getDirectoryPath(const std::string& filePath);
    static std::string combinePaths(const std::string& path1, const std::string& path2);
};

} // namespace utils
} // namespace link16
