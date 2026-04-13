#pragma once

#include <string>
#include <vector>

class TodoService {
public:
    explicit TodoService(std::string data_file_path);

    std::vector<std::string> loadTodos() const;
    bool addTodo(const std::string& todo_text) const;

private:
    std::string data_file_path_;
};
