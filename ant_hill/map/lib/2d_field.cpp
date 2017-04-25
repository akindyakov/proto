#include "2d_field.h"
#include "map_symbols.h"

#include <iostream>

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

Field ScanFromText(std::istream& is) {
    auto ch = char{0};
    // std::cerr << "scan field" << std::endl;
    auto x = Measure{0};
    auto y = Measure{0};
    is >> x;
    is >> y;
    // std::cerr << "size: " << x << "x" << y << std::endl;
    auto field = Field{x, y};
    while (!is.eof() && is.good()) {
        is.get(ch);
        if (ch == '\n') {
            // std::cerr << '\n';
            --y;
            x = 0;
        } else {
            //std::cerr << "[" << ch << "]\n";
            EMaterial material = GetSymbolMap().GetMaterial(ch);
            if (material != EMaterial::EmptySpace) {
                field.At(Point(x, y)).Grain = Grain(material);
            }
            ++x;
        }
    }
    return field;
}

void PrintToText(std::ostream& os, const Field& field) {
    Measure xSize = field.GetXSize();
    Measure ySize = field.GetYSize();
    os << xSize << '\n';
    os << ySize << '\n';
    // std::cerr << "field: " << xSize << 'x' << ySize << std::endl;
    for (Measure y = ySize; y != 0; --y) {
        for (Measure x = 0; x < ySize; ++x) {
            const Grain& grain = field.At(Point(x, y - 1)).Grain;
            os << GetSymbolMap().GetSymbol(grain.SeeMaterial());
            // std::cerr << '.';
        }
        os << "\n";
        // std::cerr << '\n';
    }
}

}  // Map
