#ifndef MEMO_SERVICE_HPP
#define MEMO_SERVICE_HPP

#include <string>
#include "model/memo.hpp"
#include "repository/data_store.hpp"
#include "util/api_response.hpp"

class MemoService {
private:
    DataStore& dataStore;

public:
    MemoService();
    ~MemoService();
    
    ApiResponse getAllMemos();
    ApiResponse getMemoById(int id);
    ApiResponse createMemo(const std::string& jsonData);
    ApiResponse updateMemo(int id, const std::string& jsonData);
    ApiResponse deleteMemo(int id);
};

#endif // MEMO_SERVICE_HPP