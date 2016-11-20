#pragma once

#include <boost/program_options/variables_map.hpp>

#include <tuple>

using TArgsMap = boost::program_options::variables_map;

std::tuple<TArgsMap, bool> Argparse(int argn, char** argv);
