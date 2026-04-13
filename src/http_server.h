#pragma once

#include "router.h"

class HttpServer {
public:
    HttpServer(int port, Router& router);
    void run();

private:
    int port_;
    Router& router_;
};
