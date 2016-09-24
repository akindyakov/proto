#include "map_server.h"
#include <iostream>


int main()
{
    // TODO: Put argument parsing here
    jsonrpc::HttpServer httpserver(8383);
    TMapServer s(httpserver);
    s.StartListening();
    std::cin.get();
    s.StopListening();
    return 0;
}
