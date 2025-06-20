#ifndef BACKUP_SERVICE_HPP
#define BACKUP_SERVICE_HPP

#include <string>
#include <thread>
#include <atomic>
#include "repository/data_store.hpp"

class BackupService {
private:
    DataStore& dataStore;
    std::atomic<bool> running;
    std::thread backupThread;
    int backupIntervalHours;
    std::string backupDir;
    
    void backupTask();
    std::string getBackupFilename();
    
public:
    BackupService();
    ~BackupService();
    
    void startScheduledBackups();
    void stopScheduledBackups();
    void performBackup();
};

#endif // BACKUP_SERVICE_HPP