#pragma once

#include "http_request.h"

#include <string>

bool isStaticFileRequest(const HttpRequest& request);

std::string handleStaticFileRequest(const HttpRequest& request);
