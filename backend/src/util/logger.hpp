#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <ctime>
#include <iostream>
#include <unordered_map>
#include "config.hpp"

class BLogger {
private:
    std::mutex mutex;
    std::ofstream logFile;
    bool initialized = false;
    bool fileLoggingFailed = false;
    std::string logLevel = "info";
    
    enum LogLevel {
        TRACE = 0,
        DEBUG = 1,
        INFO = 2,
        WARNING = 3,
        ERROR = 4,
        CRITICAL = 5
    };
    
    struct LogConfig {
        bool consoleLogging;
        bool fileLogging;
        std::string logFilePath;
    };
    
    LogConfig logConfig;
    
    BLogger() = default;
    BLogger(const BLogger&) = delete;
    BLogger& operator=(const BLogger&) = delete;
    
    std::string getCurrentDateTime() {
        auto now = std::time(nullptr);
        char buf[26];
        struct tm tm_info;
        localtime_r(&now, &tm_info);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_info);
        return std::string(buf);
    }
    
    LogLevel getLogLevel(const std::string& level) {
        static const std::unordered_map<std::string, LogLevel> levelMap = {
            {"trace", TRACE},
            {"debug", DEBUG},
            {"info", INFO},
            {"warning", WARNING},
            {"error", ERROR},
            {"critical", CRITICAL}
        };
        
        auto it = levelMap.find(level);
        return it != levelMap.end() ? it->second : INFO;
    }
    
    bool shouldLog(const std::string& level) {
        return getLogLevel(level) >= getLogLevel(logLevel);
    }
    
    std::string formatMessage(const std::string& level, const std::string& message) {
        return "[" + getCurrentDateTime() + "] [" + level + "] " + message;
    }
    
    void write(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        
        if (logConfig.consoleLogging) {
            std::cout << message << std::endl;
        }
        
        if (logConfig.fileLogging && !fileLoggingFailed) {
            if (!initialized) {
                try {
                    logFile.open(logConfig.logFilePath, std::ios::app);
                    initialized = logFile.is_open();
                    
                    if (initialized) {
                        logFile << "----------------------------------------" << std::endl;
                        logFile << "BLogger initialized: " << getCurrentDateTime() << std::endl;
                    } else {
                        std::cerr << "Failed to open log file: " << logConfig.logFilePath << std::endl;
                        fileLoggingFailed = true;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error initializing logger: " << e.what() << std::endl;
                    fileLoggingFailed = true;
                }
            }
            
            if (initialized) {
                logFile << message << std::endl;
                logFile.flush();
            }
        }
    }
    
public:
    static BLogger& getInstance() {
        static BLogger instance;
        return instance;
    }
    
    void init() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            auto& config = Config::getInstance();
            logLevel = config.get<std::string>("logging.level", "info");
            
            logConfig.consoleLogging = config.get<bool>("logging.console_logging", true);
            logConfig.fileLogging = config.get<bool>("logging.file_logging", true);
            logConfig.logFilePath = "/home/luc/myblog/" + config.get<std::string>("logging.file", "logs/blog_system.log");
        }
        write(formatMessage("INFO", "BLogger initialized with level: " + logLevel));
    }
    
    void cleanup() {
        std::lock_guard<std::mutex> lock(mutex);
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    bool hasError() const {
        return fileLoggingFailed;
    }
    
    void trace(const std::string& message) {
        if (shouldLog("trace")) {
            write(formatMessage("TRACE", message));
        }
    }
    
    void debug(const std::string& message) {
        if (shouldLog("debug")) {
            write(formatMessage("DEBUG", message));
        }
    }
    
    void info(const std::string& message) {
        if (shouldLog("info")) {
            write(formatMessage("INFO", message));
        }
    }
    
    void warning(const std::string& message) {
        if (shouldLog("warning")) {
            write(formatMessage("WARNING", message));
        }
    }
    
    void error(const std::string& message) {
        if (shouldLog("error")) {
            write(formatMessage("ERROR", message));
        }
    }
    
    void critical(const std::string& message) {
        if (shouldLog("critical")) {
            write(formatMessage("CRITICAL", message));
        }
    }
    
    template<typename... Args>
    void trace(const std::string& format, Args&&... args) {
        if (shouldLog("trace")) {
            std::stringstream ss;
            ss << format;
            ((ss << std::forward<Args>(args)), ...);
            write(formatMessage("TRACE", ss.str()));
        }
    }
    
    template<typename... Args>
    void debug(const std::string& format, Args&&... args) {
        if (shouldLog("debug")) {
            std::stringstream ss;
            ss << format;
            ((ss << std::forward<Args>(args)), ...);
            write(formatMessage("DEBUG", ss.str()));
        }
    }
    
    template<typename... Args>
    void info(const std::string& format, Args&&... args) {
        if (shouldLog("info")) {
            std::stringstream ss;
            ss << format;
            ((ss << std::forward<Args>(args)), ...);
            write(formatMessage("INFO", ss.str()));
        }
    }
    
    template<typename... Args>
    void warning(const std::string& format, Args&&... args) {
        if (shouldLog("warning")) {
            std::stringstream ss;
            ss << format;
            ((ss << std::forward<Args>(args)), ...);
            write(formatMessage("WARNING", ss.str()));
        }
    }
    
    template<typename... Args>
    void error(const std::string& format, Args&&... args) {
        if (shouldLog("error")) {
            std::stringstream ss;
            ss << format;
            ((ss << std::forward<Args>(args)), ...);
            write(formatMessage("ERROR", ss.str()));
        }
    }
    
    template<typename... Args>
    void critical(const std::string& format, Args&&... args) {
        if (shouldLog("critical")) {
            std::stringstream ss;
            ss << format;
            ((ss << std::forward<Args>(args)), ...);
            write(formatMessage("CRITICAL", ss.str()));
        }
    }
};

#endif // LOGGER_HPP