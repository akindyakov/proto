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
    auto client = Map::JsonRPC::Client{httpclient};
    auto ant = Scout(client);
    try {
        ant.appear();
        while (true) {
            //std::this_thread::sleep_for(1s);

            auto start = std::chrono::high_resolution_clock::now();
            ant.step();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            std::cout << "Ping: " << elapsed.count() << "ms" << std::endl;
        }
    }
    catch (const jsonrpc::JsonRpcException& err)
    {
        std::cerr << err.what() << std::endl;
    }
}
