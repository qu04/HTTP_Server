#include "logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

static std::string getCurrentTimeString(){
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

void logRequest(const HttpRequest& request,const std::string& status){
    std::string current_time = getCurrentTimeString();
     std::cout << "[" << current_time << "] "
     << "[" << request.method << "] "
     << request.path << " "
     << status << std::endl;
     std::ofstream output("logs/server.log", std::ios::app);
     output <<  "[" << current_time << "] "
     << "[" << request.method << "] "
     << request.path << " "
     << status << std::endl;
}