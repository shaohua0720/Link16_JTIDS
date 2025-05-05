#include "SystemConfig.h"
#include <fstream>
#include <iostream>

namespace link16 {
namespace config {

// 单例实例
SystemConfig& SystemConfig::getInstance() {
    static SystemConfig instance;
    return instance;
}

// 构造函数
SystemConfig::SystemConfig() {
    // 设置默认配置
    configMap["data_file_path"] = "data.txt";
    configMap["log_level"] = "info";
    configMap["use_hardware"] = "false";
}

// 获取配置项
std::string SystemConfig::getConfigValue(const std::string& key) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    }
    return "";
}

// 设置配置项
void SystemConfig::setConfigValue(const std::string& key, const std::string& value) {
    configMap[key] = value;
}

// 加载配置文件
bool SystemConfig::loadConfigFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "无法打开配置文件: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // 解析键值对
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // 去除前后空格
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            configMap[key] = value;
        }
    }

    file.close();
    return true;
}

// 保存配置文件
bool SystemConfig::saveConfigFile(const std::string& filePath) const {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "无法创建配置文件: " << filePath << std::endl;
        return false;
    }

    file << "# Link16系统配置文件\n";
    file << "# 格式: key=value\n\n";

    for (const auto& pair : configMap) {
        file << pair.first << "=" << pair.second << "\n";
    }

    file.close();
    return true;
}

} // namespace config
} // namespace link16
