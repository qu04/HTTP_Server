#include "router.h"
#include "todo_service.h"

#include <sstream>
#include <string>
#include <vector>

static std::string decodeFormValue(const std::string& value);

Router::Router(TodoService& todo_service) : todo_service_(todo_service) {}

std::string Router::handleRequest(const std::string& request_text) const {
    if (request_text.rfind("GET /health", 0) == 0) {
        return buildHttpResponse("ok", "200 OK", "text/plain; charset=utf-8");
    }

    if (request_text.rfind("GET / ", 0) == 0 || request_text.rfind("GET /HTTP", 0) == 0) {
        return buildHttpResponse(buildHomePage());
    }

    if(request_text.rfind("POST /todos/delete") == 0){
            size_t body_pos = request_text.find("\r\n\r\n");
            if(body_pos != std::string::npos){
                std::string body = request_text.substr(body_pos + 4);
                if(body.rfind("todo=", 0) == 0){
                    std::string todo_text = decodeFormValue(body.substr(5));
                    if(todo_service_.deleteTodo(todo_text)){
                        return buildHttpResponse("删除成功", "200 OK", "text/plain; charset=utf-8");
                    }
                }
                return buildHttpResponse("删除失败", "400 Bad Request", "text/plain; charset=utf-8");
            }
            return buildHttpResponse("请求格式错误", "400 Bad Request", "text/plain; charset=utf-8");
        }

    if (request_text.rfind("POST /todos", 0) == 0) {
        size_t body_pos = request_text.find("\r\n\r\n");
        if (body_pos != std::string::npos) {
            std::string body = request_text.substr(body_pos + 4);
            if (body.rfind("todo=", 0) == 0) {
                std::string todo_text = decodeFormValue(body.substr(5));
                if (todo_service_.addTodo(todo_text)) {
                    return buildHttpResponse("添加成功", "200 OK", "text/plain; charset=utf-8");
                }
            }
            return buildHttpResponse("添加失败，请检查输入", "400 Bad Request", "text/plain; charset=utf-8");
        }
        return buildHttpResponse("请求格式错误", "400 Bad Request", "text/plain; charset=utf-8");
    }

    // TODO(删除待办提示)：
    // 你下一步可以照着上面的 POST /todos 再写一个 POST /todos/delete。
    // 建议顺序：
    // 1. 判断 request_text 是否以 "POST /todos/delete" 开头
    // 2. 用 find("\r\n\r\n") 找到请求体起点
    // 3. 用 substr(body_pos + 4) 取出请求体
    // 4. 先假设请求体格式是 todo=某条待办内容
    // 5. 去掉前面的 todo= 后，调用 todo_service_.deleteTodo(todo_text)
    // 6. 根据返回值返回“删除成功”或“删除失败”的页面
    // 第一版先把链路跑通，不用急着处理 URL 编码。
    

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
        // TODO(删除按钮提示)：
        // 这里很适合你自己练习把每条待办从“纯文本”升级成“文本 + 删除表单”。
        // 你可以把当前这一行 <li> 改成下面这个结构：
        // 1. 先输出待办文字 todo
        // 2. 再输出 <form method="POST" action="/todos/delete">
        // 3. 表单里放一个隐藏输入框：
        //    <input type="hidden" name="todo" value="当前这条待办">
        // 4. 最后输出一个“删除”按钮
        // 第一版先直接把 todo 放进 value 里即可，后面再处理特殊字符。
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
    html << "  <h2>下一步练习提示</h2>\n";
    html << "  <p>你可以自己实现 POST /todos/delete，把某条待办从 data/todos.txt 中删除。</p>\n";
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
