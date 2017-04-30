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
    return first.X != second.X && first.X != second.X;
}

bool operator!=(const Point& first, const Point& second) {
    return first.X != second.X && first.X != second.X;
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

Grain::Grain(EMaterial material)
    : Material(material)
{
}

Grain::Grain(Grain&& other)
    : Material(other.Material)
{
}

Grain::Grain(const Grain& other)
    : Material(other.Material)
{
}

Grain& Grain::operator=(Grain&& other) {
    Material = other.Material;
    return *this;
}

Grain& Grain::operator=(const Grain& other) {
    Material = other.Material;
    return *this;
}

}  // namespace Map

namespace {
    static constexpr auto DimensionsDelimiter = ',';
    static constexpr auto PointLeftBrace = '(';
    static constexpr auto PointRightBrace = ')';
    static constexpr auto VectorLeftBrace = '<';
    static constexpr auto VectorRightBrace = '>';
    static constexpr auto Space = ' ';
}

std::ostream& operator<<(std::ostream& os, const Map::Point& pt) {
    os
        << PointLeftBrace
        << pt.X << DimensionsDelimiter << pt.Y
        << PointRightBrace
    ;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Map::Vector& vect) {
    os
        << VectorLeftBrace
        << vect.X << DimensionsDelimiter << vect.Y
        << VectorRightBrace
    ;
    return os;
}

std::istream& operator>>(std::istream& is, Map::Point& pt) {
    auto ch = Space;
    is >> std::skipws >> ch;
    AntHill::Validate(ch, PointLeftBrace);
    is >> pt.X >> ch;
    AntHill::Validate(ch, DimensionsDelimiter);
    is >> pt.Y >> ch;
    AntHill::Validate(ch, PointRightBrace);
    is >> std::noskipws;
    return is;
}

std::istream& operator>>(std::istream& is, Map::Vector& vect) {
    is >> std::skipws;
    auto ch = Space;
    is >> std::skipws >> ch;
    AntHill::Validate(ch, VectorLeftBrace);
    is >> vect.X >> ch;
    AntHill::Validate(ch, DimensionsDelimiter);
    is >> vect.Y >> ch;
    AntHill::Validate(ch, VectorRightBrace);
    is >> std::noskipws;
    return is;
}

