#pragma once

#include <boost/program_options/variables_map.hpp>

#include <tuple>

using ArgsMap = boost::program_options::variables_map;

std::tuple<ArgsMap, bool> Argparse(int argn, char** argv);
