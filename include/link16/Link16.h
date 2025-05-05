#pragma once

// 包含所有公共API
#include "api/MessageAPI.h"
#include "api/CodingAPI.h"
#include "api/PhysicalAPI.h"
#include "api/SimulationAPI.h"

// 版本信息
#define LINK16_VERSION_MAJOR 1
#define LINK16_VERSION_MINOR 0
#define LINK16_VERSION_PATCH 0
#define LINK16_VERSION_STRING "1.0.0"

namespace link16 {

// 初始化Link16库
bool initialize();

// 关闭Link16库
void shutdown();

// 获取版本信息
const char* getVersionString();

// 获取主版本号
int getVersionMajor();

// 获取次版本号
int getVersionMinor();

// 获取补丁版本号
int getVersionPatch();

} // namespace link16
