cmake_minimum_required(VERSION 3.16)
project(PersonalProductivityServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(personal_server
    src/main.cpp
    src/http_server.cpp
    src/router.cpp
    src/todo_service.cpp
    src/string_utils.cpp
    src/file_utils.cpp
    src/http_request.cpp
    src/http_response.cpp
)


target_include_directories(personal_server PRIVATE src)
