#include "FrequencyHopping.h"
#include "core/utils/logger.h"
#include <iostream>
#include <random>
#include <algorithm>

namespace link16 {
namespace physical {
namespace frequency {

// 构造函数
FrequencyHopping::FrequencyHopping()
    : pattern(1), seed(0), currentIndex(0) {
}

// 析构函数
FrequencyHopping::~FrequencyHopping() {
}

// 初始化跳频模式
bool FrequencyHopping::initialize(int pattern, uint32_t seed) {
    this->pattern = pattern;
    this->seed = seed;
    this->currentIndex = 0;
    
    LOG_INFO("初始化跳频模式: " + std::to_string(pattern) + ", 种子: " + std::to_string(seed));
    
    generateSequence();
    return true;
}

// 获取下一个跳频频率
double FrequencyHopping::getNextFrequency() {
    if (sequence.empty()) {
        LOG_ERROR("跳频序列为空");
        return 0.0;
    }
    
    double freq = sequence[currentIndex];
    currentIndex = (currentIndex + 1) % sequence.size();
    return freq;
}

// 获取指定索引的跳频频率
double FrequencyHopping::getFrequencyAt(size_t index) {
    if (sequence.empty()) {
        LOG_ERROR("跳频序列为空");
        return 0.0;
    }
    
    if (index >= sequence.size()) {
        LOG_WARNING("索引超出跳频序列范围: " + std::to_string(index) + " >= " + std::to_string(sequence.size()));
        index = index % sequence.size();
    }
    
    return sequence[index];
}

// 重置跳频序列
void FrequencyHopping::reset() {
    currentIndex = 0;
}

// 设置跳频模式
bool FrequencyHopping::setPattern(int pattern) {
    this->pattern = pattern;
    generateSequence();
    return true;
}

// 获取跳频模式
int FrequencyHopping::getPattern() const {
    return pattern;
}

// 设置跳频种子
void FrequencyHopping::setSeed(uint32_t seed) {
    this->seed = seed;
    generateSequence();
}

// 获取跳频种子
uint32_t FrequencyHopping::getSeed() const {
    return seed;
}

// 获取跳频序列长度
size_t FrequencyHopping::getSequenceLength() const {
    return sequence.size();
}

// 获取跳频序列
std::vector<double> FrequencyHopping::getSequence() const {
    return sequence;
}

// 生成跳频序列
void FrequencyHopping::generateSequence() {
    // 这里只是一个接口，实际实现将在后续完成
    // 在实际实现中，将根据Link16协议生成跳频序列
    
    // 为了编译通过，生成一个简单的跳频序列
    sequence.clear();
    
    // 基础频率
    double baseFreq = 969.0e6;  // 969MHz
    
    // 跳频间隔
    double hopStep = 1.0e6;     // 1MHz
    
    // 序列长度
    size_t length = 51;
    
    // 生成随机序列
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(0, 50);
    
    for (size_t i = 0; i < length; ++i) {
        double freq = baseFreq + dist(gen) * hopStep;
        sequence.push_back(freq);
    }
    
    // 根据不同的模式调整序列
    switch (pattern) {
        case 1:  // 标准模式
            // 不做调整
            break;
            
        case 2:  // 顺序模式
            // 按频率排序
            std::sort(sequence.begin(), sequence.end());
            break;
            
        case 3:  // 反序模式
            // 按频率逆序排序
            std::sort(sequence.begin(), sequence.end(), std::greater<double>());
            break;
            
        default:
            LOG_WARNING("未知的跳频模式: " + std::to_string(pattern) + "，使用默认模式");
            break;
    }
    
    LOG_INFO("生成跳频序列: " + std::to_string(sequence.size()) + " 个频点");
}

} // namespace frequency
} // namespace physical
} // namespace link16
