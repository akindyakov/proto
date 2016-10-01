#include "args.h"
#include "map_server.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <fstream>


TMapServer CreateServer(const TArgsMap& args) {
    if (!args.count("map-file")) {
        throw NAntHill::TException() << "option [map-file] is required";
    }
    auto dataFile = std::ifstream(args["map-file"].as<std::string>());
    return TMapServer(
        std::make_unique<jsonrpc::HttpServer>(args["port"].as<unsigned>()),
        NField::ScanFromText(dataFile)
    );
}

int main(int argn, char** argv) {
    auto args = Argparse(argn, argv);
    if (args.count("help")) {
        return 0;
    }

    auto server = CreateServer(args);

    server.StartListening();
    std::cin.get();
    server.StopListening();
    return 0;
}
