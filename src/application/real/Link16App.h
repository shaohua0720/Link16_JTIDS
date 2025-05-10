#pragma once
#include <string>
#include <memory>

namespace link16 {

// 前向声明
namespace coding {
    class BIPCoder;
    class ReedSolomon;
    class AESCrypto;
    class Interleaver;
}

namespace physical {
    class Modulator;
    class FrequencyHopping;
    class Transmitter;
    class Receiver;
}

namespace application {

class Link16App {
public:
    // 构造函数
    Link16App();
    
    // 析构函数
    ~Link16App();
    
    // 初始化应用
    bool initialize();
    
    // 运行应用
    void run();
    
    // 发送消息
    bool sendMessage(int n, int m, const std::string& message);
    
    // 接收消息
    bool receiveMessage(std::string& message, int& n, int& m);
    
    // 关闭应用
    void shutdown();

private:
    // 检查消息类型是否有效
    bool isValidMessageType(int n, int m);
    
    // 应用状态
    bool initialized;
    
    // 配置文件路径
    std::string configPath;
    
    // 数据文件路径
    std::string dataPath;
    
    // 编码组件
    std::unique_ptr<coding::BIPCoder> bipCoder;
    std::unique_ptr<coding::ReedSolomon> rsCoder;
    std::unique_ptr<coding::AESCrypto> aesCrypto;
    std::unique_ptr<coding::Interleaver> interleaver;
    
    // 物理层组件
    std::unique_ptr<physical::Modulator> modulator;
    std::unique_ptr<physical::FrequencyHopping> frequencyHopping;
    std::unique_ptr<physical::Transmitter> transmitter;
    std::unique_ptr<physical::Receiver> receiver;
};

} // namespace application
} // namespace link16
