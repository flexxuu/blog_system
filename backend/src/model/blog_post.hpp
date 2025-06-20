#ifndef BLOG_POST_HPP
#define BLOG_POST_HPP

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct BlogPost {
    int id;
    std::string title;
    std::string content;
    std::string date;
    std::string category;
    std::string author;
    std::string summary;

    json toJson() const {
        return {
            {"id", id},
            {"title", title},
            {"content", content},
            {"date", date},
            {"category", category},
            {"author", author},
            {"summary", summary}
        };
    }

    static BlogPost fromJson(const json& j) {
        BlogPost post;
        post.id = j["id"].get<int>();
        post.title = j["title"].get<std::string>();
        post.content = j["content"].get<std::string>();
        post.date = j["date"].get<std::string>();
        post.category = j["category"].get<std::string>();
        post.author = j["author"].get<std::string>();
        post.summary = j["summary"].get<std::string>();
        return post;
    }
};

#endif // BLOG_POST_HPP