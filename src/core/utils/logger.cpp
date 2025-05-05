#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace link16 {
namespace utils {

// 获取单例实例
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// 构造函数
Logger::Logger() : currentLevel(LogLevel::INFO), initialized(false) {
}

// 初始化日志系统
bool Logger::initialize(const std::string& logFilePath, LogLevel minLevel) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (initialized) {
        // 如果已经初始化，先关闭之前的日志文件
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    logFile.open(logFilePath, std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "无法打开日志文件: " << logFilePath << std::endl;
        return false;
    }
    
    currentLevel = minLevel;
    initialized = true;
    
    // 记录初始化信息
    log(LogLevel::INFO, "日志系统初始化成功");
    return true;
}

// 关闭日志系统
void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(logMutex);
    
    if (initialized && logFile.is_open()) {
        log(LogLevel::INFO, "日志系统关闭");
        logFile.close();
        initialized = false;
    }
}

// 日志记录方法
void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) {
    log(LogLevel::FATAL, message);
}

// 设置日志级别
void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    currentLevel = level;
}

// 获取当前日志级别
LogLevel Logger::getLogLevel() const {
    return currentLevel;
}

// 日志记录实现
void Logger::log(LogLevel level, const std::string& message) {
    // 检查日志级别
    if (level < currentLevel) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex);
    
    // 如果未初始化，输出到控制台
    if (!initialized || !logFile.is_open()) {
        std::cout << getCurrentTimeString() << " [" << getLevelString(level) << "] " << message << std::endl;
        return;
    }
    
    // 写入日志文件
    logFile << getCurrentTimeString() << " [" << getLevelString(level) << "] " << message << std::endl;
    logFile.flush();
    
    // 错误和致命错误同时输出到控制台
    if (level >= LogLevel::ERROR) {
        std::cerr << getCurrentTimeString() << " [" << getLevelString(level) << "] " << message << std::endl;
    }
}

// 获取日志级别字符串
std::string Logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::FATAL:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

// 获取当前时间字符串
std::string Logger::getCurrentTimeString() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

} // namespace utils
} // namespace link16
