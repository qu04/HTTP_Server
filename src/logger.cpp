#include "logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

static std::string getCurrentTimeString(){
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

void logRequest(const HttpRequest& request,const std::string& status){
     std::cout << "[" << getCurrentTimeString() << "] "
     << "[" << request.method << "] "
     << request.path << " "
     << status << std::endl;    
}
