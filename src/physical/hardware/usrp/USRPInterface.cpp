#include "USRPInterface.h"
#include "core/utils/logger.h"
#include <iostream>

namespace link16 {
namespace physical {
namespace hardware {

// 获取可用设备列表
std::vector<std::string> USRPInterface::getAvailableDevices() {
    // 这里是一个模拟实现，实际应该使用UHD库
    LOG_INFO("获取可用USRP设备列表");

    std::vector<std::string> devices;
    devices.push_back("type=b200");
    devices.push_back("type=x300,addr=192.168.10.2");
    devices.push_back("type=n200,addr=192.168.10.3");

    return devices;
}

} // namespace hardware
} // namespace physical
} // namespace link16
