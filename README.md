# Personal Productivity Server

这是一个适合 C++ 初学者持续迭代的实用项目。

它的目标不是一开始就做成复杂系统，而是先做成一个你自己能用、能部署到 Azure、也能写进简历的小型服务：

- 用 C++ 写 HTTP 服务器
- 用浏览器访问页面
- 逐步加入待办事项、笔记、文件上传等功能
- 用 Docker 打包运行环境

## 当前目录结构

```text
HTTP_Server/
├─ data/
│  └─ todos.txt
├─ src/
│  ├─ main.cpp
│  ├─ http_server.h
│  ├─ http_server.cpp
│  ├─ router.h
│  ├─ router.cpp
│  ├─ todo_service.h
│  └─ todo_service.cpp
├─ CMakeLists.txt
├─ Dockerfile
├─ .dockerignore
├─ README.md
└─ main.cpp
```

根目录下旧的 `main.cpp` 我先帮你保留了，它可以当作“单文件历史版本”参考。

## 每个文件负责什么

- `src/main.cpp`
  程序入口。负责创建服务对象和启动服务器。
- `src/http_server.h` / `src/http_server.cpp`
  封装 Linux socket 服务器，包括 `socket`、`bind`、`listen`、`accept`、`read`、`send`、`close`。
- `src/router.h` / `src/router.cpp`
  根据请求路径决定返回哪个页面或接口。
- `src/todo_service.h` / `src/todo_service.cpp`
  负责读写待办事项数据。

## 哪些是我先帮你做好的

- 工程目录结构
- 多文件拆分
- CMake 构建配置
- Docker 构建配置
- 首页和健康检查接口
- 待办读取逻辑

## 哪些最适合你自己做

代码里已经留了 `TODO(适合你来做)`，推荐你按这个顺序练：

1. 完成 `TodoService::addTodo`
2. 在 `Router::handleRequest` 中新增 `POST /todos`
3. 给首页加一个表单
4. 把待办真正写入 `data/todos.txt`

## 本地构建

```bash
cmake -S . -B build
cmake --build build
./build/personal_server
```

## Docker 构建

```bash
docker build -t personal-cpp-server .
docker run -d -p 8080:8080 --name personal-cpp-server personal-cpp-server
```
