#ifndef API_RESPONSE_HPP
#define API_RESPONSE_HPP

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// API统一响应格式
struct ApiResponse {
    int code;
    std::string message;
    json data;
    
    ApiResponse() : code(200), message("Success"), data(json()) {}
    ApiResponse(int code, const std::string& message, const json& data = json())
        : code(code), message(message), data(data) {}
    
    // 添加移动构造函数
    // 添加拷贝赋值运算符
    ApiResponse& operator=(const ApiResponse& other) {
        if (this != &other) {
            code = other.code;
            message = other.message;
            data = other.data;
        }
        return *this;
    }
    ApiResponse(ApiResponse&& other) noexcept
        : code(other.code), message(std::move(other.message)), data(std::move(other.data)) {
        other.code = 0;
        other.message.clear();
        other.data = json();
    }

    json toJson() const {
        json j = {
            {"code", code},
            {"message", message}
        };
        if (!data.is_null()) {
            j["data"] = data;
        }
        return j;
    }
};

#endif // API_RESPONSE_HPP