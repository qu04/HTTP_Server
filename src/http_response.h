#pragma once

#include <string>

std::string buildHttpResponse(
    const std::string& body,
    const std::string& status = "200 OK",
    const std::string& content_type = "text/html; charset=utf-8");

std::string buildRedirectResponse(const std::string& location);
