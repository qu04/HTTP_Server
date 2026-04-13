#include "router.h"

#include <sstream>
#include <string>
#include <vector>

Router::Router(TodoService& todo_service) : todo_service_(todo_service) {}

std::string Router::handleRequest(const std::string& request_text) const {
    if (request_text.rfind("GET /health", 0) == 0) {
        return buildHttpResponse("ok", "200 OK", "text/plain; charset=utf-8");
    }

    if (request_text.rfind("GET / ", 0) == 0 || request_text.rfind("GET /HTTP", 0) == 0) {
        return buildHttpResponse(buildHomePage());
    }

    // TODO(适合你来做)：
    // 这里很适合你自己练“路由分发”。
    // 下一步你可以在这里增加：
    // 1. POST /todos
    // 2. GET /api/todos
    // 3. 更友好的 404 页面
    return buildHttpResponse("<h1>404 Not Found</h1>", "404 Not Found");
}

std::string Router::buildHomePage() const {
    const std::vector<std::string> todos = todo_service_.loadTodos();

    std::ostringstream html;
    html << "<!DOCTYPE html>\n";
    html << "<html lang=\"zh-CN\">\n";
    html << "<head>\n";
    html << "  <meta charset=\"UTF-8\">\n";
    html << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    html << "  <title>Personal Productivity Server</title>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "  <h1>Hello from Azure C++ Server!</h1>\n";
    html << "  <p>这是你的第一个可持续扩展的 C++ 实用项目。</p>\n";
    html << "  <h2>当前待办</h2>\n";
    html << "  <ul>\n";

    for (const std::string& todo : todos) {
        html << "    <li>" << todo << "</li>\n";
    }

    html << "  </ul>\n";
    html << "  <h2>下一步练习提示</h2>\n";
    html << "  <p>你可以自己实现 POST /todos，把新待办保存到 data/todos.txt。</p>\n";
    html << "</body>\n";
    html << "</html>\n";

    return html.str();
}

std::string Router::buildHttpResponse(
    const std::string& body,
    const std::string& status,
    const std::string& content_type) const {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << "\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;
    return response.str();
}
