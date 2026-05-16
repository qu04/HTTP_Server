#pragma once
#include "http_request.h"

#include <string>

void logRequest(const HttpRequest& request,const std::string& status);
