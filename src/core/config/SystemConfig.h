#pragma once
#include <string>
#include <map>

namespace link16 {
namespace config {

class SystemConfig {
public:
    // 单例模式获取实例
    static SystemConfig& getInstance();

    // 获取配置项
    std::string getConfigValue(const std::string& key) const;
    
    // 设置配置项
    void setConfigValue(const std::string& key, const std::string& value);
    
    // 加载配置文件
    bool loadConfigFile(const std::string& filePath);
    
    // 保存配置文件
    bool saveConfigFile(const std::string& filePath) const;

private:
    // 私有构造函数(单例模式)
    SystemConfig();
    
    // 配置项存储
    std::map<std::string, std::string> configMap;
};

} // namespace config
} // namespace link16
