#include "http_request.h"
#include <sstream>

HttpRequest parseHttpRequest(const std::string& raw_request) {
    HttpRequest request;

    std::size_t line_end = raw_request.find("\r\n");
    if(line_end != std::string::npos){
        std::string request_line = raw_request.substr(0,line_end);
        std::istringstream line_stream(request_line);
        line_stream >> request.method >>request.path;
    }

    std::size_t body_pos = raw_request.find("\r\n\r\n");
    
    if(body_pos != std::string::npos){
        request.body = raw_request.substr(body_pos + 4);
    }
    
    return request;
}
