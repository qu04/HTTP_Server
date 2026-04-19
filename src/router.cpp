#include "router.h"
#include "todo_service.h"

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

static int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }

    return -1;
}

static std::string decodeFormValue(const std::string& value) {
    std::string decoded;
    decoded.reserve(value.size());

    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '+') {
            decoded.push_back(' ');
            continue;
        }

        if (value[i] == '%' && i + 2 < value.size()) {
            const int high = hexCharToInt(value[i + 1]);
            const int low = hexCharToInt(value[i + 2]);

            if (high >= 0 && low >= 0) {
                decoded.push_back(static_cast<char>((high << 4) | low));
                i += 2;
                continue;
            }
        }

        decoded.push_back(value[i]);
    }

    return decoded;
}

Router::Router(TodoService& todo_service) : todo_service_(todo_service) {}

std::string Router::handleRequest(const std::string& request_text) const {
    if (request_text.rfind("GET /health", 0) == 0) {
        return buildHttpResponse("ok", "200 OK", "text/plain; charset=utf-8");
    }

    if (request_text.rfind("GET / ", 0) == 0 || request_text.rfind("GET /HTTP", 0) == 0) {
        return buildHttpResponse(buildHomePage());
    }

    if (request_text.rfind("POST /todos/delete", 0) == 0) {
        const std::size_t body_pos = request_text.find("\r\n\r\n");
        if (body_pos != std::string::npos) {
            const std::string body = request_text.substr(body_pos + 4);
            if (body.rfind("todo=", 0) == 0) {
                const std::string todo_text = decodeFormValue(body.substr(5));
                if (todo_service_.deleteTodo(todo_text)) {
                    return buildHttpResponse("删除成功", "200 OK", "text/plain; charset=utf-8");
                }
            }

            return buildHttpResponse("删除失败", "400 Bad Request", "text/plain; charset=utf-8");
        }

        return buildHttpResponse("请求格式错误", "400 Bad Request", "text/plain; charset=utf-8");
    }

    if (request_text.rfind("POST /todos", 0) == 0) {
        const std::size_t body_pos = request_text.find("\r\n\r\n");
        if (body_pos != std::string::npos) {
            const std::string body = request_text.substr(body_pos + 4);
            if (body.rfind("todo=", 0) == 0) {
                const std::string todo_text = decodeFormValue(body.substr(5));
                if (todo_service_.addTodo(todo_text)) {
                    return buildHttpResponse("添加成功", "200 OK", "text/plain; charset=utf-8");
                }
            }

            return buildHttpResponse("添加失败，请检查输入", "400 Bad Request", "text/plain; charset=utf-8");
        }

        return buildHttpResponse("请求格式错误", "400 Bad Request", "text/plain; charset=utf-8");
    }

    std::string not_found_html =
        "<html><body>"
        "<h1 style='color: red;'>404 - Not Found</h1>"
        "<p>你访问的页面不存在。</p>"
        "<a href='/'>点击这里返回首页</a>"
        "</body></html>";

    return buildHttpResponse(not_found_html, "404 Not Found");
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
        html << "    <li>\n";
        html << "      " << todo << "\n";
        html << "      <form method=\"POST\" action=\"/todos/delete\">\n";
        html << "        <input type=\"hidden\" name=\"todo\" value=\"" << todo << "\">\n";
        html << "        <button type=\"submit\">删除</button>\n";
        html << "      </form>\n";
        html << "    </li>\n";
    }

    html << "  </ul>\n";
    html << "  <h2>添加待办</h2>\n";
    html << "  <form method=\"POST\" action=\"/todos\">\n";
    html << "    <input type=\"text\" name=\"todo\" placeholder=\"输入新的待办\">\n";
    html << "    <button type=\"submit\">添加</button>\n";
    html << "  </form>\n";
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
