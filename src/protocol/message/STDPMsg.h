#pragma once
#include "core/utils/tools.h"
#include "word/HeaderWord.h"
#include "word/InitialWord.h"
#include "word/ExtendWord.h"
#include "word/ContinueWord.h"
#include <string>
#include <vector>
#include <memory>

namespace link16 {
namespace protocol {

/**
 * @brief STDP消息类，封装Link16标准消息
 */
class STDPMsg {
public:
    // 构造函数
    STDPMsg();
    
    // 析构函数
    ~STDPMsg();
    
    // 格式化消息
    bool formatMessage(int n, int m, const std::string& message);
    
    // 解析消息
    bool parseMessage(int& n, int& m, std::string& message);
    
    // 获取原始消息
    const std::string& getRawMsg() const;
    
    // 设置原始消息
    void setRawMsg(const std::string& rawMsg);
    
    // 获取比特消息
    const std::string& getBitMsg() const;
    
    // 设置比特消息
    void setBitMsg(const std::string& bitMsg);
    
    // 获取HeaderWord
    HeaderWord* getHeaderWord();
    
    // 获取InitialWord
    InitialWord* getInitialWord();
    
    // 获取ExtendWord
    ExtendWord* getExtendWord();
    
    // 获取ContinueWord
    ContinueWord* getContinueWord();
    
    // 设置HeaderWord
    void setHeaderWord(const std::string& bitData);
    
    // 设置InitialWord
    void setInitialWord(const std::string& bitData);
    
    // 设置ExtendWord
    void setExtendWord(const std::string& bitData);
    
    // 设置ContinueWord
    void setContinueWord(const std::string& bitData);
    
    // 清除所有数据
    void clear();
    
    // 计算BIP校验
    bool calculateBIP();
    
    // 验证BIP校验
    bool verifyBIP();
    
    // 获取消息类型
    bool getMessageType(int& n, int& m);
    
    // 设置消息类型
    bool setMessageType(int n, int m);
    
    // 获取消息优先级
    int getMessagePriority();
    
    // 设置消息优先级
    void setMessagePriority(int priority);
    
    // 获取发送方ID
    int getSenderID();
    
    // 设置发送方ID
    void setSenderID(int senderID);
    
    // 获取接收方ID
    int getReceiverID();
    
    // 设置接收方ID
    void setReceiverID(int receiverID);

private:
    // 原始消息
    std::string m_rawMsg;
    
    // 比特消息
    std::string m_bitMsg;
    
    // HeaderWord
    std::unique_ptr<HeaderWord> m_headerWord;
    
    // InitialWord
    std::unique_ptr<InitialWord> m_initialWord;
    
    // ExtendWord
    std::unique_ptr<ExtendWord> m_extendWord;
    
    // ContinueWord
    std::unique_ptr<ContinueWord> m_continueWord;
    
    // 消息优先级
    int m_priority;
    
    // 发送方ID
    int m_senderID;
    
    // 接收方ID
    int m_receiverID;
};

} // namespace protocol
} // namespace link16
