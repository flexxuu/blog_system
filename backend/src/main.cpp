#include <iostream>
#include "service/blog_service.hpp"
#include "service/memo_service.hpp"
#include "repository/data_store.hpp"
#include "util/api_response.hpp"
#include "util/logger.hpp"
#include "../cpp-httplib-master/httplib.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "util/config.hpp"
#include "controller/api_controller.hpp"

using namespace httplib;

// 注册信号处理，确保程序优雅退出
void setupSignalHandlers() {
    // 示例：处理SIGINT (Ctrl+C)
    signal(SIGINT, [](int signum) {
        BLogger::getInstance().info("Received SIGINT, shutting down...");
        DataStore::cleanup();
        BLogger::getInstance().cleanup();
        exit(0);
    });
}

int main() {
    try {
        // 初始化配置
        auto& config = Config::getInstance();
        config.reload();
        
        // 初始化日志系统
        BLogger::getInstance().init();
        BLogger::getInstance().info("Application started");
        
        // 设置信号处理
        setupSignalHandlers();

        // 创建服务实例
        auto& dataStore = DataStore::getInstance();
        BlogService blogService;
        MemoService memoService;

        // 创建HTTP服务器
        Server server;

        // 创建API控制器实例并设置路由
        ApiController apiController;
        apiController.setupRoutes(server);

        // 静态文件服务
        server.set_mount_point("/", config.get<std::string>("server.static_files_dir", "www"));
        
        // 设置CORS头
        server.set_default_headers(
            httplib::Headers{{"Access-Control-Allow-Origin", "*"}
            , {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"}
            , {"Access-Control-Allow-Headers", "Content-Type"}}
        );
        
        // 启动服务器
        std::string host = config.get<std::string>("server.host", "localhost");
        int port = config.get<int>("server.port", 8080);
        BLogger::getInstance().info("服务器启动中，访问 http://{}:{}", host, port);

        // 使用线程池处理请求
        auto taskQueueFactory = []() -> httplib::TaskQueue* {
            return new ThreadPool(8); // 创建8个工作线程的线程池
        };
        server.new_task_queue = taskQueueFactory;

        auto ret = server.listen(host.c_str(), port);
        
        if (ret) {
            BLogger::getInstance().info("服务器已停止");
        } else {
            BLogger::getInstance().critical("服务器启动失败，端口可能已被占用");
        }
        
        // 清理资源
        DataStore::cleanup();
        BLogger::getInstance().cleanup();
        
        return ret ? 0 : 1;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        if (BLogger::getInstance().hasError()) {
            BLogger::getInstance().critical("Fatal error: {}", e.what());
        }
        DataStore::cleanup();
        BLogger::getInstance().cleanup();
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        if (BLogger::getInstance().hasError()) {
            BLogger::getInstance().critical("Unknown fatal error occurred");
        }
        DataStore::cleanup();
        BLogger::getInstance().cleanup();
        return 1;
    }
}