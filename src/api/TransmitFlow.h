#pragma once

#include <string>
#include <memory>
#include "protocol/MessageProcessor.h"
#include "coding/CodingProcessor.h"
#include "physical/PhysicalProcessor.h"

namespace link16 {
namespace api {

/**
 * @brief 发送流程类
 */
class TransmitFlow {
public:
    /**
     * @brief 构造函数
     * @param messageProcessor 消息处理器
     * @param codingProcessor 编码处理器
     * @param physicalProcessor 物理处理器
     */
    TransmitFlow(
        std::shared_ptr<protocol::MessageProcessor> messageProcessor,
        std::shared_ptr<coding::CodingProcessor> codingProcessor,
        std::shared_ptr<physical::PhysicalProcessor> physicalProcessor
    );
    
    /**
     * @brief 析构函数
     */
    ~TransmitFlow();
    
    /**
     * @brief 发送消息
     * @param n 消息大类
     * @param m 消息子类
     * @param message 消息内容
     * @return 是否发送成功
     */
    bool transmitMessage(int n, int m, const std::string& message);
    
    /**
     * @brief 设置发送方ID
     * @param senderID 发送方ID
     */
    void setSenderID(int senderID);
    
    /**
     * @brief 设置接收方ID
     * @param receiverID 接收方ID
     */
    void setReceiverID(int receiverID);
    
    /**
     * @brief 设置加密密钥
     * @param key 加密密钥
     */
    void setEncryptionKey(const std::string& key);
    
    /**
     * @brief 设置调制方式
     * @param modulationType 调制方式
     */
    void setModulationType(const std::string& modulationType);
    
    /**
     * @brief 设置跳频模式
     * @param hoppingPattern 跳频模式
     */
    void setHoppingPattern(const std::string& hoppingPattern);

private:
    // 消息处理器
    std::shared_ptr<protocol::MessageProcessor> messageProcessor;
    
    // 编码处理器
    std::shared_ptr<coding::CodingProcessor> codingProcessor;
    
    // 物理处理器
    std::shared_ptr<physical::PhysicalProcessor> physicalProcessor;
    
    // 发送方ID
    int senderID;
    
    // 接收方ID
    int receiverID;
    
    // 加密密