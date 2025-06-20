#include <iostream>
#include "../cpp-httplib-master/httplib.h"
#include "../util/config.hpp"

int main() {
    // std::ifstream configFile("/home/luc/myblog/config/test_config.json");
    // if (!configFile.is_open()) {
    //     std::cerr << "Failed to open test config file." << std::endl;
    //     return 1;
    // }
    // json config;
    // configFile >> config;
    Config& config = Config::getInstance("/home/luc/myblog/config/test_config.json");
    std::string host = config.get<std::string>("host", "localhost");
    int port = config.get<int>("port", 9876);
    std::cout << "测试客户端启动中，访问 http://" << host << ":" << port << std::endl;
    httplib::Client cli(host.c_str(), port);

    std::cout << "初始化完成，开始测试..." << std::endl;

    // 测试创建博客
    httplib::Headers headers = { {"Content-Type", "application/json"} };
    std::string body = "{\"title\": \"Test Blog\", \"content\": \"This is a test blog.\", \"category\": \"全部\", \"author\": \"xuf\"}";
    auto res = cli.Post("/api/blogs", body, "application/json");
    if (res && res->status == 200) {
        std::cout << "创建博客接口测试通过\n";
    } else {
        std::cout << "创建博客接口测试失败\n";
    }

    // 测试获取所有博客
    // 修改请求，添加必要参数
    res = cli.Get("/api/blogs?category=全部&page=1&limit=10");
    if (res && res->status == 200) {
        std::cout << "获取所有博客接口测试通过\n";
    } else {
        std::cout << "获取所有博客接口测试失败\n";
    }

    // 测试获取单个博客
    res = cli.Get("/api/blogs/1");
    // std::string path = "/api/blogs/1"; // 显式构建路径
    // res = cli.Get(path);
    if (res && res->status == 200) {
        std::cout << "获取单个博客接口测试通过\n";
        std::cout << "获取的单个博客内容：\n" << res->body << std::endl;
    } else {
        std::cout << "获取单个博客接口测试失败\n";
    }

    // 测试更新博客
    body = "{\"title\": \"Updated Test Blog\", \"content\": \"This is an updated test blog.\", \"category\": \"全部\"}";
    res = cli.Put("/api/blogs/1", body, "application/json");
    if (res && res->status == 200) {
        std::cout << "更新博客接口测试通过\n";
        std::cout << "更新的单个博客内容：\n" << res->body << std::endl;
    } else {
        std::cout << "更新博客接口测试失败\n";
    }

    // 测试删除博客
    res = cli.Delete("/api/blogs/1");
    if (res && res->status == 200) {
        std::cout << "删除博客接口测试通过\n";
    } else {
        std::cout << "删除博客接口测试失败\n";
    }

    // 测试创建备忘录
    // 修改第81行，避免变量重复声明，使用新变量名
    std::string memoBody = "{\"title\": \"Test Memo\", \"content\": \"This is a test memo.\", \"author\": \"xuf\"}";
    res = cli.Post("/api/memos", memoBody, "application/json");
    if (res && res->status == 200) {
        std::cout << "创建备忘录接口测试通过\n";
    } else {
        std::cout << "创建备忘录接口测试失败\n";
    }

    // 测试获取所有备忘录
    res = cli.Get("/api/memos");
    if (res && res->status == 200) {
        std::cout << "获取所有备忘录接口测试通过\n";
    } else {
        std::cout << "获取所有备忘录接口测试失败\n";
    }

    // 测试获取单个备忘录
    res = cli.Get("/api/memos/1");
    if (res && res->status == 200) {
        std::cout << "获取单个备忘录接口测试通过\n";
    } else {
        std::cout << "获取单个备忘录接口测试失败\n";
    }

    // 测试更新备忘录
    body = "{\"title\": \"Updated Test Memo\", \"content\": \"This is an updated test memo.\"}";
    res = cli.Put("/api/memos/1", body, "application/json");
    if (res && res->status == 200) {
        std::cout << "更新备忘录接口测试通过\n";
    } else {
        std::cout << "更新备忘录接口测试失败\n";
    }

    // 测试删除备忘录
    res = cli.Delete("/api/memos/1");
    if (res && res->status == 200) {
        std::cout << "删除备忘录接口测试通过\n";
    } else {
        std::cout << "删除备忘录接口测试失败\n";
    }

    return 0;
}