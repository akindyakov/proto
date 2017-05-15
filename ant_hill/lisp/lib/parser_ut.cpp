#include "parser.h"

#include <tools/tests/ut.h>

#include <iostream>

void parseIntegerTest() {
    std::cerr << " - parseIntegerTest\n";
    auto cell = Lisp::parseInteger("123  ");
    ValidateEqual(
        cell.get<Lisp::Integer>(),
        Lisp::Integer{123}
    );
}

void parseFloatTest() {
    std::cerr << " - parseFloatTest\n";
    auto cell = Lisp::parseFloat("1.23  ");
    ValidateEqual(
        cell.get<Lisp::Float>(),
        Lisp::Float{1.23}
    );
}

void parseRationalTest() {
    std::cerr << " - parseRationalTest\n";
    const auto str = std::string{"1/2  "};
    auto cell = Lisp::parseRational(str, str.find('/'));
    ValidateEqual(
        cell.get<Lisp::Float>(),
        Lisp::Float{0.5}
    );
}

void readRealNumberTest() {
    std::cerr << " - readRealNumberTest\n";
    {
        auto text = std::string{"213180  \"\""};
        auto in = std::istringstream(text);
        auto cell = Lisp::readRealNumber(in);
        ValidateEqual(
            cell.get<Lisp::Integer>(),
            Lisp::Integer{213180}
        );
    }
    {
        auto text = std::string{"-2928 5"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readRealNumber(in);
        ValidateEqual(
            cell.get<Lisp::Integer>(),
            Lisp::Integer{-2928}
        );
    }
    {
        auto text = std::string{"4.36  1"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readRealNumber(in);
        ValidateEqual(
            cell.get<Lisp::Float>(),
            Lisp::Float{4.36}
        );
    }
    {
        auto text = std::string{"-82.91  4"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readRealNumber(in);
        ValidateEqual(
            cell.get<Lisp::Float>(),
            Lisp::Float{-82.91}
        );
    }
    {
        auto text = std::string{"-1/2 1"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readRealNumber(in);
        ValidateEqual(
            cell.get<Lisp::Float>(),
            Lisp::Float{-0.5}
        );
    }
    {
        auto text = std::string{"1/8 3"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readRealNumber(in);
        ValidateEqual(
            cell.get<Lisp::Float>(),
            Lisp::Float{0.125}
        );
    }
}

void readSimpleCharacterTest() {
    std::cerr << " - readSimpleCharacterTest\n";
    {
        auto text = std::string{"#\\@ 3"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readSimpleCharacter(in);
        ValidateEqual(
            cell.get<Lisp::Symbol>(),
            Lisp::Symbol{'@'}
        );
    }
    {
        auto text = std::string{"#\\\\  2"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readSimpleCharacter(in);
        ValidateEqual(
            cell.get<Lisp::Symbol>(),
            Lisp::Symbol{'\\'}
        );
    }
    {
        auto text = std::string{"#\\$  1"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readSimpleCharacter(in);
        ValidateEqual(
            cell.get<Lisp::Symbol>(),
            Lisp::Symbol{'$'}
        );
    }
}

void readStringValueTest() {
    std::cerr << " - readStringValueTest\n";
    {
        auto text = std::string{"\"If you're talking about what you can feel, what you can smell, what you can taste and see, then real is simply electrical signals interpreted by your brain.\"  1 2 3"};
        auto in = std::istringstream(text);
        auto cell = Lisp::readStringValue(in);
        ValidateEqual(
            cell.get<Lisp::String>(),
            Lisp::String{"If you're talking about what you can feel, what you can smell, what you can taste and see, then real is simply electrical signals interpreted by your brain."}
        );
    }
}

void readNameTest() {
    std::cerr << " - readNameTest\n";
    {
        auto text = std::string{"Welcome-to_the*Desert@of the real"};
        auto in = std::istringstream(text);
        auto name = Lisp::readName(in);
        ValidateEqual(
            name,
            std::string{"Welcome-to_the*Desert@of"}
        );
    }
}

int main() {
    try {
        std::cerr << "parser_ut:\n";
        parseIntegerTest();
        parseFloatTest();
        parseRationalTest();
        readRealNumberTest();
        readSimpleCharacterTest();
        readStringValueTest();
        readNameTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}

