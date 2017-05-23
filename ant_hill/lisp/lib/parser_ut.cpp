#include "parser.h"

#include <tools/tests/ut.h>

#include <iostream>

void skipSpacesTest() {
    std::cerr << " - skipSpacesTest\n";
    {
        auto strIn = "   \t\t \t\nflow ";
        auto in = std::istringstream{strIn};
        Lisp::skipSpaces(in);
        char ans = in.get();
        ValidateEqual(ans, 'f');
    }
}

void charIsServiceTest() {
    std::cerr << " - charIsServiceTest\n";
    ValidateEqual(Lisp::charIsService('('), true);
    ValidateEqual(Lisp::charIsService(')'), true);
    ValidateEqual(Lisp::charIsService(Lisp::PARENT_OPEN), true);
    ValidateEqual(Lisp::charIsService(Lisp::PARENT_CLOSE), true);
    ValidateEqual(Lisp::charIsService(' '), true);
    ValidateEqual(Lisp::charIsService('\t'), true);
    ValidateEqual(Lisp::charIsService('\n'), true);
    ValidateEqual(Lisp::charIsService('\v'), true);

    ValidateEqual(Lisp::charIsService(','), false);
    ValidateEqual(Lisp::charIsService('.'), false);
    ValidateEqual(Lisp::charIsService(':'), false);
    ValidateEqual(Lisp::charIsService('{'), false);
    ValidateEqual(Lisp::charIsService('['), false);
    ValidateEqual(Lisp::charIsService('$'), false);
    ValidateEqual(Lisp::charIsService('-'), false);
    ValidateEqual(Lisp::charIsService('+'), false);
}

void parseIntegerTest() {
    std::cerr << " - parseIntegerTest\n";
    auto cell = Lisp::RealNumberParser::parseInteger("123  ");
    ValidateEqual(
        cell.get<Lisp::Integer>(),
        Lisp::Integer{123}
    );
}

void parseFloatTest() {
    std::cerr << " - parseFloatTest\n";
    auto cell = Lisp::RealNumberParser::parseFloat("1.23  ");
    ValidateEqual(
        cell.get<Lisp::Float>(),
        Lisp::Float{1.23}
    );
}

void parseRationalTest() {
    std::cerr << " - parseRationalTest\n";
    const auto str = std::string{"1/2  "};
    auto cell = Lisp::RealNumberParser::parseRational(str, str.find('/'));
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
        auto cell = Lisp::RealNumberParser::read(in);
        ValidateEqual(
            cell.get<Lisp::Integer>(),
            Lisp::Integer{213180}
        );
    }
    {
        auto text = std::string{"-2928 5"};
        auto in = std::istringstream(text);
        auto cell = Lisp::RealNumberParser::read(in);
        ValidateEqual(
            cell.get<Lisp::Integer>(),
            Lisp::Integer{-2928}
        );
    }
    {
        auto text = std::string{"4.36  1"};
        auto in = std::istringstream(text);
        auto cell = Lisp::RealNumberParser::read(in);
        ValidateEqual(
            cell.get<Lisp::Float>(),
            Lisp::Float{4.36}
        );
    }
    {
        auto text = std::string{"-82.91  4"};
        auto in = std::istringstream(text);
        auto cell = Lisp::RealNumberParser::read(in);
        ValidateEqual(
            cell.get<Lisp::Float>(),
            Lisp::Float{-82.91}
        );
    }
    {
        auto text = std::string{"-1/2 1"};
        auto in = std::istringstream(text);
        auto cell = Lisp::RealNumberParser::read(in);
        ValidateEqual(
            cell.get<Lisp::Float>(),
            Lisp::Float{-0.5}
        );
    }
    {
        auto text = std::string{"1/8 3"};
        auto in = std::istringstream(text);
        auto cell = Lisp::RealNumberParser::read(in);
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
        auto cell = Lisp::SimpleCharacterParser::read(in);
        ValidateEqual(
            cell.get<Lisp::Symbol>(),
            Lisp::Symbol{'@'}
        );
    }
    {
        auto text = std::string{"#\\\\  2"};
        auto in = std::istringstream(text);
        auto cell = Lisp::SimpleCharacterParser::read(in);
        ValidateEqual(
            cell.get<Lisp::Symbol>(),
            Lisp::Symbol{'\\'}
        );
    }
    {
        auto text = std::string{"#\\$  1"};
        auto in = std::istringstream(text);
        auto cell = Lisp::SimpleCharacterParser::read(in);
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
        auto cell = Lisp::StringValueParser::read(in);
        ValidateEqual(
            cell.get<Lisp::String>(),
            Lisp::String{"If you're talking about what you can feel, what you can smell, what you can taste and see, then real is simply electrical signals interpreted by your brain."}
        );
    }
}

void readNameTest() {
    std::cerr << " - readNameTest\n";
    {
        auto text = std::string{"the-real"};
        auto in = std::istringstream(text);
        auto name = Lisp::NameParser::read(in);
        ValidateEqual(
            name,
            std::string{"the-real"}
        );
    }
    {
        auto text = std::string{"Welcome-to_the*Desert@of the real"};
        auto in = std::istringstream(text);
        auto name = Lisp::NameParser::read(in);
        ValidateEqual(
            name,
            std::string{"Welcome-to_the*Desert@of"}
        );
    }
}

void exprParserTest() {
    std::cerr << " - exprParserTest\n";
    {
        auto text = std::string{"()"};
        ValidateEqual(
            Lisp::ExprParser::checkPrefix(text[0]),
            true
        );
        auto in = std::istringstream(text);
        auto ans = Lisp::ExprParser::read(in);
        ValidateEqual(ans, text);
    }
    {
        auto text = std::string{"(a b c -12 d 0.22 f) (eq 1 2)"};
        ValidateEqual(
            Lisp::ExprParser::checkPrefix(text[0]),
            true
        );
        auto in = std::istringstream(text);
        auto ans = Lisp::ExprParser::read(in);
        ValidateEqual(
            ans,
            std::string{"(a b c -12 d 0.22 f)"}
        );
    }
}

void exprParseBeginEndTest() {
    std::cerr << " - exprParseBeginEndTest\n";
    {
        auto strIn = "   \t\t \t\n(Serious 123)";
        auto in = std::istringstream{strIn};
        Lisp::ExprParser::readBegin(in);
        char ans = in.get();
        ValidateEqual(ans, 'S');
    }
    {
        auto strIn = "   \t \n\t\n \t)why?";
        auto in = std::istringstream{strIn};
        Lisp::ExprParser::readEnd(in);
        char ans = in.get();
        ValidateEqual(ans, 'w');
    }
}

int main() {
    try {
        std::cerr << "parser_ut:\n";
        skipSpacesTest();
        charIsServiceTest();
        parseIntegerTest();
        parseFloatTest();
        parseRationalTest();
        readRealNumberTest();
        readSimpleCharacterTest();
        readStringValueTest();
        readNameTest();
        exprParserTest();
        exprParseBeginEndTest();
        std::cerr << std::endl;
    } catch (const std::exception& except) {
        std::cerr << "failed: " << except.what() << std::endl;
        return 1;
    }
    return 0;
}

