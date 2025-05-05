#pragma once

// 平台检测宏
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
    #define PLATFORM_MACOS
#else
    #define PLATFORM_UNKNOWN
#endif

// 路径分隔符
#ifdef PLATFORM_WINDOWS
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

namespace link16 {
namespace utils {

// 平台特定的函数和工具
class Platform {
public:
    // 获取平台名称
    static const char* getPlatformName() {
        #ifdef PLATFORM_WINDOWS
            return "Windows";
        #elif defined(PLATFORM_LINUX)
            return "Linux";
        #elif defined(PLATFORM_MACOS)
            return "macOS";
        #else
            return "Unknown";
        #endif
    }
    
    // 获取路径分隔符
    static const char* getPathSeparator() {
        return PATH_SEPARATOR;
    }
    
    // 组合路径
    static std::string combinePath(const std::string& path1, const std::string& path2) {
        if (path1.empty()) {
            return path2;
        }
        
        if (path2.empty()) {
            return path1;
        }
        
        std::string result = path1;
        if (result.back() != PATH_SEPARATOR[0] && path2.front() != PATH_SEPARATOR[0]) {
            result += PATH_SEPARATOR;
        } else if (result.back() == PATH_SEPARATOR[0] && path2.front() == PATH_SEPARATOR[0]) {
            result.pop_back();
        }
        
        result += path2;
        return result;
    }
};

} // namespace utils
} // namespace link16
