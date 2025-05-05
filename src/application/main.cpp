#include <iostream>
#include "core/utils/logger.h"
#include "core/config/SystemConfig.h"
#include "include/link16/Link16.h"

int main(int argc, char* argv[]) {
    // 初始化日志系统
    link16::utils::Logger::getInstance().initialize("link16.log");
    LOG_INFO("Link16应用程序启动");

    // 加载系统配置
    link16::config::SystemConfig& config = link16::config::SystemConfig::getInstance();
    config.loadConfigFile("config.ini");

    LOG_INFO("系统配置加载完成");

    // 应用程序主逻辑
    std::cout << "Link16通信系统" << std::endl;
    std::cout << "版本: " << link16::getVersionString() << std::endl;
    std::cout << "==================" << std::endl;

    // 初始化Link16库
    if (!link16::initialize()) {
        LOG_ERROR("Link16库初始化失败");
        return 1;
    }

    LOG_INFO("Link16库初始化成功");

    // 使用消息API示例
    int32_t n = 7;
    int32_t m = 3;
    std::string message = "Hello, Link16!";
    std::string encodedMessage;

    if (link16::api::encodeMessage(n, m, message, encodedMessage)) {
        LOG_INFO("消息编码成功");

        // 解码消息
        int32_t decodedN, decodedM;
        std::string decodedMessage;

        if (link16::api::decodeMessage(encodedMessage, decodedN, decodedM, decodedMessage)) {
            LOG_INFO("消息解码成功");
            std::cout << "解码后的消息: " << decodedMessage << std::endl;
            std::cout << "消息类型: n=" << decodedN << ", m=" << decodedM << std::endl;
        } else {
            LOG_ERROR("消息解码失败");
        }
    } else {
        LOG_ERROR("消息编码失败");
    }

    // 使用编码API示例
    std::string data = "Test data for RS coding";
    std::string rsEncodedData;

    if (link16::api::rsEncode(data, rsEncodedData)) {
        LOG_INFO("RS编码成功");

        // 解码数据
        std::string rsDecodedData;

        if (link16::api::rsDecode(rsEncodedData, rsDecodedData)) {
            LOG_INFO("RS解码成功");
            std::cout << "解码后的数据: " << rsDecodedData << std::endl;
        } else {
            LOG_ERROR("RS解码失败");
        }
    } else {
        LOG_ERROR("RS编码失败");
    }

    // 使用物理层API示例
    if (link16::api::initializeUSRP()) {
        LOG_INFO("USRP初始化成功");

        // 设置参数
        link16::api::setTxFrequency(969.0e6);
        link16::api::setRxFrequency(969.0e6);
        link16::api::setTxGain(20.0);
        link16::api::setRxGain(30.0);
        link16::api::setSampleRate(1.0e6);
        link16::api::setModulation("BPSK");

        // 发送数据
        std::string txData = "Test data for transmission";
        if (link16::api::transmit(txData)) {
            LOG_INFO("数据发送成功");
        } else {
            LOG_ERROR("数据发送失败");
        }

        // 接收数据
        std::string rxData;
        if (link16::api::receive(rxData, 1000)) {
            LOG_INFO("数据接收成功");
            std::cout << "接收到的数据: " << rxData << std::endl;
        } else {
            LOG_ERROR("数据接收失败");
        }

        // 关闭USRP
        link16::api::closeUSRP();
    } else {
        LOG_ERROR("USRP初始化失败");
    }

    // 使用仿真API示例
    link16::SimulationParams simParams;
    simParams.snr = 10.0;
    simParams.numSamples = 1000;
    simParams.numTrials = 10;

    link16::api::setSimulationParams(simParams);
    link16::api::setChannelType("AWGN");

    if (link16::api::runSimulation()) {
        LOG_INFO("仿真运行成功");

        // 获取结果
        std::vector<double> results = link16::api::getSimulationResults();
        std::cout << "仿真结果: " << std::endl;
        for (size_t i = 0; i < results.size() && i < 5; ++i) {
            std::cout << "  " << i << ": " << results[i] << std::endl;
        }

        // 保存结果
        if (link16::api::saveSimulationResults("simulation_results.txt")) {
            LOG_INFO("仿真结果保存成功");
        } else {
            LOG_ERROR("仿真结果保存失败");
        }
    } else {
        LOG_ERROR("仿真运行失败");
    }

    // 关闭Link16库
    link16::shutdown();

    LOG_INFO("Link16应用程序正常退出");
    link16::utils::Logger::getInstance().shutdown();

    return 0;
}
