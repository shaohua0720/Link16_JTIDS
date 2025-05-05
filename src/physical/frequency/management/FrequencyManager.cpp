#include "FrequencyManager.h"
#include "core/utils/logger.h"
#include <iostream>

namespace link16 {
namespace physical {
namespace frequency {

// 构造函数
FrequencyManager::FrequencyManager()
    : currentFrequencyName("") {
}

// 析构函数
FrequencyManager::~FrequencyManager() {
}

// 初始化频率管理器
bool FrequencyManager::initialize() {
    LOG_INFO("初始化频率管理器");
    
    // 清空频率表
    frequencies.clear();
    currentFrequencyName = "";
    
    // 添加默认频率
    addFrequency("Link16_Base", 969.0e6);  // 969MHz
    addFrequency("Link16_Alt1", 972.0e6);  // 972MHz
    addFrequency("Link16_Alt2", 975.0e6);  // 975MHz
    
    // 设置默认频率
    setCurrentFrequency("Link16_Base");
    
    return true;
}

// 添加频率
bool FrequencyManager::addFrequency(const std::string& name, double frequency) {
    if (name.empty()) {
        LOG_ERROR("频率名称不能为空");
        return false;
    }
    
    if (frequency <= 0.0) {
        LOG_ERROR("频率值必须大于0: " + std::to_string(frequency));
        return false;
    }
    
    // 检查是否已存在
    if (frequencies.find(name) != frequencies.end()) {
        LOG_WARNING("频率已存在，将被覆盖: " + name);
    }
    
    frequencies[name] = frequency;
    LOG_INFO("添加频率: " + name + " = " + std::to_string(frequency / 1e6) + " MHz");
    
    return true;
}

// 移除频率
bool FrequencyManager::removeFrequency(const std::string& name) {
    if (name.empty()) {
        LOG_ERROR("频率名称不能为空");
        return false;
    }
    
    // 检查是否存在
    auto it = frequencies.find(name);
    if (it == frequencies.end()) {
        LOG_WARNING("频率不存在，无法移除: " + name);
        return false;
    }
    
    // 如果是当前频率，清除当前频率
    if (name == currentFrequencyName) {
        currentFrequencyName = "";
    }
    
    frequencies.erase(it);
    LOG_INFO("移除频率: " + name);
    
    return true;
}

// 获取频率
double FrequencyManager::getFrequency(const std::string& name) const {
    if (name.empty()) {
        LOG_ERROR("频率名称不能为空");
        return 0.0;
    }
    
    // 检查是否存在
    auto it = frequencies.find(name);
    if (it == frequencies.end()) {
        LOG_WARNING("频率不存在: " + name);
        return 0.0;
    }
    
    return it->second;
}

// 获取所有频率
std::map<std::string, double> FrequencyManager::getAllFrequencies() const {
    return frequencies;
}

// 设置当前频率
bool FrequencyManager::setCurrentFrequency(const std::string& name) {
    if (name.empty()) {
        LOG_ERROR("频率名称不能为空");
        return false;
    }
    
    // 检查是否存在
    auto it = frequencies.find(name);
    if (it == frequencies.end()) {
        LOG_WARNING("频率不存在，无法设置为当前频率: " + name);
        return false;
    }
    
    currentFrequencyName = name;
    LOG_INFO("设置当前频率: " + name + " = " + std::to_string(it->second / 1e6) + " MHz");
    
    return true;
}

// 获取当前频率名称
std::string FrequencyManager::getCurrentFrequencyName() const {
    return currentFrequencyName;
}

// 获取当前频率值
double FrequencyManager::getCurrentFrequencyValue() const {
    if (currentFrequencyName.empty()) {
        LOG_WARNING("当前未设置频率");
        return 0.0;
    }
    
    return getFrequency(currentFrequencyName);
}

// 检查频率是否存在
bool FrequencyManager::hasFrequency(const std::string& name) const {
    if (name.empty()) {
        return false;
    }
    
    return frequencies.find(name) != frequencies.end();
}

// 获取频率数量
size_t FrequencyManager::getFrequencyCount() const {
    return frequencies.size();
}

} // namespace frequency
} // namespace physical
} // namespace link16
