#pragma once
#include <string>
#include <fstream>
#include <mutex>

namespace link16 {
namespace utils {

// 日志级别
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
public:
    // 获取单例实例
    static Logger& getInstance();
    
    // 初始化日志系统
    bool initialize(const std::string& logFilePath, LogLevel minLevel = LogLevel::INFO);
    
    // 关闭日志系统
    void shutdown();
    
    // 日志记录方法
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
    
    // 设置日志级别
    void setLogLevel(LogLevel level);
    
    // 获取当前日志级别
    LogLevel getLogLevel() const;

private:
    // 私有构造函数(单例模式)
    Logger();
    
    // 禁止拷贝和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // 日志记录实现
    void log(LogLevel level, const std::string& message);
    
    // 获取日志级别字符串
    std::string getLevelString(LogLevel level) const;
    
    // 获取当前时间字符串
    std::string getCurrentTimeString() const;
    
    // 日志文件
    std::ofstream logFile;
    
    // 日志级别
    LogLevel currentLevel;
    
    // 互斥锁，保证线程安全
    std::mutex logMutex;
    
    // 是否已初始化
    bool initialized;
};

// 便捷宏定义
#define LOG_DEBUG(message) link16::utils::Logger::getInstance().debug(message)
#define LOG_INFO(message) link16::utils::Logger::getInstance().info(message)
#define LOG_WARNING(message) link16::utils::Logger::getInstance().warning(message)
#define LOG_ERROR(message) link16::utils::Logger::getInstance().error(message)
#define LOG_FATAL(message) link16::utils::Logger::getInstance().fatal(message)

} // namespace utils
} // namespace link16
