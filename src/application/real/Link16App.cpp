#include "Link16App.h"
#include "core/utils/logger.h"
#include "core/config/SystemConfig.h"
#include "protocol/interface/interface.h"

namespace link16 {
namespace application {

// 构造函数
Link16App::Link16App() : initialized(false), configPath("config.ini"), dataPath("data.txt") {
}

// 析构函数
Link16App::~Link16App() {
    if (initialized) {
        shutdown();
    }
}

// 初始化应用
bool Link16App::initialize() {
    LOG_INFO("正在初始化Link16应用...");
    
    // 加载配置
    config::SystemConfig& config = config::SystemConfig::getInstance();
    if (!config.loadConfigFile(configPath)) {
        LOG_WARNING("无法加载配置文件，使用默认配置");
    }
    
    // 获取数据文件路径
    dataPath = config.getConfigValue("data_file_path");
    if (dataPath.empty()) {
        dataPath = "data.txt";
    }
    
    initialized = true;
    LOG_INFO("Link16应用初始化完成");
    return true;
}

// 运行应用
void Link16App::run() {
    if (!initialized) {
        LOG_ERROR("应用未初始化，无法运行");
        return;
    }
    
    LOG_INFO("Link16应用开始运行");
    
    // 应用主循环
    // 这里将添加实际的应用逻辑
    
    LOG_INFO("Link16应用运行结束");
}

// 发送消息
bool Link16App::sendMessage(int n, int m, const std::string& message) {
    if (!initialized) {
        LOG_ERROR("应用未初始化，无法发送消息");
        return false;
    }
    
    LOG_INFO("发送消息: J" + std::to_string(n) + "." + std::to_string(m) + " - " + message);
    
    try {
        // 1. 协议层处理 - 格式化Link16消息
        protocol::STDPMsg stdpMsg;
        if (!stdpMsg.formatMessage(n, m, message)) {
            LOG_ERROR("消息格式化失败");
            return false;
        }
        
        // 2. 编码层处理 - Reed-Solomon编码
        coding::ReedSolomon rs;
        std::string encodedData = rs.encode(stdpMsg.getRawMsg());
        
        // 3. 编码层处理 - 加密
        coding::AESCrypto aes;
        std::string encryptedData = aes.encrypt(encodedData);
        
        // 4. 编码层处理 - 交织
        coding::Interleaver interleaver;
        std::string interleavedData = interleaver.interleave(encryptedData);
        
        // 5. 物理层处理 - 调制
        physical::Modulator modulator;
        std::vector<std::complex<double>> modulatedSignal = modulator.modulate(interleavedData);
        
        // 6. 物理层处理 - 跳频
        physical::FrequencyHopping hopping;
        std::vector<std::complex<double>> hoppedSignal = hopping.applyHopping(modulatedSignal);
        
        // 7. 物理层处理 - 发送
        physical::Transmitter transmitter;
        if (!transmitter.transmit(hoppedSignal)) {
            LOG_ERROR("信号发送失败");
            return false;
        }
        
        LOG_INFO("消息发送成功");
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("消息发送过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

// 接收消息
bool Link16App::receiveMessage(std::string& message, int& n, int& m) {
    if (!initialized) {
        LOG_ERROR("应用未初始化，无法接收消息");
        return false;
    }
    
    LOG_INFO("正在接收消息...");
    
    try {
        // 1. 物理层处理 - 接收
        physical::Receiver receiver;
        std::vector<std::complex<double>> receivedSignal;
        if (!receiver.receive(receivedSignal)) {
            LOG_ERROR("信号接收失败");
            return false;
        }
        
        // 2. 物理层处理 - 解跳频
        physical::FrequencyHopping hopping;
        std::vector<std::complex<double>> dehoppedSignal = hopping.removeHopping(receivedSignal);
        
        // 3. 物理层处理 - 解调
        physical::Modulator modulator;
        std::string demodulatedData = modulator.demodulate(dehoppedSignal);
        
        // 4. 编码层处理 - 解交织
        coding::Interleaver interleaver;
        std::string deinterleavedData = interleaver.deinterleave(demodulatedData);
        
        // 5. 编码层处理 - 解密
        coding::AESCrypto aes;
        std::string decryptedData = aes.decrypt(deinterleavedData);
        
        // 6. 编码层处理 - Reed-Solomon解码
        coding::ReedSolomon rs;
        std::string decodedData = rs.decode(decryptedData);
        
        // 7. 协议层处理 - 解析Link16消息
        protocol::STDPMsg stdpMsg;
        stdpMsg.setRawMsg(decodedData);
        if (!stdpMsg.parseMessage(n, m, message)) {
            LOG_ERROR("消息解析失败");
            return false;
        }
        
        LOG_INFO("接收到消息: J" + std::to_string(n) + "." + std::to_string(m) + " - " + message);
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("消息接收过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

// 关闭应用
void Link16App::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("正在关闭Link16应用...");
    
    // 执行清理操作
    
    initialized = false;
    LOG_INFO("Link16应用已关闭");
}

} // namespace application
} // namespace link16
