#include "Throughput.h"
#include "core/utils/logger.h"
#include <sstream>
#include <iomanip>

namespace link16 {
namespace simulation {
namespace metrics {

// 构造函数
Throughput::Throughput() : running(false), totalBits(0), totalPackets(0) {
}

// 析构函数
Throughput::~Throughput() {
}

// 开始计时
void Throughput::start() {
    if (running) {
        LOG_WARNING("吞吐量计时器已经在运行");
        return;
    }
    
    startTime = std::chrono::high_resolution_clock::now();
    running = true;
}

// 停止计时
void Throughput::stop() {
    if (!running) {
        LOG_WARNING("吞吐量计时器未在运行");
        return;
    }
    
    endTime = std::chrono::high_resolution_clock::now();
    running = false;
}

// 重置计时器
void Throughput::reset() {
    running = false;
    totalBits = 0;
    totalPackets = 0;
}

// 添加传输的比特数
void Throughput::addBits(size_t bits) {
    totalBits += bits;
}

// 添加传输的字节数
void Throughput::addBytes(size_t bytes) {
    totalBits += bytes * 8;
}

// 添加传输的数据包数
void Throughput::addPackets(size_t packets) {
    totalPackets += packets;
}

// 计算比特吞吐量(bps)
double Throughput::calculateBitThroughput() const {
    double elapsed = getElapsedTime();
    if (elapsed <= 0.0) {
        return 0.0;
    }
    
    return totalBits / elapsed;
}

// 计算字节吞吐量(Bps)
double Throughput::calculateByteThroughput() const {
    return calculateBitThroughput() / 8.0;
}

// 计算数据包吞吐量(pps)
double Throughput::calculatePacketThroughput() const {
    double elapsed = getElapsedTime();
    if (elapsed <= 0.0) {
        return 0.0;
    }
    
    return totalPackets / elapsed;
}

// 获取传输的总比特数
size_t Throughput::getTotalBits() const {
    return totalBits;
}

// 获取传输的总字节数
size_t Throughput::getTotalBytes() const {
    return totalBits / 8;
}

// 获取传输的总数据包数
size_t Throughput::getTotalPackets() const {
    return totalPackets;
}

// 获取传输时间(秒)
double Throughput::getElapsedTime() const {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - startTime).count();
    } else {
        return std::chrono::duration<double>(endTime - startTime).count();
    }
}

// 获取格式化的吞吐量字符串
std::string Throughput::getFormattedThroughput() const {
    std::ostringstream oss;
    
    double bitThroughput = calculateBitThroughput();
    double byteThroughput = calculateByteThroughput();
    double packetThroughput = calculatePacketThroughput();
    
    oss << std::fixed << std::setprecision(2);
    
    // 格式化比特吞吐量
    if (bitThroughput >= 1e9) {
        oss << bitThroughput / 1e9 << " Gbps";
    } else if (bitThroughput >= 1e6) {
        oss << bitThroughput / 1e6 << " Mbps";
    } else if (bitThroughput >= 1e3) {
        oss << bitThroughput / 1e3 << " Kbps";
    } else {
        oss << bitThroughput << " bps";
    }
    
    oss << " (" << std::setprecision(2);
    
    // 格式化字节吞吐量
    if (byteThroughput >= 1e9) {
        oss << byteThroughput / 1e9 << " GB/s";
    } else if (byteThroughput >= 1e6) {
        oss << byteThroughput / 1e6 << " MB/s";
    } else if (byteThroughput >= 1e3) {
        oss << byteThroughput / 1e3 << " KB/s";
    } else {
        oss << byteThroughput << " B/s";
    }
    
    oss << ", " << std::setprecision(2) << packetThroughput << " pps)";
    
    return oss.str();
}

} // namespace metrics
} // namespace simulation
} // namespace link16
