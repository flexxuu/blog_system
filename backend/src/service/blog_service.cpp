#include "blog_service.hpp"
#include <nlohmann/json.hpp>
#include "util/date_time_utils.hpp"
#include "util/logger.hpp"

using json = nlohmann::json;

BlogService::BlogService() : dataStore(DataStore::getInstance()) {}
BlogService::~BlogService() = default;

ApiResponse BlogService::getBlogsByCategory(const std::string& category, int page, int limit) {
    auto blogs = dataStore.getBlogsByCategory(category);
    
    int start = (page - 1) * limit;
    int end = std::min(start + limit, static_cast<int>(blogs.size()));
    
    json j;
    for (int i = start; i < end; ++i) {
        j.push_back(blogs[i].toJson());
    }
    
    BLogger::getInstance().info("Retrieved " + std::to_string(j.size()) + " blogs by category: " + category);
    
    return ApiResponse(200, "Success", {
        {"total", blogs.size()},
        {"page", page},
        {"limit", limit},
        {"data", j}
    });
}

ApiResponse BlogService::searchBlogs(const std::string& keyword, int page, int limit) {
    auto blogs = dataStore.searchBlogs(keyword);
    
    int start = (page - 1) * limit;
    int end = std::min(start + limit, static_cast<int>(blogs.size()));
    
    json j;
    for (int i = start; i < end; ++i) {
        j.push_back(blogs[i].toJson());
    }
    
    BLogger::getInstance().info("Searched " + std::to_string(j.size()) + " blogs with keyword: " + keyword);
    
    return ApiResponse(200, "Success", {
        {"total", blogs.size()},
        {"page", page},
        {"limit", limit},
        {"data", j}
    });
}

ApiResponse BlogService::getBlogById(int id) {
    auto blog = dataStore.getBlogById(id);
    
    if (blog.has_value()) {
        BLogger::getInstance().info("Retrieved blog with ID: " + std::to_string(id));
        return ApiResponse(200, "Success", blog->toJson());
    } else {
        BLogger::getInstance().warning("Blog not found with ID: " + std::to_string(id));
        return ApiResponse(404, "Blog not found");
    }
}

ApiResponse BlogService::createBlog(const std::string& jsonData) {
    try {
        auto j = json::parse(jsonData);
        
        BlogPost newBlog;
        newBlog.title = j["title"].get<std::string>();
        newBlog.content = j["content"].get<std::string>();
        newBlog.date = DateTimeUtils::getCurrentDate();
        newBlog.category = j["category"].get<std::string>();
        newBlog.author = j["author"].get<std::string>();
        newBlog.summary = j.contains("summary") 
            ? j["summary"].get<std::string>() 
            : newBlog.content.length() > 100 
                ? newBlog.content.substr(0, 100) + "..." 
                : newBlog.content;
        
        auto createdBlog = dataStore.addBlog(newBlog);
        BLogger::getInstance().info("Created new blog with ID: " + std::to_string(createdBlog.id));
        
        return ApiResponse(200, "Blog created successfully", createdBlog.toJson());
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error creating blog: " + std::string(e.what()));
        return ApiResponse(400, "Invalid request: " + std::string(e.what()));
    }
}

ApiResponse BlogService::updateBlog(int id, const std::string& jsonData) {
    try {
        auto j = json::parse(jsonData);
        
        auto existingBlog = dataStore.getBlogById(id);
        if (!existingBlog.has_value()) {
            BLogger::getInstance().warning("Blog not found for update with ID: " + std::to_string(id));
            return ApiResponse(404, "Blog not found");
        }
        
        BlogPost updatedBlog = existingBlog.value();
        updatedBlog.title = j.contains("title") ? j["title"].get<std::string>() : updatedBlog.title;
        updatedBlog.content = j.contains("content") ? j["content"].get<std::string>() : updatedBlog.content;
        updatedBlog.date = DateTimeUtils::getCurrentDate();
        updatedBlog.category = j.contains("category") ? j["category"].get<std::string>() : updatedBlog.category;
        updatedBlog.summary = j.contains("summary") 
            ? j["summary"].get<std::string>() 
            : updatedBlog.content.length() > 100 
                ? updatedBlog.content.substr(0, 100) + "..." 
                : updatedBlog.content;
        
        if (dataStore.updateBlog(updatedBlog)) {
            BLogger::getInstance().info("Updated blog with ID: " + std::to_string(id));
            return ApiResponse(200, "Blog updated successfully", updatedBlog.toJson());
        } else {
            BLogger::getInstance().error("Failed to update blog with ID: " + std::to_string(id));
            return ApiResponse(500, "Failed to update blog");
        }
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error updating blog: " + std::string(e.what()));
        return ApiResponse(400, "Invalid request: " + std::string(e.what()));
    }
}

ApiResponse BlogService::deleteBlog(int id) {
    if (dataStore.deleteBlog(id)) {
        BLogger::getInstance().info("Deleted blog with ID: " + std::to_string(id));
        return ApiResponse(200, "Blog deleted successfully");
    } else {
        BLogger::getInstance().warning("Blog not found for deletion with ID: " + std::to_string(id));
        return ApiResponse(404, "Blog not found");
    }
}