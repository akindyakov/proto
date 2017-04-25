#include "args.h"

#include <boost/program_options/variables_map.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace Args = boost::program_options;

std::tuple<Args::variables_map, bool> Argparse(int argn, char** argv) {
    auto generic = Args::options_description{"Generic options"};
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
    ;

    auto config = Args::options_description{"Map server configuration"};
    config.add_options()
        (
            "map-port",
            Args::value<int>()->default_value(8383),
            "Map server port"
        ) (
            "map-host",
            Args::value<std::string>()->default_value("localhost")->composing(),
            "Map server hostname"
        )
    ;

    auto unit = Args::options_description{"Ant unit configuration"};
    unit.add_options()
        (
            "name",
            Args::value<std::string>(),
            "Unit unique name"
        )
    ;

    auto hidden = Args::options_description{"Hidden options"};
    hidden.add_options()
        ("input-file", Args::value<std::vector<std::string>>(), "input file")
    ;

    auto cmdline_options = Args::options_description{};
    cmdline_options.add(generic).add(config).add(unit).add(hidden);

    auto args = Args::variables_map{};
    Args::store(Args::parse_command_line(argn, argv, cmdline_options), args);
    Args::notify(args);

    bool isValid = true;
    if (args.count("help")) {
        std::cerr << generic << '\n';
        std::cerr << config << '\n';
        std::cerr << hidden << '\n';
        isValid = false;
    }

    return std::make_tuple(args, isValid);
}
