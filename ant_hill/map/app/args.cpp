#include "args.h"

#include <boost/program_options/variables_map.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <vector>
#include <string>
#include <iostream>

namespace opts = boost::program_options;

opts::variables_map Argparse(int argn, char** argv) {
    auto generic = opts::options_description{"Generic options"};
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
    ;

    auto config = opts::options_description{"Network configuration"};
    config.add_options()
        (
            "map-port",
            opts::value<int>()->default_value(8383),
            "Map server port"
        ) (
            "map-host",
            opts::value<std::string>()->default_value("localhost")->composing(),
            "Map server hostname"
        )
    ;

    auto data = opts::options_description{"Construction options"};
    data.add_options()
        ("map-file", opts::value<std::string>(), "input map file")
    ;

    auto cmdline_options = opts::options_description{};
    cmdline_options.add(generic).add(config).add(data);

    auto args = opts::variables_map{};
    opts::store(opts::parse_command_line(argn, argv, cmdline_options), args);
    opts::notify(args);

    if (args.count("help")) {
        std::cerr << generic << '\n';
        std::cerr << config << '\n';
        std::cerr << data << '\n';
    }

    return args;
}
