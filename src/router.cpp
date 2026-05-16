#include "file_utils.h"
#include "http_request.h"
#include "http_response.h"
#include "router.h"
#include "static_file_handler.h"
#include "string_utils.h"
#include "todo_service.h"
#include "logger.h"

#include <sstream>
#include <string>
#include <vector>

Router::Router(TodoService& todo_service) : todo_service_(todo_service) {}

HttpResponse Router::handleRequest(const std::string& request_text) const {
    const HttpRequest request = parseHttpRequest(request_text);
    
    

    if (request.method == "GET" && request.path == "/health") {
        HttpResponse response = buildHttpResponse("ok", "200 OK", "text/plain; charset=utf-8");
        logRequest(request,response.status);
        return response;
    }

    if (isStaticFileRequest(request)) {
        HttpResponse response = handleStaticFileRequest(request);
        logRequest(request,response.status);
        return response;
    }

    if (request.method == "GET" && request.path == "/") {
        HttpResponse response = buildHttpResponse(buildHomePage());
        logRequest(request,response.status);
        return response;
    }

    if (request.method == "POST" && request.path == "/todos/delete") {
        const std::string todo_text = extractFormValue(request.body, "todo");
        if (!todo_text.empty() && todo_service_.deleteTodo(todo_text)) {
            HttpResponse response = buildRedirectResponse("/");
            logRequest(request,response.status);
            return response;
        }
        HttpResponse response = buildHttpResponse("删除失败", "400 Bad Request", "text/plain; charset=utf-8");
        logRequest(request,response.status);
        return response;
    }

    if (request.method == "POST" && request.path == "/todos") {
        const std::string todo_text = extractFormValue(request.body, "todo");
        if (!todo_text.empty() && todo_service_.addTodo(todo_text)) {
            HttpResponse response = buildRedirectResponse("/");
            logRequest(request,response.status);
            return response;
        }
        HttpResponse response = buildHttpResponse("添加失败，请检查输入", "400 Bad Request", "text/plain; charset=utf-8");
        logRequest(request,response.status);
        return response;
    }

    const std::string not_found_html =
        "<html><body>"
        "<h1 style='color: red;'>404 - Not Found</h1>"
        "<p>你访问的页面不存在。</p>"
        "<a href='/'>点击这里返回首页</a>"
        "</body></html>";
    HttpResponse response = buildHttpResponse(not_found_html, "404 Not Found");
    logRequest(request,response.status);
    return response;
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
