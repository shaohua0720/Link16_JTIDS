#include "include/link16/Link16.h"
#include "core/utils/logger.h"
#include <iostream>

namespace link16 {

// 初始化Link16库
bool initialize() {
    LOG_INFO("初始化Link16库 v" LINK16_VERSION_STRING);
    
    // 初始化各个API
    bool messageResult = api::MessageAPI::getInstance().initialize();
    bool codingResult = api::CodingAPI::getInstance().initialize();
    bool physicalResult = api::PhysicalAPI::getInstance().initialize();
    bool simulationResult = api::SimulationAPI::getInstance().initialize();
    
    return messageResult && codingResult && physicalResult && simulationResult;
}

// 关闭Link16库
void shutdown() {
    LOG_INFO("关闭Link16库");
    
    // 关闭各个API
    api::MessageAPI::getInstance().shutdown();
    api::CodingAPI::getInstance().shutdown();
    api::PhysicalAPI::getInstance().shutdown();
    api::SimulationAPI::getInstance().shutdown();
}

// 获取版本信息
const char* getVersionString() {
    return LINK16_VERSION_STRING;
}

// 获取主版本号
int getVersionMajor() {
    return LINK16_VERSION_MAJOR;
}

// 获取次版本号
int getVersionMinor() {
    return LINK16_VERSION_MINOR;
}

// 获取补丁版本号
int getVersionPatch() {
    return LINK16_VERSION_PATCH;
}

} // namespace link16
