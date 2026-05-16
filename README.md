# Personal Productivity Server

一个用 C++ 原生 socket 实现的轻量级 HTTP Web 服务。项目从最小的 `main.cpp` HTTP 服务器逐步演进为一个模块化的小型后端应用，支持待办事项管理、HTML 模板、CSS 样式、本地静态图片、Docker 部署和服务器一键重部署。

这个项目的重点不是做复杂 UI，而是练习 C++ 服务端工程能力：网络编程、HTTP 请求解析、路由分发、文件持久化、模块拆分和 Linux 部署。

## 功能特性

- 使用 Linux 原生 socket API 实现 HTTP 服务。
- 支持 `GET` / `POST` 请求解析。
- 支持待办事项新增和删除。
- 使用文本文件 `data/todos.txt` 做简单持久化。
- 支持 HTML 模板：`templates/index.html`。
- 支持 CSS 和本地图片等静态资源：`static/`。
- 支持表单 URL 解码，兼容中文和空格。
- 支持 Docker 镜像构建和 Azure Ubuntu 服务器部署。
- 支持 `redeploy.sh` 一键拉取最新代码并重启容器。

## 技术栈

- C++17
- Linux socket API
- HTTP/1.1 基础协议
- CMake
- Docker
- Ubuntu / Azure VM
- GitHub

## 项目结构

```text
HTTP_Server/
├─ data/
│  └─ todos.txt
├─ src/
│  ├─ main.cpp
│  ├─ http_server.h
│  ├─ http_server.cpp
│  ├─ http_request.h
│  ├─ http_request.cpp
│  ├─ http_response.h
│  ├─ http_response.cpp
│  ├─ router.h
│  ├─ router.cpp
│  ├─ todo_service.h
│  ├─ todo_service.cpp
│  ├─ static_file_handler.h
│  ├─ static_file_handler.cpp
│  ├─ file_utils.h
│  ├─ file_utils.cpp
│  ├─ string_utils.h
│  └─ string_utils.cpp
├─ static/
│  ├─ style.css
│  └─ images/
│     ├─ bg.jpg
│     └─ banner.jpg
├─ templates/
│  └─ index.html
├─ CMakeLists.txt
├─ Dockerfile
├─ redeploy.sh
└─ README.md
```

## 模块说明

- `http_server`
  负责底层 socket 流程，包括 `socket`、`bind`、`listen`、`accept`、`read`、`send`、`close`。
- `http_request`
  把原始 HTTP 请求文本解析成 `method`、`path`、`body`。
- `http_response`
  构造普通 HTTP 响应和重定向响应。
- `router`
  根据请求方法和路径分发到首页、待办接口、健康检查和静态文件处理。
- `todo_service`
  负责待办事项的读取、新增、删除。
- `static_file_handler`
  负责处理 `/static/...` 静态资源请求，并根据文件后缀返回正确的 `Content-Type`。
- `file_utils`
  提供通用文本文件读取函数。
- `string_utils`
  提供表单解码、字符串替换、表单字段提取等工具函数。

## 本地构建

Linux 环境下可以使用 CMake 构建：

```bash
cmake -S . -B build
cmake --build build
./build/personal_server
```

启动后访问：

```text
http://localhost:8080/
```

## Docker 构建与运行

```bash
docker build -t personal-cpp-server .
docker run -d -p 8080:8080 --name personal-cpp-server personal-cpp-server
```

查看容器日志：

```bash
docker logs personal-cpp-server
```

停止并删除容器：

```bash
docker stop personal-cpp-server
docker rm personal-cpp-server
```

## 服务器一键重部署

服务器上项目目录为 `~/HTTP_Server` 时，可以使用：

```bash
cd ~/HTTP_Server
./redeploy.sh
```

脚本会执行：

- 拉取 GitHub 最新代码。
- 重新构建 Docker 镜像。
- 停止并删除旧容器。
- 启动新容器并映射 `8080` 端口。

如果脚本没有执行权限：

```bash
chmod +x redeploy.sh
./redeploy.sh
```

## 常用测试地址

```text
http://服务器IP:8080/
http://服务器IP:8080/health
http://服务器IP:8080/static/style.css
http://服务器IP:8080/static/images/banner.jpg
http://服务器IP:8080/static/images/bg.jpg
```

## 当前已实现的请求

```text
GET  /                     返回首页
GET  /health               健康检查
GET  /static/...           返回静态资源
POST /todos                新增待办
POST /todos/delete         删除待办
```

## 简历描述参考

使用 C++17 和 Linux 原生 socket API 实现轻量级 HTTP Server，支持 HTTP 请求解析、路由分发、表单处理、URL 解码、静态资源服务、HTML 模板渲染和文件持久化，并通过 Docker 部署到 Azure Ubuntu 服务器。

## 后续计划

- 增加请求日志系统。
- 支持待办完成状态。
- 使用 JSON API 替代表单直出页面。
- 使用 SQLite 替代文本文件。
- 增加单元测试。
- 增加更完整的 HTTP header 解析。
