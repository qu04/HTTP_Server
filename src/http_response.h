#pragma once

#include <string>

struct HttpResponse
{
    std::string status;
    std::string raw;
};


HttpResponse buildHttpResponse(
    const std::string& body,
    const std::string& status = "200 OK",
    const std::string& content_type = "text/html; charset=utf-8");

HttpResponse buildRedirectResponse(const std::string& location);
