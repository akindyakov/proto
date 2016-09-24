#include "args.h"

#include <boost/program_options/variables_map.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <vector>
#include <string>
#include <iostream>

namespace NArgs = boost::program_options;

NArgs::variables_map Argparse(int argn, char** argv) {
    auto generic = NArgs::options_description{"Generic options"};
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
    ;

    auto config = NArgs::options_description{"Network configuration"};
    config.add_options()
        (
            "port",
            NArgs::value<unsigned>()->default_value(8383),
            "Map server port"
        ) (
            "host",
            NArgs::value<std::string>()->default_value("localhost"),
            "Map server hostname"
        )
    ;

    auto data = NArgs::options_description{"Construction options"};
    data.add_options()
        ("map-file", NArgs::value<std::string>(), "input map file")
    ;

    auto cmdline_options = NArgs::options_description{};
    cmdline_options.add(generic).add(config).add(data);

    auto args = NArgs::variables_map{};
    NArgs::store(NArgs::parse_command_line(argn, argv, cmdline_options), args);
    NArgs::notify(args);

    if (args.count("help")) {
        std::cerr << generic << '\n';
        std::cerr << config << '\n';
        std::cerr << data << '\n';
    }

    return args;
}
