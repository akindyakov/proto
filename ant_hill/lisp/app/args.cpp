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

    auto config = Args::options_description{"Network configuration"};
    config.add_options()
        (
            "host,o",
            Args::value<std::string>()->default_value("a.out"),
            "Place output in file file."
        )
    ;

    auto cmdline_options = Args::options_description{};
    cmdline_options.add(generic).add(config);

    auto args = Args::variables_map{};
    Args::store(Args::parse_command_line(argn, argv, cmdline_options), args);
    Args::notify(args);

    bool isValid = true;
    if (args.count("help")) {
        std::cerr << generic << '\n';
        std::cerr << config << '\n';
        isValid = false;
    }

    return std::make_tuple(args, isValid);
}
