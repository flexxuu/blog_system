#ifndef BLOG_SERVICE_HPP
#define BLOG_SERVICE_HPP

#include <string>
#include "model/blog_post.hpp"
#include "repository/data_store.hpp"
#include "util/api_response.hpp"

class BlogService {
private:
    DataStore& dataStore;

public:
    BlogService();
    ~BlogService();
    
    ApiResponse getBlogsByCategory(const std::string& category, int page, int limit);
    ApiResponse searchBlogs(const std::string& keyword, int page, int limit);
    ApiResponse getBlogById(int id);
    ApiResponse createBlog(const std::string& jsonData);
    ApiResponse updateBlog(int id, const std::string& jsonData);
    ApiResponse deleteBlog(int id);
};

#endif // BLOG_SERVICE_HPP