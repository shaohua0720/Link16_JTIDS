#include "Link16App.h"
#include "core/utils/logger.h"
#include "core/config/SystemConfig.h"
#include "protocol/interface/interface.h"
#include "protocol/message/STDPMsg.h"
#include "protocol/formats/J_Series.h"  // 添加J_Series头文件
#include "coding/BIPCoder.h"
#include "coding/ReedSolomon.h"
#include "coding/AESCrypto.h"
#include "coding/Interleaver.h"
#include "physical/Modulator.h"
#include "physical/FrequencyHopping.h"
#include "physical/Transmitter.h"
#include "physical/Receiver.h"
#include "utils/Tools.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

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
    
    // 初始化编码器和调制器
    try {
        // 初始化BIP编码器
        bipCoder = std::make_unique<coding::BIPCoder>();
        
        // 初始化Reed-Solomon编码器
        rsCoder = std::make_unique<coding::ReedSolomon>();
        
        // 初始化AES加密
        aesCrypto = std::make_unique<coding::AESCrypto>();
        
        // 初始化交织器
        interleaver = std::make_unique<coding::Interleaver>();
        
        // 初始化调制器
        modulator = std::make_unique<physical::Modulator>();
        
        // 初始化跳频器
        frequencyHopping = std::make_unique<physical::FrequencyHopping>();
        
        // 初始化发射器
        transmitter = std::make_unique<physical::Transmitter>();
        
        // 初始化接收器
        receiver = std::make_unique<physical::Receiver>();
    }
    catch (const std::exception& e) {
        LOG_ERROR("初始化组件失败: " + std::string(e.what()));
        return false;
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
        // 1. 检查消息类型是否有效
        std::string type = std::to_string(n) + " " + std::to_string(m);
        if (!isValidMessageType(n, m)) {
            LOG_ERROR("无效的消息类型: J" + std::to_string(n) + "." + std::to_string(m));
            return false;
        }
        
        // 2. 协议层处理 - 格式化Link16消息
        protocol::STDPMsg stdpMsg;
        if (!stdpMsg.formatMessage(n, m, message)) {
            LOG_ERROR("消息格式化失败");
            return false;
        }
        
        // 3. 编码层处理 - BIP奇偶校验
        std::string checkedData;
        if (!bipCoder->encode(stdpMsg.getRawMsg(), checkedData)) {
            LOG_ERROR("BIP校验编码失败");
            return false;
        }
        
        // 4. 编码层处理 - Reed-Solomon编码
        std::string encodedData;
        if (!rsCoder->encode(checkedData, encodedData)) {
            LOG_ERROR("Reed-Solomon编码失败");
            return false;
        }
        
        // 5. 编码层处理 - 加密
        std::string encryptedData;
        if (!aesCrypto->encrypt(encodedData, encryptedData)) {
            LOG_ERROR("AES加密失败");
            return false;
        }
        
        // 6. 编码层处理 - 交织
        std::string interleavedData;
        if (!interleaver->interleave(encryptedData, interleavedData)) {
            LOG_ERROR("数据交织失败");
            return false;
        }
        
        // 7. 物理层处理 - 调制
        std::vector<std::complex<double>> modulatedSignal;
        if (!modulator->modulate(interleavedData, modulatedSignal)) {
            LOG_ERROR("信号调制失败");
            return false;
        }
        
        // 8. 物理层处理 - 跳频
        std::vector<std::complex<double>> hoppedSignal;
        if (!frequencyHopping->applyHopping(modulatedSignal, hoppedSignal)) {
            LOG_ERROR("跳频处理失败");
            return false;
        }
        
        // 9. 物理层处理 - 发送
        if (!transmitter->transmit(hoppedSignal)) {
            LOG_ERROR("信号发送失败");
            return false;
        }
        
        // 10. 可选：保存到文件（用于调试或备份）
        if (!dataPath.empty()) {
            if (!utils::Tools::writeMessage(dataPath, interleavedData)) {
                LOG_WARNING("无法将消息数据保存到文件: " + dataPath);
            } else {
                LOG_INFO("消息数据已保存到文件: " + dataPath);
            }
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
        std::vector<std::complex<double>> receivedSignal;
        if (!receiver->receive(receivedSignal)) {
            // 如果物理接收失败，尝试从文件读取（用于调试或备份）
            if (!dataPath.empty() && utils::Tools::fileExists(dataPath)) {
                LOG_INFO("尝试从文件读取消息数据: " + dataPath);
                std::string fileData = utils::Tools::readMessage(dataPath);
                if (fileData.empty()) {
                    LOG_ERROR("从文件读取消息数据失败");
                    return false;
                }
                
                // 直接进入解交织步骤，跳过物理层处理
                std::string deinterleavedData;
                if (!interleaver->deinterleave(fileData, deinterleavedData)) {
                    LOG_ERROR("数据解交织失败");
                    return false;
                }
                
                // 继续处理解密步骤
                std::string decryptedData;
                if (!aesCrypto->decrypt(deinterleavedData, decryptedData)) {
                    LOG_ERROR("AES解密失败");
                    return false;
                }
                
                // 继续处理RS解码步骤
                std::string decodedData;
                if (!rsCoder->decode(decryptedData, decodedData)) {
                    LOG_ERROR("Reed-Solomon解码失败");
                    return false;
                }
                
                // BIP校验
                if (!bipCoder->validate(decodedData)) {
                    LOG_WARNING("BIP校验失败，数据可能已损坏");
                    // 可以选择继续处理或返回错误
                }
                
                // 解析消息
                protocol::STDPMsg stdpMsg;
                stdpMsg.setRawMsg(decodedData);
                if (!stdpMsg.parseMessage(n, m, message)) {
                    LOG_ERROR("消息解析失败");
                    return false;
                }
                
                LOG_INFO("从文件成功接收到消息: J" + std::to_string(n) + "." + std::to_string(m));
                return true;
            } else {
                LOG_ERROR("信号接收失败且无备份文件");
                return false;
            }
        }
        
        // 2. 物理层处理 - 解跳频
        std::vector<std::complex<double>> dehoppedSignal;
        if (!frequencyHopping->removeHopping(receivedSignal, dehoppedSignal)) {
            LOG_ERROR("解跳频处理失败");
            return false;
        }
        
        // 3. 物理层处理 - 解调
        std::string demodulatedData;
        if (!modulator->demodulate(dehoppedSignal, demodulatedData)) {
            LOG_ERROR("信号解调失败");
            return false;
        }
        
        // 4. 编码层处理 - 解交织
        std::string deinterleavedData;
        if (!interleaver->deinterleave(demodulatedData, deinterleavedData)) {
            LOG_ERROR("数据解交织失败");
            return false;
        }
        
        // 5. 编码层处理 - 解密
        std::string decryptedData;
        if (!aesCrypto->decrypt(deinterleavedData, decryptedData)) {
            LOG_ERROR("AES解密失败");
            return false;
        }
        
        // 6. 编码层处理 - Reed-Solomon解码
        std::string decodedData;
        if (!rsCoder->decode(decryptedData, decodedData)) {
            LOG_ERROR("Reed-Solomon解码失败");
            return false;
        }
        
        // 7. 编码层处理 - BIP校验
        if (!bipCoder->validate(decodedData)) {
            LOG_WARNING("BIP校验失败，数据可能已损坏");
            // 可以选择继续处理或返回错误
        }
        
        // 8. 协议层处理 - 解析Link16消息
        protocol::STDPMsg stdpMsg;
        stdpMsg.setRawMsg(decodedData);
        if (!stdpMsg.parseMessage(n, m, message)) {
            LOG_ERROR("消息解析失败");
            return false;
        }
        
        // 9. 可选：保存接收到的数据（用于调试或分析）
        if (!dataPath.empty()) {
            std::string backupPath = dataPath + ".received";
            if (!utils::Tools::writeMessage(backupPath, demodulatedData)) {
                LOG_WARNING("无法将接收数据保存到文件: " + backupPath);
            } else {
                LOG_INFO("接收数据已保存到文件: " + backupPath);
            }
        }
        
        LOG_INFO("接收到消息: J" + std::to_string(n) + "." + std::to_string(m) + " - " + message);
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("消息接收过程中发生异常: " + std::string(e.what()));
        return false;
    }
}

// 检查消息类型是否有效
bool Link16App::isValidMessageType(int n, int m) {
    LOG_INFO("验证消息类型: J" + std::to_string(n) + "." + std::to_string(m));
    
    // 使用J_Series类验证消息类型
    protocol::formats::J_Series jSeries;
    
    // 首先检查基本范围
    if (n < 0 || n > 31) {
        LOG_ERROR("无效的消息类型N值: " + std::to_string(n) + "（应在0-31范围内）");
        return false;
    }
    
    if (m < 0 || m > 7) {
        LOG_ERROR("无效的消息类型M值: " + std::to_string(m) + "（应在0-7范围内）");
        return false;
    }
    
    // 使用J_Series验证消息类型
    if (!jSeries.isValidMessageType(n, m)) {
        LOG_WARNING("未定义的消息类型: J" + std::to_string(n) + "." + std::to_string(m));
        // 注意：这里只是警告，不一定要返回false
        // 因为可能有些应用允许使用未在标准中定义的消息类型
        // return false;  // 取消注释如果要严格验证
    } else {
        // 获取并记录消息类型描述
        std::string description = jSeries.getMessageTypeDescription(n, m);
        LOG_INFO("消息类型: " + description);
    }
    
    return true;
}

// 关闭应用
void Link16App::shutdown() {
    if (!initialized) {
        return;
    }
    
    LOG_INFO("正在关闭Link16应用...");
    
    // 释放资源
    bipCoder.reset();
    rsCoder.reset();
    aesCrypto.reset();
    interleaver.reset();
    modulator.reset();
    frequencyHopping.reset();
    transmitter.reset();
    receiver.reset();
    
    initialized = false;
    LOG_INFO("Link16应用已关闭");
}

} // namespace application
} // namespace link16
