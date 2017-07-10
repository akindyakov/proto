#include "validate.h"

#include <iostream>
#include <sstream>


void streamArgsToExceptionTest() {
    std::cerr << " - streamArgsToExceptionTest\n";
    {
        auto err = Lib::streamArgsToException(
            Lib::Exception{"You can sense it "}
        );
        if (err.message() != "You can sense it ") {
            throw Lib::Exception() << "Message must be exactly the same "
                << "'" << err.message() << "' and 'You can sense it '"
            ;
        }
    }
    {
        auto err = Lib::streamArgsToException(
            Lib::Exception{"Let's count: "},
            "0"
        );
        if (err.message() != "Let's count: 0") {
            throw Lib::Exception() << "Message must be exactly the same "
                << "'" << err.message() << "' and 'Let's count: 0'"
            ;
        }
    }
    {
        auto err = Lib::streamArgsToException(
            Lib::Exception{"Let's count: "},
            "0, ",
            "1, ",
            "2!"
        );
        if (err.message() != "Let's count: 0, 1, 2!") {
            throw Lib::Exception() << "Message must be exactly the same "
                << "'" << err.message() << "' and 'Let's count: 0, 1, 2!'"
            ;
        }
    }
    {
        auto err = Lib::streamArgsToException(
            Lib::Exception{"Let's count: "},
            "0, ",
            1, ", ",
            "two, ",
            std::string("3!")
        );
        if (err.message() != "Let's count: 0, 1, two, 3!") {
            throw Lib::Exception() << "Message must be exactly the same "
                << "'" << err.message() << "' and 'Let's count: 0, 1, two, 3!'"
            ;
        }
    }
}

int main(int argn, char** argv) {
    try {
        std::cerr << "validate_ut:\n";
        streamArgsToExceptionTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << except.what() << std::endl;
        return 1;
    }
    return 0;
}
