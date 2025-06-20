#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <any>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config {
private:
    // 单例模式
    Config(const std::string& configPath = "/home/luc/myblog/config/config.json");
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    ~Config() = default;

    // 配置数据
    json configData;
    std::string configFilePath;

    // 加载配置文件
    void loadConfig();

public:
    // 获取单例实例
    static Config& getInstance(const std::string& configPath = "");

    // 获取配置值
    template<typename T>
    T get(const std::string& key, const T& defaultValue = T()) const {
        try {
            // 支持点号分隔的嵌套路径
            std::string currentKey;
            const json* current = &configData;
            
            size_t pos = 0;
            while ((pos = key.find('.', pos)) != std::string::npos) {
                currentKey = key.substr(0, pos);
                if (!current->contains(currentKey) || !(*current)[currentKey].is_object()) {
                    return defaultValue;
                }
                current = &(*current)[currentKey];
                pos++;
            }
            
            currentKey = key.substr(pos);
            if (!current->contains(currentKey)) {
                return defaultValue;
            }
            
            return (*current)[currentKey].get<T>();
        } catch (...) {
            return defaultValue;
        }
    }

    // 重新加载配置
    void reload();
};

#endif // CONFIG_HPP    

#if !defined(CONFIG_FILE_PATH_DEFINED)
#define CONFIG_FILE_PATH_DEFINED
// 修改为绝对路径
const std::string configFilePath = "/home/luc/myblog/config/config.json";
#endif