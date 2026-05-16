#pragma once

#include "http_request.h"
#include "http_response.h"

#include <string>

bool isStaticFileRequest(const HttpRequest& request);

HttpResponse handleStaticFileRequest(const HttpRequest& request);
