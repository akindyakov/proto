#include "2d_field.h"
#include "map_symbols.h"

#include <iostream>
#include <ios>

namespace Map {

Vector& operator+=(Vector& self, const Vector& shift) {
    self.X += shift.X;
    self.Y += shift.Y;
    return self;
}

std::ostream& operator<<(std::ostream& out, const SimpleCell& cell) {
    out << GetSymbolMap().GetSymbol(cell.grain);
    return out;
}

std::istream& operator>>(std::istream& in, SimpleCell& cell) {
    auto ch = char{0};
    in.get(ch);
    cell.grain = GetSymbolMap().GetMaterial(ch);
    return in;
}

bool operator==(const Square& first, const Square& second) {
    return (
        first.min() == second.min()
        && first.size() == second.size()
    );
}

std::ostream& operator<<(std::ostream& out, const Square& square) {
    out << "(map-square "
        << square.min()
        << " "
        << square.size()
        << ")";
    return out;
}

namespace {
    static constexpr auto DimensionsDelimiter = ',';
    static constexpr auto PointLeftBrace = '(';
    static constexpr auto PointRightBrace = ')';
    static constexpr auto VectorLeftBrace = '<';
    static constexpr auto VectorRightBrace = '>';
    static constexpr auto Space = ' ';
}

std::ostream& operator<<(std::ostream& os, const Point& pt) {
    os
        << PointLeftBrace
        << pt.X << DimensionsDelimiter << pt.Y
        << PointRightBrace
    ;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector& vect) {
    os
        << VectorLeftBrace
        << vect.X << DimensionsDelimiter << vect.Y
        << VectorRightBrace
    ;
    return os;
}

std::istream& operator>>(std::istream& is, Point& pt) {
    auto ch = Space;
    is >> std::skipws >> ch;
    Lib::validateEqual(
        ch, PointLeftBrace,
        Lib::Exception(), "Wrong begining character"
    );
    is >> pt.X >> ch;
    Lib::validateEqual(
        ch, DimensionsDelimiter,
        Lib::Exception(), "Wrong separator"
    );
    is >> pt.Y >> ch;
    Lib::validateEqual(
        ch, PointRightBrace,
        Lib::Exception(), "Wrong ending character"
    );
    is >> std::noskipws;
    return is;
}

std::istream& operator>>(std::istream& is, Vector& vect) {
    is >> std::skipws;
    auto ch = Space;
    is >> std::skipws >> ch;
    Lib::validateEqual(
        ch, VectorLeftBrace,
        Lib::Exception(), "Wrong begining character"
    );
    is >> vect.X >> ch;
    Lib::validateEqual(
        ch, DimensionsDelimiter,
        Lib::Exception(), "Wrong separator"
    );
    is >> vect.Y >> ch;
    Lib::validateEqual(
        ch, VectorRightBrace,
        Lib::Exception(), "Wrong ending character"
    );
    is >> std::noskipws;
    return is;
}
}  // namespace Map

