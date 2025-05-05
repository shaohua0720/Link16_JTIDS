#pragma once
#include "dataType.h"

// Link16特定类型定义
namespace link16 {

    // 消息类型
    enum class MessageType {
        J_SERIES,
        R_SERIES,
        OTHER
    };

    // 字类型
    enum class WordType {
        HEADER,
        INITIAL,
        EXTEND,
        CONTINUE
    };

    // 物理层参数
    struct PhysicalParams {
        double frequency;      // 频率(Hz)
        double bandwidth;      // 带宽(Hz)
        double power;          // 功率(dBm)
        int hoppingPattern;    // 跳频模式
    };

    // 仿真参数
    struct SimulationParams {
        double snr;            // 信噪比(dB)
        double ber;            // 误码率
        double duration;       // 仿真时长(s)
        int iterations;        // 迭代次数
    };

} // namespace link16
