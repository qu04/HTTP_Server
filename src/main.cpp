#include "http_server.h"
#include "router.h"
#include "todo_service.h"

#include <iostream>

int main() {
    TodoService todo_service("data/todos.txt");
    Router router(todo_service);
    HttpServer server(8080, router);

    std::cout << "Server is starting on port 8080..." << std::endl;
    std::cout << "Open http://localhost:8080/ in your browser." << std::endl;

    server.run();
    return 0;
}
