#include "router.h"
#include "todo_service.h"

#include <cctype>
#include <fstream>
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

static std::string readTextFile(const std::string& path) {
    std::ifstream input(path);
    if (!input.is_open()) {
        return "";
    }

    std::ostringstream content;
    content << input.rdbuf();
    return content.str();
}

static void replaceAll(std::string& text, const std::string& from, const std::string& to) {
    std::size_t pos = 0;
    while ((pos = text.find(from, pos)) != std::string::npos) {
        text.replace(pos, from.size(), to);
        pos += to.size();
    }
}

Router::Router(TodoService& todo_service) : todo_service_(todo_service) {}

std::string Router::handleRequest(const std::string& request_text) const {
    if (request_text.rfind("GET /health", 0) == 0) {
        return buildHttpResponse("ok", "200 OK", "text/plain; charset=utf-8");
    }

    if (request_text.rfind("GET /static/style.css", 0) == 0) {
        const std::string css = readTextFile("static/style.css");
        if (css.empty()) {
            return buildHttpResponse("CSS file not found", "404 Not Found", "text/plain; charset=utf-8");
        }
        return buildHttpResponse(css, "200 OK", "text/css; charset=utf-8");
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
                    return buildRedirectResponse("/");
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
                    return buildRedirectResponse("/");
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
    std::string html = readTextFile("templates/index.html");
    if (html.empty()) {
        return "<h1>Template file not found</h1>";
    }

    std::ostringstream todo_items;
    for (const std::string& todo : todos) {
        todo_items << "      <li>\n";
        todo_items << "        " << todo << "\n";
        todo_items << "        <form method=\"POST\" action=\"/todos/delete\">\n";
        todo_items << "          <input type=\"hidden\" name=\"todo\" value=\"" << todo << "\">\n";
        todo_items << "          <button type=\"submit\">删除</button>\n";
        todo_items << "        </form>\n";
        todo_items << "      </li>\n";
    }

    replaceAll(html, "{{TODOS}}", todo_items.str());
    return html;
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

std::string buildRedirectResponse(const std::string& location) {
    std::ostringstream response;
    response << "HTTP/1.1 303 See Other\r\n";
    response << "Location: " << location << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    return response.str();
}
