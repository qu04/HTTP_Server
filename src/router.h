#pragma once

#include "todo_service.h"

#include <string>

std::string buildRedirectResponse(const std::string& location);

class Router {
public:
    explicit Router(TodoService& todo_service);
    std::string handleRequest(const std::string& request_text) const;

private:
    TodoService& todo_service_;

    std::string buildHomePage() const;
    std::string buildHttpResponse(
        const std::string& body,
        const std::string& status = "200 OK",
        const std::string& content_type = "text/html; charset=utf-8") const;
};
