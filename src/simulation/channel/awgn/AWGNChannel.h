#pragma once
#include "../base/ChannelModel.h"

namespace link16 {
namespace simulation {
namespace channel {

// 加性高斯白噪声信道模型
class AWGNChannel : public ChannelModel {
public:
    // 构造函数
    AWGNChannel(double snr = 10.0);
    
    // 析构函数
    virtual ~AWGNChannel();
    
    // 处理信号
    virtual std::vector<std::complex<double>> process(const std::vector<std::complex<double>>& input) override;
    
    // 获取信道名称
    virtual std::string getName() const override;
    
    // 获取信道描述
    virtual std::string getDescription() const override;
};

} // namespace channel
} // namespace simulation
} // namespace link16
