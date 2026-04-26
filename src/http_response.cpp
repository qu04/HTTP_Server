#include "http_response.h"

#include <sstream>

std::string buildHttpResponse(
    const std::string& body,
    const std::string& status,
    const std::string& content_type) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << "\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;
    return response.str();    
}

std::string buildRedirectResponse(const std::string& location) {
    std::ostringstream response;
    response << "HTTP/1.1 303 See Other\r\n";
    response << "Location: " << location << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    return response.str();
}
