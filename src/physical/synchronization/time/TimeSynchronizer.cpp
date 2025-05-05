#include "TimeSynchronizer.h"
#include "core/utils/logger.h"
#include <iostream>
#include <thread>

namespace link16 {
namespace physical {
namespace synchronization {

// 构造函数
TimeSynchronizer::TimeSynchronizer()
    : timeSlotLength(7800), timeSlotOffset(0), synchronized(false) {
}

// 析构函数
TimeSynchronizer::~TimeSynchronizer() {
}

// 初始化时间同步器
bool TimeSynchronizer::initialize() {
    LOG_INFO("初始化时间同步器");
    
    // 记录同步开始时间
    syncStartTime = std::chrono::steady_clock::now();
    
    // 设置默认参数
    timeSlotLength = 7800;  // 7.8ms
    timeSlotOffset = 0;
    
    synchronized = true;
    return true;
}

// 获取当前时间戳
uint64_t TimeSynchronizer::getCurrentTimestamp() {
    if (!synchronized) {
        LOG_WARNING("时间同步器未同步");
    }
    
    // 计算当前时间戳(微秒)
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - syncStartTime);
    return static_cast<uint64_t>(duration.count());
}

// 获取当前时隙
uint32_t TimeSynchronizer::getCurrentTimeSlot() {
    uint64_t timestamp = getCurrentTimestamp();
    return calculateTimeSlotIndex(timestamp);
}

// 获取下一个时隙开始时间
uint64_t TimeSynchronizer::getNextTimeSlotStart() {
    uint64_t timestamp = getCurrentTimestamp();
    uint32_t currentSlot = calculateTimeSlotIndex(timestamp);
    uint64_t nextSlotStart = (currentSlot + 1) * static_cast<uint64_t>(timeSlotLength) + timeSlotOffset;
    return nextSlotStart;
}

// 等待下一个时隙
bool TimeSynchronizer::waitForNextTimeSlot() {
    if (!synchronized) {
        LOG_ERROR("时间同步器未同步，无法等待下一个时隙");
        return false;
    }
    
    uint64_t nextSlotStart = getNextTimeSlotStart();
    uint64_t currentTime = getCurrentTimestamp();
    
    if (nextSlotStart > currentTime) {
        uint64_t waitTime = nextSlotStart - currentTime;
        LOG_INFO("等待下一个时隙: " + std::to_string(waitTime) + " 微秒");
        std::this_thread::sleep_for(std::chrono::microseconds(waitTime));
        return true;
    } else {
        LOG_WARNING("下一个时隙已经开始，无需等待");
        return false;
    }
}

// 设置时隙长度(微秒)
void TimeSynchronizer::setTimeSlotLength(uint32_t length) {
    timeSlotLength = length;
    LOG_INFO("设置时隙长度: " + std::to_string(length) + " 微秒");
}

// 获取时隙长度(微秒)
uint32_t TimeSynchronizer::getTimeSlotLength() const {
    return timeSlotLength;
}

// 设置时隙偏移(微秒)
void TimeSynchronizer::setTimeSlotOffset(int32_t offset) {
    timeSlotOffset = offset;
    LOG_INFO("设置时隙偏移: " + std::to_string(offset) + " 微秒");
}

// 获取时隙偏移(微秒)
int32_t TimeSynchronizer::getTimeSlotOffset() const {
    return timeSlotOffset;
}

// 同步到外部时间源
bool TimeSynchronizer::synchronizeToExternalSource(const std::string& source) {
    LOG_INFO("同步到外部时间源: " + source);
    
    // 这里只是一个接口，实际实现将在后续完成
    // 在实际实现中，将根据外部时间源进行同步
    
    // 重置同步开始时间
    syncStartTime = std::chrono::steady_clock::now();
    
    synchronized = true;
    return true;
}

// 获取同步状态
bool TimeSynchronizer::isSynchronized() const {
    return synchronized;
}

// 计算时隙索引
uint32_t TimeSynchronizer::calculateTimeSlotIndex(uint64_t timestamp) const {
    // 考虑偏移
    int64_t adjustedTimestamp = static_cast<int64_t>(timestamp) - timeSlotOffset;
    
    // 处理负数时间戳
    if (adjustedTimestamp < 0) {
        adjustedTimestamp = 0;
    }
    
    return static_cast<uint32_t>(adjustedTimestamp / timeSlotLength);
}

} // namespace synchronization
} // namespace physical
} // namespace link16
