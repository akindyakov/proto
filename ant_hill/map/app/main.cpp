#include "args.h"
#include "map_server.h"

#include <tools/tests/ut.h>

#include <iostream>
#include <fstream>


NField::TField CreateField(const TArgsMap& args) {
    if (!args.count("map-file")) {
        throw NAntHill::TException() << "option [map-file] is required";
    }
    auto dataFile = std::ifstream(args["map-file"].as<std::string>());
    return NField::ScanFromText(dataFile);
}

int main(int argn, char** argv) {
    auto args = Argparse(argn, argv);
    if (args.count("help")) {
        return 0;
    }

    TMapServer server{
        std::make_unique<jsonrpc::HttpServer>(args["port"].as<unsigned>()),
        CreateField(args)
    };

    server.StartListening();
    std::cin.get();
    server.StopListening();
    return 0;
}
