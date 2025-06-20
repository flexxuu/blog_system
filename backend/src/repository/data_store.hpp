#ifndef DATA_STORE_HPP
#define DATA_STORE_HPP

#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <shared_mutex>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "model/blog_post.hpp"
#include "model/memo.hpp"
#include "util/config.hpp"
#include "util/logger.hpp"

namespace fs = std::filesystem;

class DataStore {
private:
    static DataStore* instance;

    DataStore();
    DataStore(const DataStore&) = delete;
    DataStore& operator=(const DataStore&) = delete;
    ~DataStore() = default;

public:
    static DataStore& getInstance();
    static void cleanup();

private:
    std::vector<BlogPost> blogs;
    std::vector<Memo> memos;
    std::string blogsFilePath;
    std::string memosFilePath;
    mutable std::shared_mutex rwMutex; // 读写锁

    // 锁辅助方法
    template<typename Func>
    auto withWriteLock(Func&& func) {
        std::unique_lock<std::shared_mutex> lock(rwMutex);
        return func();
    }

    template<typename Func>
    auto withReadLock(Func&& func) const {
        std::shared_lock<std::shared_mutex> lock(rwMutex);
        return func();
    }

    void loadBlogs();
    void loadMemos();
    void createDirectories();
    
    // 无锁版本的保存方法
    void saveBlogsUnlocked();
    void saveMemosUnlocked();

public:
    // 博客操作
    std::vector<BlogPost> getAllBlogs() const;
    std::optional<BlogPost> getBlogById(int id) const;
    BlogPost addBlog(const BlogPost& blog);
    bool updateBlog(const BlogPost& blog);
    bool deleteBlog(int id);
    
    // 备忘录操作
    std::vector<Memo> getAllMemos() const;
    std::optional<Memo> getMemoById(int id) const;
    Memo addMemo(const Memo& memo);
    bool updateMemo(const Memo& memo);
    bool deleteMemo(int id);
    
    // 分类和搜索
    std::vector<BlogPost> getBlogsByCategory(const std::string& category) const;
    std::vector<BlogPost> searchBlogs(const std::string& keyword) const;
};

#endif // DATA_STORE_HPP