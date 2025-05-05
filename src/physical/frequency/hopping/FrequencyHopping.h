#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace link16 {
namespace physical {
namespace frequency {

class FrequencyHopping {
public:
    // 构造函数
    FrequencyHopping();
    
    // 析构函数
    ~FrequencyHopping();
    
    // 初始化跳频模式
    bool initialize(int pattern, uint32_t seed = 0);
    
    // 获取下一个跳频频率
    double getNextFrequency();
    
    // 获取指定索引的跳频频率
    double getFrequencyAt(size_t index);
    
    // 重置跳频序列
    void reset();
    
    // 设置跳频模式
    bool setPattern(int pattern);
    
    // 获取跳频模式
    int getPattern() const;
    
    // 设置跳频种子
    void setSeed(uint32_t seed);
    
    // 获取跳频种子
    uint32_t getSeed() const;
    
    // 获取跳频序列长度
    size_t getSequenceLength() const;
    
    // 获取跳频序列
    std::vector<double> getSequence() const;

private:
    // 跳频模式
    int pattern;
    
    // 跳频种子
    uint32_t seed;
    
    // 当前跳频索引
    size_t currentIndex;
    
    // 跳频序列
    std::vector<double> sequence;
    
    // 生成跳频序列
    void generateSequence();
};

} // namespace frequency
} // namespace physical
} // namespace link16
