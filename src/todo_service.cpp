#include "todo_service.h"

#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include<algorithm>

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
    if(todo_text.empty()){
        return false;
    }

    std::ofstream output(data_file_path_, std::ios::app);
    if (!output.is_open()) {
        return false;
    }

    output << todo_text << '\n';
    return true;
}

bool TodoService::deleteTodo(const std::string& todo_text) const {
    // TODO(适合你来做)：
    // 这里非常适合你自己完成，因为它能帮你练到：
    // 1. 用 std::ifstream 读文件
    // 2. 用 std::ofstream 写文件
    // 3. 输入为空时如何校验
    // 4. 如何用 bool 表达成功或失败
    //
    // 一个简单版本的思路：
    // - 如果 todo_text 为空，返回 false
    // - 读出所有待办到一个 vector<string> 中
    // - 在 vector<string> 中查找 todo_text，如果找不到，返回 false
    // - 如果找到，从 vector<string> 中删除它
    // - 将更新后的 vector<string> 写回 data/todos.txt（覆盖原文件）
    // - 成功后返回 true
    if(todo_text.empty()){
        return false;
    }
    std::vector<std::string> copy_todos = loadTodos();
    auto it = std::find(copy_todos.begin(),copy_todos.end(),todo_text);
    if(it == copy_todos.end()){
        return false;
    }
    copy_todos.erase(it);
    std::ofstream output(data_file_path_);
    if (!output.is_open()) {
        return false;
    }
    for (const auto& todo : copy_todos) {
        output << todo << '\n';
    }
    return true;

}