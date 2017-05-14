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

Cell parseInteger(const std::string& str);
Cell parseFloat(const std::string& str);
Cell parseRational(const std::string& str, size_t slashPos);

Cell readRealNumber(std::istream& is);
Cell readSimpleCharacter(std::istream& is);
Cell readStringValue(std::istream& is);

std::string readName(std::istream& is);

}  // namespace Lisp
