#include "args.h"
#include "map_server.h"

#include <tools/tests/ut.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>

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

NField::TField CreateField(const TArgsMap& args) {
    const auto& mapFileName = args["map-file"];
    if (mapFileName.empty()) {
        auto in = std::istringstream(DefaultMapText);
        return NField::ScanFromText(in);
    }
    auto dataFile = std::ifstream(mapFileName.as<std::string>());
    return NField::ScanFromText(dataFile);
}

int main(int argn, char** argv) {
    auto args = Argparse(argn, argv);
    if (!std::get<1>(args)) {
        return 1;
    }
    TMapServer server{
        std::make_unique<jsonrpc::HttpServer>(
            std::get<0>(args)["port"].as<unsigned>()
        ),
        CreateField(std::get<0>(args))
    };

    server.StartListening();
    std::cin.get();
    server.StopListening();
    return 0;
}
