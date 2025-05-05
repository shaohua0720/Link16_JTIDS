#pragma once
#include <string>
#include <chrono>
#include <cstdint>

namespace link16 {
namespace physical {
namespace synchronization {

class TimeSynchronizer {
public:
    // 构造函数
    TimeSynchronizer();
    
    // 析构函数
    ~TimeSynchronizer();
    
    // 初始化时间同步器
    bool initialize();
    
    // 获取当前时间戳
    uint64_t getCurrentTimestamp();
    
    // 获取当前时隙
    uint32_t getCurrentTimeSlot();
    
    // 获取下一个时隙开始时间
    uint64_t getNextTimeSlotStart();
    
    // 等待下一个时隙
    bool waitForNextTimeSlot();
    
    // 设置时隙长度(微秒)
    void setTimeSlotLength(uint32_t length);
    
    // 获取时隙长度(微秒)
    uint32_t getTimeSlotLength() const;
    
    // 设置时隙偏移(微秒)
    void setTimeSlotOffset(int32_t offset);
    
    // 获取时隙偏移(微秒)
    int32_t getTimeSlotOffset() const;
    
    // 同步到外部时间源
    bool synchronizeToExternalSource(const std::string& source);
    
    // 获取同步状态
    bool isSynchronized() const;

private:
    // 时隙长度(微秒)
    uint32_t timeSlotLength;
    
    // 时隙偏移(微秒)
    int32_t timeSlotOffset;
    
    // 同步状态
    bool synchronized;
    
    // 同步开始时间
    std::chrono::time_point<std::chrono::steady_clock> syncStartTime;
    
    // 计算时隙索引
    uint32_t calculateTimeSlotIndex(uint64_t timestamp) const;
};

} // namespace synchronization
} // namespace physical
} // namespace link16
