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

bool operator!=(const Vector& first, const Vector& second) {
    return first.X != second.X || first.Y != second.Y;
}

bool operator!=(const Point& first, const Point& second) {
    return first.X != second.X || first.Y != second.Y;
}

Point& operator+=(Point& self, const Vector& shift) {
    self.X += shift.X;
    self.Y += shift.Y;
    return self;
}

Vector operator-(const Point& left, const Point& right) {
    return Vector(
        left.X - right.X,
        left.Y - right.Y
    );
}

Point operator+(const Point& base, const Vector& shift) {
    return Point(
        base.X + shift.X,
        base.Y + shift.Y
    );
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
        first.min == second.min
        && first.size == second.size
    );
}

std::ostream& operator<<(std::ostream& out, const Square& square) {
    out << "(map-square "
        << square.min
        << " "
        << square.size
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
        Exception() << "Wrong begining character"
    );
    is >> pt.X >> ch;
    Lib::validateEqual(
        ch, DimensionsDelimiter,
        Exception() << "Wrong separator"
    );
    is >> pt.Y >> ch;
    Lib::validateEqual(
        ch, PointRightBrace,
        Exception() << "Wrong ending character"
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
        Exception() << "Wrong begining character"
    );
    is >> vect.X >> ch;
    Lib::validateEqual(
        ch, DimensionsDelimiter,
        Exception() << "Wrong separator"
    );
    is >> vect.Y >> ch;
    Lib::validateEqual(
        ch, VectorRightBrace,
        Exception() << "Wrong ending character"
    );
    is >> std::noskipws;
    return is;
}
}  // namespace Map

