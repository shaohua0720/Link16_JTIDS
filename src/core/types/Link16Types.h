#pragma once
#include "dataType.h"
#include <string>
#include <vector>
#include <complex>
#include <cstdint>

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
        double sampleRate;       // 采样率(Hz)
        double symbolRate;       // 符号率(Hz)
        double frequency;        // 中心频率(Hz)
        double bandwidth;        // 带宽(Hz)
        double txGain;           // 发送增益(dB)
        double rxGain;           // 接收增益(dB)
        std::string modulation;  // 调制方式
        int hoppingPattern;      // 跳频模式
        double power;            // 功率(dBm)

        // 构造函数
        PhysicalParams()
            : sampleRate(1.0e6), symbolRate(1.0e5), frequency(969.0e6),
              bandwidth(1.0e6), txGain(20.0), rxGain(30.0),
              modulation("BPSK"), hoppingPattern(1), power(10.0) {
        }
    };

    // 仿真参数
    struct SimulationParams {
        double snr;              // 信噪比(dB)
        double ber;              // 误码率
        double duration;         // 仿真时长(s)
        int iterations;          // 迭代次数
        size_t numSamples;       // 样本数
        size_t numTrials;        // 试验次数
        double timeStep;         // 时间步长(秒)

        // 构造函数
        SimulationParams()
            : snr(10.0), ber(0.0), duration(1.0), iterations(100),
              numSamples(1000), numTrials(10), timeStep(0.001) {
        }
    };

    // 信道参数
    struct ChannelParams {
        double snr;              // 信噪比(dB)
        double fadingFactor;     // 衰落因子
        double dopplerShift;     // 多普勒频移(Hz)
        double pathLoss;         // 路径损耗(dB)
        double delaySpread;      // 时延扩展(秒)

        // 构造函数
        ChannelParams()
            : snr(10.0), fadingFactor(0.0), dopplerShift(0.0),
              pathLoss(0.0), delaySpread(0.0) {
        }
    };

    // 仿真结果
    struct SimulationResults {
        std::vector<double> ber;         // 误码率
        std::vector<double> throughput;  // 吞吐量
        std::vector<double> latency;     // 延迟
        double averageBER;               // 平均误码率
        double averageThroughput;        // 平均吞吐量
        double averageLatency;           // 平均延迟

        // 构造函数
        SimulationResults()
            : averageBER(0.0), averageThroughput(0.0), averageLatency(0.0) {
        }
    };

    namespace channel {
        // 信道模型基类
        class ChannelModel {
        public:
            virtual ~ChannelModel() {}

            // 处理信号
            virtual void processSignal(const std::vector<std::complex<double>>& input,
                                      std::vector<std::complex<double>>& output) = 0;

            // 设置信噪比
            virtual void setSnr(double snr) = 0;

            // 获取信噪比
            virtual double getSnr() const = 0;
        };
    }

    namespace engine {
        // 仿真引擎
        class SimulationEngine {
        public:
            virtual ~SimulationEngine() {}

            // 初始化
            virtual bool initialize() = 0;

            // 运行仿真
            virtual bool run() = 0;

            // 停止仿真
            virtual void stop() = 0;

            // 获取进度
            virtual double getProgress() const = 0;

            // 获取结果
            virtual SimulationResults getResults() const = 0;
        };
    }

} // namespace link16
