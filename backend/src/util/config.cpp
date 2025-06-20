#include "config.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

Config::Config(const std::string& configPath) : configFilePath(configPath) {
    loadConfig();
}

Config& Config::getInstance(const std::string& configPath) {
    static Config instance(configPath.empty() ? "/home/luc/myblog/config/config.json" : configPath);
    return instance;
}

void Config::loadConfig() {
    try {
        std::ifstream file(configFilePath);
        if (file.is_open()) {
            file >> configData;
            file.close();
        } else {
            std::cout << configFilePath << std::endl;
            std::cerr << "Failed to open config file: " << configFilePath << std::endl;
            configData = json::object();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
        configData = json::object();
    }
}

void Config::reload() {
    loadConfig();
}