#include "backup_service.hpp"
#include "util/config.hpp"
#include "util/logger.hpp"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

BackupService::BackupService() 
    : dataStore(DataStore::getInstance()), 
      running(false),
      backupIntervalHours(24) {
    auto& config = Config::getInstance();
    backupIntervalHours = config.get<int>("data.backup_interval_hours", 24);
    backupDir = config.get<std::string>("data.backup_dir", "data/backups");
    
    // 创建备份目录
    fs::create_directories(backupDir);
    BLogger::getInstance().info("Backup service initialized. Interval: " + 
                std::to_string(backupIntervalHours) + " hours, Directory: " + backupDir);
}

BackupService::~BackupService() {
    stopScheduledBackups();
}

void BackupService::startScheduledBackups() {
    if (!running) {
        running = true;
        backupThread = std::thread(&BackupService::backupTask, this);
        BLogger::getInstance().info("Scheduled backups started");
    }
}

void BackupService::stopScheduledBackups() {
    if (running) {
        running = false;
        if (backupThread.joinable()) {
            backupThread.join();
        }
        BLogger::getInstance().info("Scheduled backups stopped");
    }
}

void BackupService::backupTask() {
    while (running) {
        try {
            performBackup();
        } catch (const std::exception& e) {
            BLogger::getInstance().error("Error during backup: " + std::string(e.what()));
        }
        
        // 等待指定的时间间隔
        std::this_thread::sleep_for(std::chrono::hours(backupIntervalHours));
    }
}

void BackupService::performBackup() {
    try {
        // 创建时间戳目录
        auto now = std::time(nullptr);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", std::localtime(&now));
        std::string backupPath = backupDir + "/" + timestamp;
        fs::create_directories(backupPath);
        
        // 备份博客数据
        std::string blogsBackupPath = backupPath + "/blogs.json";
        std::ofstream blogsFile(blogsBackupPath);
        if (blogsFile.is_open()) {
            auto blogs = dataStore.getAllBlogs();
            json j;
            for (const auto& blog : blogs) {
                j.push_back(blog.toJson());
            }
            blogsFile << j.dump(2);
            blogsFile.close();
        }
        
        // 备份备忘录数据
        std::string memosBackupPath = backupPath + "/memos.json";
        std::ofstream memosFile(memosBackupPath);
        if (memosFile.is_open()) {
            auto memos = dataStore.getAllMemos();
            json j;
            for (const auto& memo : memos) {
                j.push_back(memo.toJson());
            }
            memosFile << j.dump(2);
            memosFile.close();
        }
        BLogger::getInstance().info("Backup completed successfully");
    }
    catch (const std::exception& e) {
        BLogger::getInstance().error("Error during backup: " + std::string(e.what()));
    }
        }