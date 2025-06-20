#ifndef DATE_TIME_UTILS_HPP
#define DATE_TIME_UTILS_HPP

#include <string>
#include <ctime>

class DateTimeUtils {
public:
    // 获取当前日期 (YYYY-MM-DD)
    static std::string getCurrentDate();
    
    // 获取当前时间 (HH:MM:SS)
    static std::string getCurrentTime();
};

#endif // DATE_TIME_UTILS_HPP    