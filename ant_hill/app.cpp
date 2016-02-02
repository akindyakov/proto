/*
 * g++(clang++) -std=c++11
 *
 */
#include "2d_field.h"

#include <getopt.h>

#include <exception>
#include <iostream>
#include <sstream>
#include <string>


class ArgparseException: public std::exception {
public:
    ArgparseException() {}
    ArgparseException(const std::string& what) {
        whatHappen.append(what);
    }

    template<typename T>
    ArgparseException& operator<<(const T& val) {
        std::ostringstream ostr;
        ostr << val;
        whatHappen.append(ostr.str());
        return *this;
    }

    const char* what() const throw() override {
        return whatHappen.c_str();
    }

private:
    std::string whatHappen = "[argument parser exception] ";
};

struct TArgs {
    std::string input;
    std::string output;
};

TArgs argparse(int argn, char** argv) {
    static struct option long_options[] = {
        {"input",       required_argument,  0,  'i' },
        {"output",      required_argument,  0,  'o' },
        {"help",        no_argument,        0,  'h' },
        {0,             0,                  0,   0 }
    };

    TArgs args;

    int option_index = 0;
    while (
        int c = getopt_long(argn, argv, "i:o:?:h", long_options, &option_index)
    ) {
        if (c == -1) {
            break;
        } else if (c == 'i') {
            if (!optarg) {
                throw ArgparseException("empty value for option \'i\'");
            }
            args.input = optarg;
        } else if (c == 'o') {
            if (!optarg) {
                throw ArgparseException("empty value for option \'o\'");
            }
            args.output = optarg;
        } else if (c == 'h' || c == '?') {
            std::cerr << "Usage:\n"
                << " -i, --input : Input file\n"
                << " -o, --output : Output file\n"
                << " -h, -?, --help : Print this help\n"
            ;
            return args;
        } else {
            if (0 != opterr) {
                throw ArgparseException();
            }
        }
    }
    return args;
}

void App(const std::string&) {
    TField field;
    field.ScanFromText(std::cin);
    field.PrintToText(std::cout);
}

int main(int argn, char** argv) {
    try {
        auto args = argparse(argn, argv);
        App(args.input);
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}

