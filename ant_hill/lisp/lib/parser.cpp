#include "parser.h"


namespace Lisp {

bool RealNumberParser::checkPrefix(char ch) {
    return std::isdigit(ch) || ch == minusMark;
}

bool RealNumberParser::isAcceptable(char ch) {
    return (
        std::isdigit(ch)
        || ch == divMark
        || ch == decimalMark
        || ch == minusMark
    );
}

Cell RealNumberParser::parseInteger(const std::string& str) {
    return Cell(
        static_cast<Integer>(
            std::atoll(str.c_str())
        )
    );
}

Cell RealNumberParser::parseFloat(const std::string& str) {
    char* end;
    return Cell(
        static_cast<Float>(
            std::strtod(str.c_str(), &end)
        )
    );
}

Cell RealNumberParser::parseRational(const std::string& str, size_t slashPos) {
    auto numerator = std::atoll(str.substr(0, slashPos).c_str());
    auto deNumerator = std::atoll(str.substr(slashPos + 1).c_str());
    // FIXME: use truly rational type here
    return Cell(
        static_cast<Float>(
            numerator
        )
        / static_cast<Float>(
            deNumerator
        )
    );
}

Cell RealNumberParser::read(std::istream& is) {
    auto number = std::string{};
    while (is && isAcceptable(is.peek())) {
        number.push_back(is.get());
    }

    size_t pos = number.find(decimalMark);
    if (pos == std::string::npos) {
        pos = number.find(divMark);
        if (pos == std::string::npos) {
            return parseInteger(number);
        } else {
            return parseRational(number, pos);
        }
    }
    return parseFloat(number);
}

constexpr char StringValueParser::quote;

bool StringValueParser::checkPrefix(char ch) {
    return ch == quote;
}

Cell StringValueParser::read(std::istream& is) {
    auto value = String{};
    auto ch = String::value_type{};
    if (is && is.peek() == quote) {
        is.ignore();
    } else {
        throw ParserError() << "String value should to starts with '" << quote << "'";
    }
    while (is && is.get(ch) && ch != quote) {
        value.push_back(ch);
    }
    return Cell(value);
}

bool SimpleCharacterParser::checkPrefix(char ch) {
    return ch == '#';
}

Cell SimpleCharacterParser::read(std::istream& is) {
    auto str = std::string{};
    is >> str;
    auto ch = Symbol{};
    if (str.size() == 3) {
        ch = str.back();
    } else {
        throw ParserError() << "Simple character input should have prefix '#\\'";
    }
    return Cell(ch);
}

std::string NameParser::read(std::istream& is) {
    auto name = String{};
    // TODO: check characters is valid here
    is >> name;
    return name;
}

}  // namespace Lisp
