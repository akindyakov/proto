#pragma once

#include "map_symbols.h"

#include <array>
#include <cstdint>
#include <memory>
#include <vector>
#include <istream>
#include <ostream>

using TMeasure = unsigned;

class TPoint {
private:
    TMeasure XVal;
    TMeasure YVal;

public:
    TPoint(
        TMeasure x,
        TMeasure y
    );

    ~TPoint() = default;

    TMeasure X() const;
    TMeasure Y() const;

};

class TVector: public TPoint {
public:
    TVector(
        TMeasure x,
        TMeasure y
    );
    TVector(const TVector& other)
        : TPoint(other.X(), other.Y())
    {
    }
    TMeasure Lenght() const {
        return X()*X() + Y()*Y();
    }
};

TVector operator-(const TPoint& left, const TPoint& right) {
    return TVector(
        left.X() - right.X(),
        left.Y() - right.Y()
    );
}


using TGrainId = unsigned int;

class TGrain {
public:
    TGrain(EMaterial material);
    const EMaterial Material;

    bool IsNone() const;
};


class TMatrix {
public:
    TMatrix(TMeasure xSize, TMeasure ySize);

    void Resize(TMeasure xSize, TMeasure ySize);

    TMeasure GetXSize() const;
    TMeasure GetY Size() const;

    TCell& At(const TPoint& pt);
    const TCell& At(const TPoint& pt) const;

private:
    TMeasure XSize;
    std::vector<TCell> Field;
};


class IField {
public:
    TGrainId CreateGrain(TGrain&&);

    const TGrain& GetGrain(TGrainId) const;
    const TGrain& GetGrain(const TPoint&) const;

    bool TryMoveGrain(TGrainId, const TVector& to) const;
    void MoveGrain(TGrainId, const TVector& to);
};


TField ScanFromText(std::istream&);
void PrintToText(std::ostream&, const TField&);

