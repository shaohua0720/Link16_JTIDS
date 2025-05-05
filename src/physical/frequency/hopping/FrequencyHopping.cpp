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
    // 清空序列
    sequence.clear();

    // Link16频率范围
    double baseFreq = 969.0e6;  // 969MHz
    double maxFreq = 1206.0e6;  // 1206MHz
    double hopStep = 3.0e6;     // 3MHz (Link16标准)

    // 计算可用频率数量
    int numFrequencies = static_cast<int>((maxFreq - baseFreq) / hopStep) + 1;

    // 根据不同的跳频模式生成序列
    switch (pattern) {
        case 1: {
            // 模式1: 随机跳频
            std::mt19937 gen(seed);
            std::uniform_int_distribution<int> dist(0, numFrequencies - 1);

            // 生成51个频率(Link16标准)
            for (int i = 0; i < 51; ++i) {
                int index = dist(gen);
                double frequency = baseFreq + index * hopStep;
                sequence.push_back(frequency);
            }
            break;
        }
        case 2: {
            // 模式2: 伪随机跳频(线性同余法)
            const uint32_t a = 1664525;
            const uint32_t c = 1013904223;
            const uint32_t m = 0xFFFFFFFF;

            uint32_t x = seed;

            // 生成51个频率
            for (int i = 0; i < 51; ++i) {
                x = (a * x + c) & m;
                int index = x % numFrequencies;
                double frequency = baseFreq + index * hopStep;
                sequence.push_back(frequency);
            }
            break;
        }
        case 3: {
            // 模式3: 固定跳频模式(按顺序)
            for (int i = 0; i < numFrequencies; ++i) {
                double frequency = baseFreq + i * hopStep;
                sequence.push_back(frequency);
            }
            break;
        }
        case 4: {
            // 模式4: 固定跳频模式(按逆序)
            for (int i = numFrequencies - 1; i >= 0; --i) {
                double frequency = baseFreq + i * hopStep;
                sequence.push_back(frequency);
            }
            break;
        }
        case 5: {
            // 模式5: 交替跳频模式
            for (int i = 0; i < numFrequencies / 2; ++i) {
                double frequency1 = baseFreq + i * hopStep;
                double frequency2 = maxFreq - i * hopStep;
                sequence.push_back(frequency1);
                if (std::abs(frequency1 - frequency2) > 1.0) {
                    sequence.push_back(frequency2);
                }
            }
            break;
        }
        case 6: {
            // 模式6: 基于时隙的跳频(Link16标准)
            // 在实际的Link16系统中，跳频是基于时隙的
            // 这里简化实现，使用伪随机序列

            std::mt19937 gen(seed);

            // 生成基本跳频集
            std::vector<double> baseSet;
            for (int i = 0; i < numFrequencies; ++i) {
                baseSet.push_back(baseFreq + i * hopStep);
            }

            // 打乱基本跳频集
            std::shuffle(baseSet.begin(), baseSet.end(), gen);

            // 选择51个频率
            for (int i = 0; i < std::min(51, static_cast<int>(baseSet.size())); ++i) {
                sequence.push_back(baseSet[i]);
            }
            break;
        }
        default: {
            // 默认模式: 随机跳频
            LOG_WARNING("未知的跳频模式: " + std::to_string(pattern) + "，使用默认模式");

            std::mt19937 gen(seed);
            std::uniform_int_distribution<int> dist(0, numFrequencies - 1);

            // 生成51个频率
            for (int i = 0; i < 51; ++i) {
                int index = dist(gen);
                double frequency = baseFreq + index * hopStep;
                sequence.push_back(frequency);
            }
            break;
        }
    }

    // 重置索引
    currentIndex = 0;

    LOG_INFO("生成跳频序列，长度: " + std::to_string(sequence.size()) + " 个频点");
}

} // namespace frequency
} // namespace physical
} // namespace link16
