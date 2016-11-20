#include "args.h"

#include <boost/program_options/variables_map.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace NArgs = boost::program_options;

std::tuple<NArgs::variables_map, bool> Argparse(int argn, char** argv) {
    auto generic = NArgs::options_description{"Generic options"};
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
    ;

    auto config = NArgs::options_description{"Map server configuration"};
    config.add_options()
        (
            "map-port",
            NArgs::value<int>()->default_value(8383),
            "Map server port"
        ) (
            "map-host",
            NArgs::value<std::string>()->default_value("localhost")->composing(),
            "Map server hostname"
        )
    ;

    auto unit = NArgs::options_description{"Ant unit configuration"};
    unit.add_options()
        (
            "name",
            NArgs::value<std::string>(),
            "Unit unique name"
        )
    ;

    auto hidden = NArgs::options_description{"Hidden options"};
    hidden.add_options()
        ("input-file", NArgs::value<std::vector<std::string>>(), "input file")
    ;

    auto cmdline_options = NArgs::options_description{};
    cmdline_options.add(generic).add(config).add(unit).add(hidden);

    auto args = NArgs::variables_map{};
    NArgs::store(NArgs::parse_command_line(argn, argv, cmdline_options), args);
    NArgs::notify(args);

    bool isValid = true;
    if (args.count("help")) {
        std::cerr << generic << '\n';
        std::cerr << config << '\n';
        std::cerr << hidden << '\n';
        isValid = false;
    }

    return std::make_tuple(args, isValid);
}
