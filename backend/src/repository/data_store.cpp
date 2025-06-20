#include "data_store.hpp"
#include <fstream>
#include <stdexcept>
#include <set>

// 初始化静态成员
DataStore* DataStore::instance = nullptr;

DataStore& DataStore::getInstance() {
    static DataStore instance;
    return instance;
}

void DataStore::cleanup() {
    auto& instance = getInstance();
    instance.withWriteLock([&] {
        instance.blogs.clear();
        instance.memos.clear();
        BLogger::getInstance().info("DataStore resources cleaned up");
    });
}

DataStore::DataStore() {
    auto& config = Config::getInstance();
    blogsFilePath = config.get<std::string>("data.blogs_file", "../../data/blogs.json");
    memosFilePath = config.get<std::string>("data.memos_file", "../../data/memos.json");
    
    createDirectories();
    loadBlogs();
    loadMemos();
    
    BLogger::getInstance().info("DataStore initialized. Blogs: " + std::to_string(blogs.size()) + 
                              ", Memos: " + std::to_string(memos.size()));
}

void DataStore::createDirectories() {
    auto createDir = [](const std::string& path) {
        auto dir = fs::path(path).parent_path();
        if (!fs::exists(dir)) {
            fs::create_directories(dir);
        }
    };
    
    createDir(blogsFilePath);
    createDir(memosFilePath);
}

void DataStore::loadBlogs() {
    try {
        if (fs::exists(blogsFilePath)) {
            std::ifstream file(blogsFilePath);
            if (file.is_open()) {
                nlohmann::json j;
                file >> j;
                for (auto& item : j) {
                    blogs.push_back(BlogPost::fromJson(item));
                }
                BLogger::getInstance().info("Loaded " + std::to_string(blogs.size()) + " blogs from file");
            }
        }
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error loading blogs: " + std::string(e.what()));
    }
}

void DataStore::loadMemos() {
    try {
        if (fs::exists(memosFilePath)) {
            std::ifstream file(memosFilePath);
            if (file.is_open()) {
                nlohmann::json j;
                file >> j;
                for (auto& item : j) {
                    memos.push_back(Memo::fromJson(item));
                }
                BLogger::getInstance().info("Loaded " + std::to_string(memos.size()) + " memos from file");
            }
        }
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error loading memos: " + std::string(e.what()));
    }
}

void DataStore::saveBlogsUnlocked() {
    try {
        nlohmann::json j;
        for (const auto& blog : blogs) {
            j.push_back(blog.toJson());
        }
        std::ofstream file(blogsFilePath);
        //tttest
        if (file.is_open()) {
            file << j.dump(2);
            BLogger::getInstance().info("Saved " + std::to_string(blogs.size()) + " blogs to file");
        }
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error saving blogs: " + std::string(e.what()));
    }
}

void DataStore::saveMemosUnlocked() {
    try {
        nlohmann::json j;
        for (const auto& memo : memos) {
            j.push_back(memo.toJson());
        }
        std::ofstream file(memosFilePath);
        if (file.is_open()) {
            file << j.dump(2);
            BLogger::getInstance().info("Saved " + std::to_string(memos.size()) + " memos to file");
        }
    } catch (const std::exception& e) {
        BLogger::getInstance().error("Error saving memos: " + std::string(e.what()));
    }
}

// 博客操作实现
std::vector<BlogPost> DataStore::getAllBlogs() const {
    return withReadLock([this] {
        return blogs;
    });
}

std::optional<BlogPost> DataStore::getBlogById(int id) const {
    return withReadLock([this, id] {
        for (const auto& blog : blogs) {
            if (blog.id == id) {
                return std::optional<BlogPost>(blog);
            }
        }
        return std::optional<BlogPost>(std::nullopt);
    });
}

BlogPost DataStore::addBlog(const BlogPost& blog) {
    return withWriteLock([&] {
        std::set<int> usedIds;
        for (const auto& b : blogs) {
            usedIds.insert(b.id);
        }
        int nextId = 1;
        while (usedIds.find(nextId) != usedIds.end()) {
            nextId++;
        }
        BlogPost newBlog = blog;
        newBlog.id = nextId;
        blogs.push_back(newBlog);
        saveBlogsUnlocked();
        BLogger::getInstance().info("Added new blog with ID: " + std::to_string(nextId));
        return newBlog;
    });
}

bool DataStore::updateBlog(const BlogPost& blog) {
    return withWriteLock([&] {
        for (auto& b : blogs) {
            if (b.id == blog.id) {
                b = blog;
                saveBlogsUnlocked();
                BLogger::getInstance().info("Updated blog with ID: " + std::to_string(blog.id));
                return true;
            }
        }
        BLogger::getInstance().warning("Blog not found for update with ID: " + std::to_string(blog.id));
        return false;
    });
}

bool DataStore::deleteBlog(int id) {
    return withWriteLock([this, id] {
        auto it = std::remove_if(blogs.begin(), blogs.end(), [id](const BlogPost& b) {
            return b.id == id;
        });
        
        if (it != blogs.end()) {
            blogs.erase(it, blogs.end());
            saveBlogsUnlocked();
            BLogger::getInstance().info("Deleted blog with ID: " + std::to_string(id));
            return true;
        }
        BLogger::getInstance().warning("Blog not found for deletion with ID: " + std::to_string(id));
        return false;
    });
}

// 备忘录操作实现
std::vector<Memo> DataStore::getAllMemos() const {
    return withReadLock([this] {
        return memos;
    });
}

std::optional<Memo> DataStore::getMemoById(int id) const {
    return withReadLock([this, id] {
        for (const auto& memo : memos) {
            if (memo.id == id) {
                return std::optional<Memo>(memo);
            }
        }
        return std::optional<Memo>(std::nullopt);
    });
}

Memo DataStore::addMemo(const Memo& memo) {
    return withWriteLock([&] {
        std::set<int> usedIds;
        for (const auto& m : memos) {
            usedIds.insert(m.id);
        }
        int nextId = 1;
        while (usedIds.find(nextId) != usedIds.end()) {
            nextId++;
        }
        Memo newMemo = memo;
        newMemo.id = nextId;
        memos.push_back(newMemo);
        saveMemosUnlocked();
        BLogger::getInstance().info("Added new memo with ID: " + std::to_string(nextId));
        return newMemo;
    });
}

bool DataStore::updateMemo(const Memo& memo) {
    return withWriteLock([&] {
        for (auto& m : memos) {
            if (m.id == memo.id) {
                m = memo;
                saveMemosUnlocked();
                BLogger::getInstance().info("Updated memo with ID: " + std::to_string(memo.id));
                return true;
            }
        }
        BLogger::getInstance().warning("Memo not found for update with ID: " + std::to_string(memo.id));
        return false;
    });
}

bool DataStore::deleteMemo(int id) {
    return withWriteLock([this, id] {
        auto it = std::remove_if(memos.begin(), memos.end(), [id](const Memo& m) {
            return m.id == id;
        });
        
        if (it != memos.end()) {
            memos.erase(it, memos.end());
            saveMemosUnlocked();
            BLogger::getInstance().info("Deleted memo with ID: " + std::to_string(id));
            return true;
        }
        BLogger::getInstance().warning("Memo not found for deletion with ID: " + std::to_string(id));
        return false;
    });
}

// 分类和搜索实现
std::vector<BlogPost> DataStore::getBlogsByCategory(const std::string& category) const {
    return withReadLock([this, &category] {
        std::vector<BlogPost> result;
        for (const auto& blog : blogs) {
            if (category == "全部" || category == "" || blog.category == category) {
                result.push_back(blog);
            }
        }
        return result;
    });
}

std::vector<BlogPost> DataStore::searchBlogs(const std::string& keyword) const {
    return withReadLock([this, &keyword] {
        std::vector<BlogPost> result;
        for (const auto& blog : blogs) {
            if (blog.title.find(keyword) != std::string::npos ||
                blog.content.find(keyword) != std::string::npos ||
                blog.summary.find(keyword) != std::string::npos) {
                result.push_back(blog);
            }
        }
        return result;
    });
}