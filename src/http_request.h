#pragma once

#include <string>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string body;
};

HttpRequest parseHttpRequest(const std::string& raw_request);
