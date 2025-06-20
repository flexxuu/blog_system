#ifndef MEMO_HPP
#define MEMO_HPP

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Memo {
    int id;
    std::string title;
    std::string content;
    std::string date;
    std::string time;

    json toJson() const {
        return {
            {"id", id},
            {"title", title},
            {"content", content},
            {"date", date},
            {"time", time}
        };
    }

    static Memo fromJson(const json& j) {
        Memo memo;
        memo.id = j["id"].get<int>();
        memo.title = j["title"].get<std::string>();
        memo.content = j["content"].get<std::string>();
        memo.date = j["date"].get<std::string>();
        memo.time = j["time"].get<std::string>();
        return memo;
    }
};

#endif // MEMO_HPP