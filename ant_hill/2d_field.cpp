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

TMatrix::TMatrix(
    TMeasure xSize,
    TMeasure ySize
)
    : XSize(xSize)
    , Field(xSize * ySize)
{
}

void TMatrix::Resize(TMeasure xSize, TMeasure ySize) {
    XSize = xSize;
    Field.resize(xSize * ySize);
}

TMeasure TMatrix::GetXSize() const {
    return XSize;
}

TMeasure TMatrix::GetYSize() const {
    return Field.size() / XSize;
}

TCell& TMatrix::At(const TPoint& pt) {
    return Field.at(pt.Y() * XSize + pt.X());
}

const TCell& At(const TPoint& pt) const {
    return Field.at(pt.Y() * XSize + pt.X());
}

TCell& TField::Get(const TPoint& pt) {
    return Matrix.At(pt);
}

const TCell& TField::Get(const TPoint& pt) const {
    return Matrix.At(pt);
}

void TField::ScanFromText(std::istream& is) {
    size_t cell = 0;
    char ch = 0;
    std::cerr << "scan field" << std::endl;
    TMeasure x = 0;
    TMeasure y = 0;
    is >> x;
    is >> y;
    std::cerr << "size: " << x << "x" << y << std::endl;
    Matrix.Resize(x, y);
    while (!is.eof() && is.good()) {
        is.get(ch);
        if (ch == '\n') {
            ++y;
            x = 0;
        } else {
            ++x;
            EMaterial material = GetSymbolMap().GetMaterial(ch);
            if (material != EMaterial::EmptySpace) {
                Matrix.At(TPoint(x,y)) = TCell(material);
            } else {
                Matrix.At(TPoint(x,y)).Release();
            }
        }
    }
}

void TField::PrintToText(std::ostream& os) const {
    TMeasure xSize = Field.XSize();
    TMeasure ySize = Field.YSize();
    os << xSize << '\n';
    os << ySize << '\n';
    for (TMeasure y = 0; y < ySize; ++y) {
        for (TMeasure x = 0; x < ySize; ++x) {
            const TCell& cell = Matrix.At(TPoint(x, y));
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
