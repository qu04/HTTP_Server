#include "http_server.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

HttpServer::HttpServer(int port, Router& router) : port_(port), router_(router) {}

void HttpServer::run() {
    // 第 1 步：socket
    // 创建服务器监听套接字。AF_INET 表示 IPv4，SOCK_STREAM 表示 TCP。
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        throw std::runtime_error("socket 创建失败");
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    // 第 2 步：bind
    // 把服务器套接字绑定到本机端口，客户端才能通过这个端口访问程序。
    if (bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        close(server_fd);
        throw std::runtime_error("bind 绑定失败");
    }

    // 第 3 步：listen
    // 把普通套接字切换到监听状态，等待客户端连接排队进入。
    if (listen(server_fd, 10) < 0) {
        close(server_fd);
        throw std::runtime_error("listen 监听失败");
    }

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        // 第 4 步：accept
        // 接收一个新的客户端连接，生成只服务当前客户端的 client_fd。
        int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if (client_fd < 0) {
            std::cerr << "accept 接收连接失败" << std::endl;
            continue;
        }

        char buffer[4096];
        std::memset(buffer, 0, sizeof(buffer));

        // 第 5 步：read
        // 读取客户端发来的 HTTP 请求文本。为了入门简单，这里先只读取一次。
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            std::cerr << "read 读取请求失败" << std::endl;
            close(client_fd);
            continue;
        }

        const std::string request_text(buffer, static_cast<std::size_t>(bytes_read));
        const std::string response_text = router_.handleRequest(request_text);

        // 第 6 步：send
        // 发送完整 HTTP 响应，注意响应头和响应体之间必须有 \r\n\r\n。
        ssize_t bytes_sent = send(client_fd, response_text.c_str(), response_text.size(), 0);
        if (bytes_sent < 0) {
            std::cerr << "send 发送响应失败" << std::endl;
        }

        // 第 7 步：close
        // 关闭当前客户端连接，但不关闭 server_fd，这样服务器还能继续运行。
        close(client_fd);
    }
}
