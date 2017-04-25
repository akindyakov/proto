#include "args.h"
#include "map_server.h"

#include <tools/tests/ut.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <thread>
#include <chrono>

std::string DefaultMapText = R"FieldMap(10
10
iiiiiiiiii
i........i
i.w......i
i...w....i
i........i
i........i
i...w....i
i........i
i........i
iiiiiiiiii
)FieldMap";

Map::Field CreateField(const ArgsMap& args) {
    const auto& mapFileName = args["map-file"];
    if (mapFileName.empty()) {
        auto in = std::istringstream(DefaultMapText);
        return Map::ScanFromText(in);
    }
    auto dataFile = std::ifstream(mapFileName.as<std::string>());
    return Map::ScanFromText(dataFile);
}

int main(int argn, char** argv) {
    auto args = Argparse(argn, argv);
    if (!std::get<1>(args)) {
        return 1;
    }
    auto field = CreateField(std::get<0>(args));
    MapServer server{
        std::make_unique<jsonrpc::HttpServer>(
            std::get<0>(args)["port"].as<unsigned>()
        ),
        field
    };

    server.StartListening();
    while (true) {
        Map::PrintToText(std::cout, field);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    server.StopListening();
    return 0;
}
