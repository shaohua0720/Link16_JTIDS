#include "simulation/EndToEndSimulation.h"
#include "core/utils/logger.h"
#include "core/utils/fileUtils.h"
#include "simulation/channel/awgn/AWGNChannel.h"
#include "simulation/channel/fading/RayleighChannel.h"
#include "simulation/metrics/BER.h"
#include "simulation/metrics/Throughput.h"
#include "physical/modulation/digital/PSK/BPSKModulator.h"
#include "physical/modulation/digital/PSK/QPSKModulator.h"
#include "coding/error_correction/reed_solomon/RSCoder.h"
#include "coding/error_detection/crc/CRCCoder.h"
#include "coding/interleaving/matrix/MatrixInterleaver.h"
#include <fstream>
#include <random>
#include <chrono>

namespace link16 {
namespace simulation {

// 构造函数
EndToEndSimulation::EndToEndSimulation() : initialized(false), running(false), progress(0.0) {
}

// 析构函数
EndToEndSimulation::~EndToEndSimulation() {
}

// 初始化
bool EndToEndSimulation::initialize(const std::string& configFile) {
    LOG_INFO("初始化端到端仿真");

    // 如果提供了配置文件，从文件加载配置
    if (!configFile.empty()) {
        engine::SimulationConfig config;
        if (config.loadFromFile(configFile)) {
            simParams = config.getSimulationParams();
            physParams = config.getPhysicalParams();
            channelType = config.getChannelType();
        } else {
            LOG_WARNING("无法从文件加载配置，使用默认配置");
        }
    }

    // 设置默认测试消息
    testMessage = "Link16 End-to-End Simulation Test Message";

    // 创建仿真引擎
    simulationEngine = std::make_shared<engine::SimulationEngine>();

    // 初始化仿真引擎
    engine::SimulationConfig config;
    config.setSimulationParams(simParams);
    config.setPhysicalParams(physParams);
    config.setChannelType(channelType);

    if (!simulationEngine->initialize(config)) {
        LOG_ERROR("初始化仿真引擎失败");
        return false;
    }

    initialized = true;
    running = false;
    progress = 0.0;

    LOG_INFO("端到端仿真初始化完成");
    return true;
}

// 关闭
void EndToEndSimulation::shutdown() {
    if (!initialized) {
        return;
    }

    LOG_INFO("关闭端到端仿真");

    if (running) {
        stop();
    }

    initialized = false;
}

// 运行仿真
bool EndToEndSimulation::run() {
    if (!initialized) {
        LOG_ERROR("端到端仿真未初始化，无法运行");
        return false;
    }

    if (running) {
        LOG_WARNING("端到端仿真已经在运行");
        return true;
    }

    LOG_INFO("开始运行端到端仿真");

    // 记录开始时间
    auto startTime = std::chrono::high_resolution_clock::now();

    // 设置运行状态
    running = true;
    progress = 0.0;
    results.clear();

    // 创建信道模型
    std::shared_ptr<channel::ChannelModel> channelModel;
    if (channelType == "AWGN") {
        channelModel = std::make_shared<channel::AWGNChannel>(simParams.snr);
    } else if (channelType == "Rayleigh") {
        channelModel = std::make_shared<channel::RayleighChannel>(simParams.snr);
    } else {
        LOG_ERROR("不支持的信道类型: " + channelType);
        running = false;
        return false;
    }

    // 设置信道模型
    simulationEngine->setChannelModel(channelModel);

    // 创建调制器
    std::shared_ptr<physical::modulation::BPSKModulator> modulator = std::make_shared<physical::modulation::BPSKModulator>();
    modulator->initialize();

    // 创建RS编码器
    coding::error_correction::RSCoder rsCoder;

    // 创建交织器
    coding::interleaving::MatrixInterleaver interleaver;

    // 创建BER计算器
    metrics::BER berCalculator;

    // 创建吞吐量计算器
    metrics::Throughput throughputCalculator;
    throughputCalculator.start();

    // 仿真主循环
    for (int i = 0; i < simParams.iterations; ++i) {
        // 更新进度
        progress = static_cast<double>(i) / simParams.iterations;

        // 生成随机数据
        std::string data = testMessage + " - Iteration " + std::to_string(i + 1);

        // 编码数据
        std::string encodedData = data;

        // 交织
        std::string interleavedData = interleaver.interleave(encodedData, 8, 8);

        // 调制
        std::vector<std::complex<double>> modulatedSignal = modulator->modulate(interleavedData);

        // 通过信道
        std::vector<std::complex<double>> receivedSignal = channelModel->process(modulatedSignal);

        // 解调
        std::string demodulatedData = modulator->demodulate(receivedSignal);

        // 解交织
        std::string deinterleavedData = interleaver.deinterleave(demodulatedData, 8, 8);

        // 解码
        std::string decodedData = deinterleavedData;

        // 计算误码率
        double ber = berCalculator.calculate(data, decodedData);

        // 记录结果
        results.push_back(ber);

        // 更新吞吐量
        throughputCalculator.addBits(data.length() * 8);
        throughputCalculator.addPackets(1);
    }

    // 停止吞吐量计算
    throughputCalculator.stop();

    // 记录结束时间
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    LOG_INFO("端到端仿真完成，耗时: " + std::to_string(duration) + " ms");
    LOG_INFO("平均误码率: " + std::to_string(berCalculator.calculateAverage(results)));
    LOG_INFO("吞吐量: " + throughputCalculator.getFormattedThroughput());

    // 设置运行状态
    running = false;
    progress = 1.0;

    return true;
}

// 停止仿真
void EndToEndSimulation::stop() {
    if (!running) {
        return;
    }

    LOG_INFO("停止端到端仿真");
    running = false;
}

// 设置仿真参数
void EndToEndSimulation::setSimulationParams(const SimulationParams& params) {
    simParams = params;
}

// 获取仿真参数
SimulationParams EndToEndSimulation::getSimulationParams() const {
    return simParams;
}

// 设置物理层参数
void EndToEndSimulation::setPhysicalParams(const PhysicalParams& params) {
    physParams = params;
}

// 获取物理层参数
PhysicalParams EndToEndSimulation::getPhysicalParams() const {
    return physParams;
}

// 设置信道类型
void EndToEndSimulation::setChannelType(const std::string& type) {
    channelType = type;
}

// 获取信道类型
std::string EndToEndSimulation::getChannelType() const {
    return channelType;
}

// 设置测试消息
void EndToEndSimulation::setTestMessage(const std::string& message) {
    testMessage = message;
}

// 获取测试消息
std::string EndToEndSimulation::getTestMessage() const {
    return testMessage;
}

// 获取仿真结果
std::vector<double> EndToEndSimulation::getResults() const {
    return results;
}

// 保存仿真结果
bool EndToEndSimulation::saveResults(const std::string& resultFile) {
    std::ofstream file(resultFile);
    if (!file.is_open()) {
        LOG_ERROR("无法打开文件进行写入: " + resultFile);
        return false;
    }

    file << "# Link16端到端仿真结果\n";
    file << "# 生成时间: " << __DATE__ << " " << __TIME__ << "\n\n";

    file << "# 仿真参数\n";
    file << "SNR = " << simParams.snr << " dB\n";
    file << "持续时间 = " << simParams.duration << " s\n";
    file << "迭代次数 = " << simParams.iterations << "\n\n";

    file << "# 物理层参数\n";
    file << "频率 = " << physParams.frequency / 1e6 << " MHz\n";
    file << "带宽 = " << physParams.bandwidth / 1e6 << " MHz\n";
    file << "功率 = " << physParams.power << " dBm\n";
    file << "跳频模式 = " << physParams.hoppingPattern << "\n\n";

    file << "# 信道类型\n";
    file << channelType << "\n\n";

    file << "# 仿真结果\n";
    file << "迭代,误码率\n";

    for (size_t i = 0; i < results.size(); ++i) {
        file << i + 1 << "," << results[i] << "\n";
    }

    file.close();
    LOG_INFO("仿真结果已保存到: " + resultFile);

    return true;
}

// 检查仿真是否正在运行
bool EndToEndSimulation::isRunning() const {
    return running;
}

// 获取仿真进度
double EndToEndSimulation::getProgress() const {
    return progress;
}

} // namespace simulation
} // namespace link16
