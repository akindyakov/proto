#include "args.h"

#include <ant/lib/ant.h>

#include <map/rpc/client.h>

#include <jsonrpccpp/client/connectors/httpclient.h>

#include <iostream>
#include <thread>
#include <chrono>


int main(int argn, char** argv) {

    auto args = Argparse(argn, argv);
    if (!std::get<1>(args)) {
        return 1;
    }

    using namespace std::chrono_literals;
    auto httpclient = jsonrpc::HttpClient("http://localhost:8383");
    auto client = Map::JsonRPCClient{httpclient};
    auto ant = Ant::Scout(client);
    try {
        while (ant.run());
        //while (ant.run()) {
           //auto start = std::chrono::high_resolution_clock::now();
           //auto end = std::chrono::high_resolution_clock::now();
           //std::chrono::duration<double, std::milli> elapsed = end - start;
           //std::cout << "Ping: " << elapsed.count() << "ms" << std::endl;
        //}
    }
    catch (const jsonrpc::JsonRpcException& err)
    {
        std::cerr << err.what() << std::endl;
    }
}
