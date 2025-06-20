cmake_minimum_required(VERSION 3.10)
project(BlogSystem VERSION 7.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 添加可执行文件
add_executable(blog_system 
    src/main.cpp
    src/controller/api_controller.hpp
    src/model/blog_post.hpp
    src/model/blog_post.cpp
    src/model/memo.hpp
    src/model/memo.cpp
    src/repository/data_store.hpp
    src/repository/data_store.cpp
    src/service/blog_service.hpp
    src/service/blog_service.cpp
    src/service/memo_service.hpp
    src/service/memo_service.cpp
    src/service/backup_service.hpp
    src/service/backup_service.cpp
    src/util/logger.cpp
    src/util/date_time_utils.cpp
    src/util/api_response.hpp
    src/util/config.hpp
    src/util/config.cpp
    src/util/date_time_utils.hpp
    src/util/logger.hpp
)

add_executable(test_client
    src/test/main_test.cpp
    src/util/config.cpp
)

# 包含目录
target_include_directories(blog_system PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib-master)
target_include_directories(test_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib-master
)

# 查找并链接依赖库
find_package(nlohmann_json 3.2.0 REQUIRED)
# find_package(httplib REQUIRED)
find_package(Threads REQUIRED)

# 链接库
target_link_libraries(blog_system
    nlohmann_json::nlohmann_json
    # httplib::httplib
    Threads::Threads
)