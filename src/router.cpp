#include "router.h"
#include "todo_service.h"
#include "file_utils.h"
#include "string_utils.h"

#include <sstream>
#include <string>
#include <vector>



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
