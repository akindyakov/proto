#include "2d_field.h"
#include "map_symbols.h"

#include <iostream>

TPoint::TPoint(
    TMeasure x,
    TMeasure y
)
    : Dimensions {{x, y}}
{
}

TMeasure TPoint::X() const {
    return Dimensions[EX];
}

TMeasure TPoint::Y() const {
    return Dimensions[EY];
}

TGrain::TGrain(EMaterial material)
    : Material(material)
{
}

bool TCell::IsOccupied() const {
    return bool(Grain);
}

void TCell::Release() {
    Grain.reset();
}

std::unique_ptr<TGrain> TCell::GetGrain() {
    // TODO: mutex required
    return std::move(Grain);
}

void TCell::PutGrain(std::unique_ptr<TGrain>&& grain) {
    if (Grain) {
        Grain = std::move(grain);
    } else {
        throw std::exception();
    }
}

void TCell::PutGrain(EMaterial material) {
    PutGrain(std::make_unique<TGrain>(material));
}

const TGrain* TCell::SeeGrain() const {
    return Grain.get();
}

TCell& TField::Get(const TPoint& pt) {
    return Matrix.at(pt.X()).at(pt.Y());
}

const TCell& TField::Get(const TPoint& pt) const {
    return Matrix.at(pt.X()).at(pt.Y());
}

void TField::ScanFromText(std::istream& is) {
    size_t cell = 0;
    char ch = 0;
    Matrix.clear();
    TRow row;
    std::cerr << "scan..." << std::endl;
    while (!is.eof() && is.good()) {
        is.get(ch);
        if (ch == '\n') {
            cell = 0;
            if (!row.empty()) {
                Matrix.push_back(std::move(row));
            }
        } else {
            EMaterial material = GetSymbolMap().GetMaterial(ch);
            if (material != EMaterial::EmptySpace) {
                row.emplace_back(material);
            } else {
                row.emplace_back();
            }
            ++cell;
        }
    }
}

void TField::PrintToText(std::ostream& os) const {
    for (const auto& row : Matrix) {
        for (const auto& cell : row) {
            if (cell.IsOccupied()) {
                os << GetSymbolMap().GetSymbol(cell.SeeGrain()->Material);
            } else {
                os << GetSymbolMap().GetSymbol(EMaterial::EmptySpace);
            }
        }
        os << "\n";
    }
}

TOwnerId IdGenerator() {
    static TOwnerId id = 0;
    return ++id;
}
