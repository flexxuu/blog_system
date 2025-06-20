# 博客系统后端

这是一个使用C++开发的博客系统后端，采用现代C++技术栈构建，提供博客和备忘录管理功能。

## 项目结构
blog-system/
├── backend/                     # 后端代码
│   ├── CMakeLists.txt           # CMake配置文件
│   └── src/                     # 后端源代码
│       ├── main.cpp             # 主程序入口
│       ├── model/               # 数据模型
│       │   ├── blog_post.hpp
│       │   ├── blog_post.cpp
│       │   ├── memo.hpp
│       │   └── memo.cpp
│       ├── repository/          # 数据存储层
│       │   ├── data_store.hpp
│       │   └── data_store.cpp
│       ├── service/             # 业务逻辑层
│       │   ├── blog_service.hpp
│       │   ├── blog_service.cpp
│       │   ├── memo_service.hpp
│       │   └── memo_service.cpp
│       ├── util/                # 工具类
│       │   ├── api_response.hpp
│       │   ├── config.hpp
│       │   ├── config.cpp
│       │   └── date_time_utils.hpp
│       └── controller/          # API控制器
│           └── api_controller.hpp
├── config/                      # 配置文件
│   └── config.json              # 主配置文件
├── data/                        # 数据存储目录
│   └── .gitkeep                 # 空目录占位文件
├── logs/                        # 日志目录
│   └── .gitkeep                 # 空目录占位文件
├── www/                         # 静态文件目录
│   └── index.html               # 前端入口页面
├── scripts/                     # 脚本工具
│   ├── build.sh                 # 构建脚本
│   └── run.sh                   # 运行脚本
└── .gitignore                   # Git忽略配置
## 环境要求

- C++17兼容的编译器 (GCC 7+, Clang 5+)
- CMake 3.10+
- nlohmann/json 3.2.0+
- libhttplib

## 构建和运行

1. 克隆项目:
   ```bash
   git clone https://github.com/yourusername/blog-system.git
   cd blog-system
   ```

2. 构建项目:
   ```bash
   ./scripts/build.sh
   ```

3. 配置项目:
   ```bash
   cp config/config.example.json config/config.json
   # 编辑配置文件以适应您的环境
   ```

4. 运行项目:
   ```bash
   ./scripts/run.sh
   ```

5. 访问应用:
   打开浏览器访问 http://localhost:8080

## API文档

### 博客API

- `GET /api/blogs` - 获取博客列表
- `GET /api/blogs/search` - 搜索博客
- `GET /api/blogs/:id` - 获取单个博客
- `POST /api/blogs` - 创建博客
- `PUT /api/blogs/:id` - 更新博客
- `DELETE /api/blogs/:id` - 删除博客

### 备忘录API

- `GET /api/memos` - 获取所有备忘录
- `GET /api/memos/:id` - 获取单个备忘录
- `POST /api/memos` - 创建备忘录
- `PUT /api/memos/:id` - 更新备忘录
- `DELETE /api/memos/:id` - 删除备忘录

## 配置说明

配置文件位于`config/config.json`，包含以下主要配置项：
{
    "server": {
        "host": "localhost",
        "port": 8080,
        "thread_pool_size": 10,
        "timeout_seconds": 30,
        "static_files_dir": "../www"
    },
    "data": {
        "blogs_file": "data/blogs.json",
        "memos_file": "data/memos.json",
        "backup_interval_hours": 24,
        "backup_dir": "data/backups"
    },
    "logging": {
        "level": "info",
        "file": "logs/blog_system.log",
        "max_size_mb": 10,
        "max_files": 5
    },
    "security": {
        "jwt_secret": "your-secret-key",
        "jwt_expiration_hours": 24,
        "cors_allowed_origins": ["http://localhost:3000"]
    }
}
## 贡献

欢迎贡献代码、报告问题或提出建议！请先查看贡献指南。

## 许可证

本项目采用MIT许可证。    