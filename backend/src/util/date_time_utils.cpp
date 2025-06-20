#include "date_time_utils.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

std::string DateTimeUtils::getCurrentDate() {
    auto now = std::time(nullptr);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    std::ostringstream oss;
    oss << std::put_time(&tm_info, "%Y-%m-%d");
    return oss.str();
}

std::string DateTimeUtils::getCurrentTime() {
    auto now = std::time(nullptr);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    std::ostringstream oss;
    oss << std::put_time(&tm_info, "%H:%M:%S");
    return oss.str();
}