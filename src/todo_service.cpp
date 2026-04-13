#include "todo_service.h"

#include <fstream>
#include <utility>

TodoService::TodoService(std::string data_file_path) : data_file_path_(std::move(data_file_path)) {}

std::vector<std::string> TodoService::loadTodos() const {
    std::ifstream input(data_file_path_);
    std::vector<std::string> todos;
    std::string line;

    while (std::getline(input, line)) {
        if (!line.empty()) {
            todos.push_back(line);
        }
    }

    return todos;
}

bool TodoService::addTodo(const std::string& todo_text) const {
    // TODO(适合你来做)：
    // 这里非常适合你自己完成，因为它能帮你练到：
    // 1. 用 std::ofstream 追加写文件
    // 2. 输入为空时如何校验
    // 3. 如何用 bool 表达成功或失败
    //
    // 一个简单版本的思路：
    // - 如果 todo_text 为空，返回 false
    // - 用追加模式打开 data/todos.txt
    // - 写入 todo_text 和换行
    // - 成功后返回 true
    (void)todo_text;
    return false;
}
