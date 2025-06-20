#include "memo_service.hpp"
#include <nlohmann/json.hpp>
#include "util/date_time_utils.hpp"
#include "util/logger.hpp"

using json = nlohmann::json;

MemoService::MemoService() : dataStore(DataStore::getInstance()) {}
MemoService::~MemoService() = default;

ApiResponse MemoService::getAllMemos() {
    auto memos = dataStore.getAllMemos();
    
    json j;
    for (const auto& memo : memos) {
        j.push_back(memo.toJson());
    }
    
    BLogger::getInstance().info("Retrieved " + std::to_string(memos.size()) + " memos");
    
    return ApiResponse(200, "Success", j);
}

ApiResponse MemoService::getMemoById(int id) {
    auto memo = dataStore.getMemoById(id);
    
    if (memo.has_value()) {
        BLogger::getInstance().info("Retrieved memo with ID: " + std::to_string(id));
        return ApiResponse(200, "Success", memo->toJson());
    } else {
        BLogger::getInstance().warning("Memo not found with ID: " + std::to_string(id));
        return ApiResponse(404, "Memo not found");
    }
}

ApiResponse MemoService::createMemo(const std::string& jsonData) {
    try {
        auto j = json::parse(jsonData);
        
        Memo newMemo;
        newMemo.title = j["title"].get<std::string>();
        newMemo.content = j["content"].get<std::string>();
        newMemo.date = DateTimeUtils::getCurrentDate();
        newMemo.time = DateTimeUtils::getCurrentTime();
        
        auto createdMemo = dataStore.addMemo(newMemo);
        BLogger::getInstance().info("Created new memo with ID: " + std::to_string(createdMemo.id));
        
        return ApiResponse(200, "Memo created successfully", createdMemo.toJson());
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error creating memo: " + std::string(e.what()));
        return ApiResponse(400, "Invalid request: " + std::string(e.what()));
    }
}

ApiResponse MemoService::updateMemo(int id, const std::string& jsonData) {
    try {
        auto j = json::parse(jsonData);
        
        auto existingMemo = dataStore.getMemoById(id);
        if (!existingMemo.has_value()) {
            BLogger::getInstance().warning("Memo not found for update with ID: " + std::to_string(id));
            return ApiResponse(404, "Memo not found");
        }
        
        Memo updatedMemo = existingMemo.value();
        updatedMemo.title = j.contains("title") ? j["title"].get<std::string>() : updatedMemo.title;
        updatedMemo.content = j.contains("content") ? j["content"].get<std::string>() : updatedMemo.content;
        updatedMemo.date = DateTimeUtils::getCurrentDate();
        updatedMemo.time = DateTimeUtils::getCurrentTime();
        
        if (dataStore.updateMemo(updatedMemo)) {
            BLogger::getInstance().info("Updated memo with ID: " + std::to_string(id));
            return ApiResponse(200, "Memo updated successfully", updatedMemo.toJson());
        } else {
            BLogger::getInstance().error("Failed to update memo with ID: " + std::to_string(id));
            return ApiResponse(500, "Failed to update memo");
        }
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error updating memo: " + std::string(e.what()));
        return ApiResponse(400, "Invalid request: " + std::string(e.what()));
    }
}

ApiResponse MemoService::deleteMemo(int id) {
    if (dataStore.deleteMemo(id)) {
        BLogger::getInstance().info("Deleted memo with ID: " + std::to_string(id));
        return ApiResponse(200, "Memo deleted successfully");
    } else {
        BLogger::getInstance().warning("Memo not found for deletion with ID: " + std::to_string(id));
        return ApiResponse(404, "Memo not found");
    }
}