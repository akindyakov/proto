#include "parser.h"

#include <tools/tests/ut.h>

#include <iostream>

void parseIntegerTest() {
    std::cerr << " - parseIntegerTest\n";
    auto cell = Lisp::parseInteger("123");
    ValidateEqual(
        cell.get<Lisp::Integer>(),
        Lisp::Integer{123}
    );
}

void parseFloatTest() {
    std::cerr << " - parseFloatTest\n";
    auto cell = Lisp::parseFloat("1.23");
}

void parseRationalTest() {
    std::cerr << " - parseRationalTest\n";
    const auto str = std::string{"123/156"};
    auto cell = Lisp::parseRational(str, str.find('/'));
}

//Cell readRealNumber(std::istream& is);
//Cell readSimpleCharacter(std::istream& is);
//Cell readStringValue(std::istream& is);
//std::string readName(std::istream& is);

int main() {
    try {
        std::cerr << "parser_ut:\n";
        parseIntegerTest();
        parseFloatTest();
        parseRationalTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}

