#pragma once

#include "todo_service.h"
#include "http_response.h"

#include <string>

class Router {
public:
    explicit Router(TodoService& todo_service);
    HttpResponse handleRequest(const std::string& request_text) const;

private:
    TodoService& todo_service_;

    std::string buildHomePage() const;
};
