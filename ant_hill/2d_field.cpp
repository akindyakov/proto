#include "2d_field.h"

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

const TGrain* TCell::SeeGrain() {
    return Grain.get();
}

TCell& TField::Get(const TPoint& pt) {
    return Matrix.at(pt.X()).at(pt.Y());
}

const TCell& TField::Get(const TPoint& pt) const {
    return Matrix.at(pt.X()).at(pt.Y());
}
