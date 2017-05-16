#pragma once

#include "cell.h"

#include <tools/exception.h>

#include <cstdlib>
#include <istream>


namespace Lisp {

class ParserError
    : public Exception
{
public:
    explicit ParserError()
        : Exception("Parser error. ")
    {
    }
};

class RealNumberParser {
public:
    static bool checkPrefix(char ch);
    static Cell read(std::istream& is);

    static Cell parseInteger(const std::string& str);
    static Cell parseFloat(const std::string& str);
    static Cell parseRational(const std::string& str, size_t slashPos);

private:
    static constexpr char decimalMark = '.';
    static constexpr char minusMark = '-';
    static constexpr char divMark = '/';
    static bool isAcceptable(char ch);
};

class SimpleCharacterParser {
public:
    static bool checkPrefix(char ch);
    static Cell read(std::istream& is);
};

class StringValueParser {
public:
    static bool checkPrefix(char ch);
    static Cell read(std::istream& is);

private:
    static constexpr char quote = '"';
};

class NameParser {
public:
    static std::string read(std::istream& is);
};

}  // namespace Lisp
