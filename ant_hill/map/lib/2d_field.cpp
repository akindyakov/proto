#include "2d_field.h"
#include "map_symbols.h"

#include <iostream>

namespace NField {

TVector& operator+=(TVector& self, const TVector& shift) {
    self.X += shift.X;
    self.Y += shift.Y;
    return self;
}

bool operator!=(const TVector& first, const TVector& second) {
    return first.X != second.X && first.X != second.X;
}

bool operator!=(const TPoint& first, const TPoint& second) {
    return first.X != second.X && first.X != second.X;
}

TPoint& operator+=(TPoint& self, const TVector& shift) {
    self.X += shift.X;
    self.Y += shift.Y;
    return self;
}

TVector operator-(const TPoint& left, const TPoint& right) {
    return TVector(
        left.X - right.X,
        left.Y - right.Y
    );
}

TPoint operator+(const TPoint& base, const TVector& shift) {
    return TPoint(
        base.X + shift.X,
        base.Y + shift.Y
    );
}

TGrain::TGrain(EMaterial material)
    : Material(material)
{
}

TGrain::TGrain(TGrain&& other)
    : Material(other.Material)
{
}

TGrain::TGrain(const TGrain& other)
    : Material(other.Material)
{
}

TGrain& TGrain::operator=(TGrain&& other) {
    Material = other.Material;
    return *this;
}

TGrain& TGrain::operator=(const TGrain& other) {
    Material = other.Material;
    return *this;
}

TField ScanFromText(std::istream& is) {
    auto ch = char{0};
    // std::cerr << "scan field" << std::endl;
    auto x = TMeasure{0};
    auto y = TMeasure{0};
    is >> x;
    is >> y;
    // std::cerr << "size: " << x << "x" << y << std::endl;
    auto field = TField{x, y};
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
                field.At(TPoint(x, y)).Grain = TGrain(material);
            }
            ++x;
        }
    }
    return field;
}

void PrintToText(std::ostream& os, const TField& field) {
    TMeasure xSize = field.GetXSize();
    TMeasure ySize = field.GetYSize();
    os << xSize << '\n';
    os << ySize << '\n';
    // std::cerr << "field: " << xSize << 'x' << ySize << std::endl;
    for (TMeasure y = ySize; y != 0; --y) {
        for (TMeasure x = 0; x < ySize; ++x) {
            const TGrain& grain = field.At(TPoint(x, y - 1)).Grain;
            os << GetSymbolMap().GetSymbol(grain.SeeMaterial());
            // std::cerr << '.';
        }
        os << "\n";
        // std::cerr << '\n';
    }
}

}  // NField
