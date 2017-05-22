#include "parser.h"


namespace Lisp {

void skipSpaces(std::istream& in) {
    while (in.good() && std::isspace(in.peek())) {
        in.ignore();
    }
}

bool RealNumberParser::checkPrefix(char ch) {
    return std::isdigit(ch) || ch == minusMark;
}

bool charIsService(char ch) {
    return (
        std::isspace(ch)
        || ch == PARENT_OPEN
        || ch == PARENT_CLOSE
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
    while (is.good() && !charIsService(is.peek())) {
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
    if (is.good() && is.peek() == quote) {
        is.ignore();
    } else {
        throw ParserError() << "String value should to starts with '" << quote << "'";
    }
    while (is.good() && is.get(ch) && ch != quote) {
        std::cerr << "str ch: " << char(ch) << '\n';
        value.push_back(ch);
    }
    return Cell(std::move(value));
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
    // TODO: check characters is valid here
    auto name = std::string{};
    while (is.good() && !charIsService(is.peek())) {
        name.push_back(is.get());
    }
    std::cerr << "reader name: " << name << '\n';
    return name;
}

std::string ExprParser::read(std::istream& is) {
    auto counter = int{0};
    auto fbody = std::string{};
    auto ch = char{};
    do {
        is.get(ch);
        counter += ch == PARENT_OPEN ? 1 : 0;
        counter -= ch == PARENT_CLOSE ? 1 : 0;
        fbody.push_back(ch);
    } while (is.good() && counter != 0);
    return fbody;
}

void ExprParser::readBegin(std::istream& in) {
    skipSpaces(in);
    if (!in.good()) {
        throw Exception() << "Unexpected end of file at the expression begining (parentheses group)";
    }
    char ch = in.get();
    if (ch != PARENT_OPEN) {
        throw Exception() << "Wrong parentheses group first character: '" << ch << "'\n";
    }
}
void ExprParser::readEnd(std::istream& in) {
    skipSpaces(in);
    if (!in.good()) {
        throw Exception() << "Unexpected end of file at the end of parentheses group";
    }
    char ch = in.get();
    if (ch != PARENT_CLOSE) {
        throw Exception()
            << "Wrong parentheses group last character: '" << ch << "'\n";
    }
}

bool ExprParser::checkPrefix(char ch) {
    return ch == PARENT_OPEN;
}

}  // namespace Lisp
