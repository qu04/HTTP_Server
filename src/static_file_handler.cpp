#include "static_file_handler.h"
#include "http_response.h"

#include <fstream>
#include <sstream>

static std::string readBinaryFile(const std::string& path){
    std::ifstream input(path,std::ios::binary);
    if(!input.is_open()){
        return "";
    }

    std::ostringstream content;
    content << input.rdbuf();
    return content.str();

}

static bool endsWith(const std::string& text, const std::string& suffix){
    if(text.size() < suffix.size()){
        return false;
    }
    return text.substr(text.size() - suffix.size()) == suffix;
}

static std::string guessContentType(const std::string& path){
    if(endsWith(path, ".css")){
        return "text/css; charset=utf-8";
    }

    if(endsWith(path, ".jpg")){
        return "image/jpeg";
    }
    
    if(endsWith(path, ".jpeg")){
        return "image/jpeg";
    }
    
    if(endsWith(path, ".png")){
        return "image/png";
    }
    
    if(endsWith(path, ".gif")){
        return "image/gif";
    }
    return "application/octet-stream";
}

bool isStaticFileRequest(const HttpRequest& request){
    return request.method == "GET" && request.path.rfind("/static/",0) == 0;
}


std::string handleStaticFileRequest(const HttpRequest& request){
    std::string file_path = request.path.substr(1);
    std::string content = readBinaryFile(file_path);

    if(content.empty()){
        return buildHttpResponse("Static file not found", "404 Not Found", "text/plain; charset=utf-8");
    }

    std::string content_type = guessContentType(file_path);
    return buildHttpResponse(content, "200 OK", content_type);
}


