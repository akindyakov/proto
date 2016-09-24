#pragma once

#include <boost/program_options/variables_map.hpp>

using TArgsMap = boost::program_options::variables_map;

TArgsMap Argparse(int argn, char** argv);
