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

int main(int argn, char** argv) {
    auto parsedArg = Argparse(argn, argv);
    if (!std::get<1>(parsedArg)) {
        return 1;
    }
    auto args = std::get<0>(parsedArg);

    auto server = std::unique_ptr<MapServer>{};
    {
        const auto& mapFileName = args["map-file"];
        auto dataFile = std::ifstream(mapFileName.as<std::string>());

        server = std::make_unique<MapServer>(
            std::make_unique<jsonrpc::HttpServer>(
                args["port"].as<unsigned>()
            ),
            dataFile
        );
    }

    server->StartListening();
    while (true) {
        server->print(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    server->StopListening();
    return 0;
}
