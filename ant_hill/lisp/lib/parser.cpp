#include "parser.h"


namespace Lisp {

Cell parseInteger(const std::string& str) {
    return Cell(
        static_cast<Integer>(
            std::atoll(str.c_str())
        )
    );
}

Cell parseFloat(const std::string& str) {
    char* end;
    return Cell(
        static_cast<Float>(
            std::strtod(str.c_str(), &end)
        )
    );
}

Cell parseRational(const std::string& str, size_t slashPos) {
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

Cell readRealNumber(std::istream& is) {
    auto number = std::string{};
    is >> number;
    size_t pos = number.find('.');
    if (pos == std::string::npos) {
        pos = number.find('/');
        if (pos == std::string::npos) {
            return parseInteger(number);
        } else {
            return parseRational(number, pos);
        }
    }
    return parseFloat(number);
}

Cell readStringValue(std::istream& is) {
    static constexpr auto quote = '"';
    auto value = String{};
    auto ch = String::value_type{};
    if (is && is.peek() == quote) {
        is.ignore();
    } else {
        throw ParserError() << "String value should to starts with '" << quote << "'";
    }
    while (is && is.peek() != quote) {
        is.get(ch);
        value.push_back(ch);
    }
    if (is) {
        // skip last quote
        is.ignore();
    }
    return Cell(value);
}

Cell readSimpleCharacter(std::istream& is) {
    auto str = std::string{};
    is >> str;
    if (str.size() != 3) {
        throw ParserError() << "Simple character input should have prefix '#\\'";
    }
    auto ch = Symbol{};
    ch = str.back();
    return Cell(ch);
}

std::string readName(std::istream& is) {
    auto name = String{};
    // TODO: check characters is valid here
    is >> name;
    return name;
}

}  // namespace Lisp
