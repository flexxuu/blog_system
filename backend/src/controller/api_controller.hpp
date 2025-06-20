#ifndef API_CONTROLLER_HPP
#define API_CONTROLLER_HPP

#include <httplib.h>
#include "service/blog_service.hpp"
#include "service/memo_service.hpp"

using namespace httplib;

class ApiController {
private:
    BlogService blogService;
    MemoService memoService;
    
public:
    void setupRoutes(Server& server) {
            // ============== 博客API ==============
        
        // 获取所有博客
        server.Get("/api/blogs", [&](const Request& req, Response& res) {
            try {
                std::string category = req.get_param_value("category");
                std::string search = req.get_param_value("search");
                
                // 安全解析分页参数
                int page = 1;
                int limit = 10;
                
                if (!req.get_param_value("page").empty()) {
                    page = std::stoi(req.get_param_value("page"));
                    if (page <= 0) page = 1;
                }
                
                if (!req.get_param_value("limit").empty()) {
                    limit = std::stoi(req.get_param_value("limit"));
                    if (limit <= 0 || limit > 100) limit = 10;
                }
                
                BLogger::getInstance().info("Fetching blogs - category: {}, search: {}, page: {}, limit: {}", 
                                        category, search, page, limit);
                
                ApiResponse response;
                if (!search.empty()) {
                    response = blogService.searchBlogs(search, page, limit);
                } else {
                    response = blogService.getBlogsByCategory(category, page, limit);
                }
                
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error processing /api/blogs: {}", e.what());
                res.status = 500;
                res.set_content(ApiResponse(500, "Internal server error").toJson().dump(), "application/json");
            }
        });
        
        // 获取单个博客
        server.Get(R"(/api/blogs/(\d+))", [&](const Request& req, Response& res) {
            try {
                std::string idStr = req.matches[1];
                if (idStr.empty()) {
                    BLogger::getInstance().warning("Missing ID parameter");
                    res.status = 400;
                    res.set_content(ApiResponse(400, "Missing ID parameter").toJson().dump(), "application/json");
                    return;
                }
                
                int id = std::stoi(idStr);
                BLogger::getInstance().info("Fetching blog with ID: {}", id);
                
                auto response = blogService.getBlogById(id);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::invalid_argument& e) {
                BLogger::getInstance().error("Invalid ID format: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid ID format").toJson().dump(), "application/json");
            } catch (const std::out_of_range& e) {
                BLogger::getInstance().error("ID out of range: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "ID out of range").toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Internal error: {}", e.what());
                res.status = 500;
                res.set_content(ApiResponse(500, "Internal server error").toJson().dump(), "application/json");
            } catch (...) {
                BLogger::getInstance().critical("Unknown error occurred");
                res.status = 500;
                res.set_content(ApiResponse(500, "Internal server error").toJson().dump(), "application/json");
            }
        });
        
        // 创建博客
        server.Post("/api/blogs", [&](const Request& req, Response& res) {
            try {
                BLogger::getInstance().info("Creating new blog - Request body: {}", req.body);
                
                // 验证JSON格式
                try {
                    auto jsonData = nlohmann::json::parse(req.body);
                    (void)jsonData; // 避免编译器警告变量未使用
                } catch (const nlohmann::json::parse_error& e) {
                    BLogger::getInstance().warning("Invalid JSON format: {}", e.what());
                    res.status = 400;
                    res.set_content(ApiResponse(400, "Invalid JSON format").toJson().dump(), "application/json");
                    return;
                }
                
                auto response = blogService.createBlog(req.body);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error creating blog: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid request").toJson().dump(), "application/json");
            }
        });
        
        // 更新博客
        server.Put(R"(/api/blogs/(\d+))", [&](const Request& req, Response& res) {
            try {
                std::string idStr = req.matches[1];
                int id = std::stoi(idStr);
                BLogger::getInstance().info("Updating blog with ID: {}", id);
                
                // 验证JSON格式
                try {
                    auto jsonData = nlohmann::json::parse(req.body);
                    (void)jsonData; // 避免编译器警告变量未使用
                } catch (const nlohmann::json::parse_error& e) {
                    BLogger::getInstance().warning("Invalid JSON format: {}", e.what());
                    res.status = 400;
                    res.set_content(ApiResponse(400, "Invalid JSON format").toJson().dump(), "application/json");
                    return;
                }
                
                auto response = blogService.updateBlog(id, req.body);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error updating blog: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid request").toJson().dump(), "application/json");
            }
        });
        
        // 删除博客
        server.Delete(R"(/api/blogs/(\d+))", [&](const Request& req, Response& res) {
            try {
                std::string idStr = req.matches[1];
                int id = std::stoi(idStr);
                BLogger::getInstance().info("Deleting blog with ID: {}", id);
                
                auto response = blogService.deleteBlog(id);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error deleting blog: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid ID").toJson().dump(), "application/json");
            }
        });
        
        // ============== 备忘录API ==============
        
        // 获取所有备忘录
        server.Get("/api/memos", [&](const Request& req, Response& res) {
            try {
                BLogger::getInstance().info("Fetching all memos");
                auto response = memoService.getAllMemos();
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error fetching memos: {}", e.what());
                res.status = 500;
                res.set_content(ApiResponse(500, "Internal server error").toJson().dump(), "application/json");
            }
        });
        
        // 获取单个备忘录
        server.Get(R"(/api/memos/(\d+))", [&](const Request& req, Response& res) {
            try {
                std::string idStr = req.matches[1];
                int id = std::stoi(idStr);
                BLogger::getInstance().info("Fetching memo with ID: {}", id);
                
                auto response = memoService.getMemoById(id);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error fetching memo: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid ID").toJson().dump(), "application/json");
            }
        });
        
        // 创建备忘录
        server.Post("/api/memos", [&](const Request& req, Response& res) {
            try {
                BLogger::getInstance().info("Creating new memo - Request body: {}", req.body);
                
                // 验证JSON格式
                try {
                    auto jsonData = nlohmann::json::parse(req.body);
                    (void)jsonData; // 避免编译器警告变量未使用
                } catch (const nlohmann::json::parse_error& e) {
                    BLogger::getInstance().warning("Invalid JSON format: {}", e.what());
                    res.status = 400;
                    res.set_content(ApiResponse(400, "Invalid JSON format").toJson().dump(), "application/json");
                    return;
                }
                
                auto response = memoService.createMemo(req.body);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error creating memo: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid request").toJson().dump(), "application/json");
            }
        });
        
        // 更新备忘录
        server.Put(R"(/api/memos/(\d+))", [&](const Request& req, Response& res) {
            try {
                std::string idStr = req.matches[1];
                int id = std::stoi(idStr);
                BLogger::getInstance().info("Updating memo with ID: {}", id);
                
                // 验证JSON格式
                try {
                    auto jsonData = nlohmann::json::parse(req.body);
                    (void)jsonData; // 避免编译器警告变量未使用
                } catch (const nlohmann::json::parse_error& e) {
                    BLogger::getInstance().warning("Invalid JSON format: {}", e.what());
                    res.status = 400;
                    res.set_content(ApiResponse(400, "Invalid JSON format").toJson().dump(), "application/json");
                    return;
                }
                
                auto response = memoService.updateMemo(id, req.body);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error updating memo: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid request").toJson().dump(), "application/json");
            }
        });
        
        // 删除备忘录
        server.Delete(R"(/api/memos/(\d+))", [&](const Request& req, Response& res) {
            try {
                std::string idStr = req.matches[1];
                int id = std::stoi(idStr);
                BLogger::getInstance().info("Deleting memo with ID: {}", id);
                
                auto response = memoService.deleteMemo(id);
                res.status = response.code;
                res.set_content(response.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                BLogger::getInstance().error("Error deleting memo: {}", e.what());
                res.status = 400;
                res.set_content(ApiResponse(400, "Invalid ID").toJson().dump(), "application/json");
            }
        });
    }
};

#endif // API_CONTROLLER_HPP