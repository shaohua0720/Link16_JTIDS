#pragma once
#include <vector>
#include <string>
#include <map>
#include <cstdint>

namespace link16 {
namespace physical {
namespace frequency {

class FrequencyManager {
public:
    // 构造函数
    FrequencyManager();
    
    // 析构函数
    ~FrequencyManager();
    
    // 初始化频率管理器
    bool initialize();
    
    // 添加频率
    bool addFrequency(const std::string& name, double frequency);
    
    // 移除频率
    bool removeFrequency(const std::string& name);
    
    // 获取频率
    double getFrequency(const std::string& name) const;
    
    // 获取所有频率
    std::map<std::string, double> getAllFrequencies() const;
    
    // 设置当前频率
    bool setCurrentFrequency(const std::string& name);
    
    // 获取当前频率名称
    std::string getCurrentFrequencyName() const;
    
    // 获取当前频率值
    double getCurrentFrequencyValue() const;
    
    // 检查频率是否存在
    bool hasFrequency(const std::string& name) const;
    
    // 获取频率数量
    size_t getFrequencyCount() const;

private:
    // 频率映射表
    std::map<std::string, double> frequencies;
    
    // 当前频率名称
    std::string currentFrequencyName;
};

} // namespace frequency
} // namespace physical
} // namespace link16
