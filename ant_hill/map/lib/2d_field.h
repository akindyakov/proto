#pragma once

#include "map_symbols.h"

#include <tools/tests/ut.h>

#include <array>
#include <cstdint>
#include <istream>
#include <memory>
#include <mutex>
#include <ostream>
#include <vector>

namespace NField {

using TMeasure = int;

class TPoint {
public:
    TMeasure X;
    TMeasure Y;

public:
    TPoint(
        TMeasure x,
        TMeasure y
    );

    ~TPoint() = default;
};

inline bool operator == (
    const TPoint& first
    , const TPoint& second
) {
    return (
        first.X == second.X
        && first.Y == second.Y
    );
}

class TVector: public TPoint {
public:
    TVector(
        TMeasure x,
        TMeasure y
    );

    TVector(const TVector& other)
        : TPoint(other.X, other.Y)
    {
    }

    TMeasure Lenght() const {
        return X*X + Y*Y;
    }

};

bool operator!=(const TVector& first, const TVector& second);

TPoint& operator+=(TPoint& self, const TVector& shift);

TVector operator-(const TPoint& left, const TPoint& right);
TPoint operator+(const TPoint& base, const TVector& shift);

class TGrain {
public:
    TGrain(EMaterial material);

    TGrain(TGrain&& other);
    TGrain(const TGrain& other);
    TGrain& operator=(TGrain&& other);
    TGrain& operator=(const TGrain& other);

    EMaterial SeeMaterial() const noexcept {
        return Material;
    }

    bool IsNone() const {
        return Material == EMaterial::EmptySpace;
    }

private:

    EMaterial Material;
};

struct TCell {
    TCell()
        : Grain(EMaterial::EmptySpace)
    {
    }

    TGrain Grain;
};


class TField {
private:
    size_t GetIndexByPoint(const TPoint& pt) const {
        auto signedIndex = (pt.X - bottomLeft_.X) + (pt.Y - bottomLeft_.Y) * size_.X;
        if (signedIndex < 0) {
            throw NAntHill::TException("Access by outrange point");
        }
        auto index = static_cast<size_t>(signedIndex);
        // std::cerr << "index: " << index << std::endl;
        return index;
    }

public:
    TField(TMeasure xSize, TMeasure ySize, TPoint bottomLeft=TPoint{0, 0})
        : size_(xSize, ySize)
        , bottomLeft_(bottomLeft)
        , Field(xSize * ySize)
    {
    }

    TField(const TField&) = delete;
    TField(TField&&) = default;

    TField& operator=(const TField&) = delete;
    TField& operator=(TField&&) = default;

    TMeasure GetXSize() const {
        return size_.X;
    }

    TMeasure GetYSize() const {
        return size_.Y;
    }

    TCell& At(const TPoint& pt) {
        //std::cerr << pt.X << ", " << pt.Y << '\n';
        return Field.at(GetIndexByPoint(pt));
    }

    const TCell& At(const TPoint& pt) const {
        // std::cerr << pt.X << ", " << pt.Y << '\n';
        return Field.at(GetIndexByPoint(pt));
    }

    void Insert(const TPoint& pt, EMaterial material) {
        // std::cerr << pt.X << ", " << pt.Y << '\n';
        Field.at(GetIndexByPoint(pt)).Grain = TGrain(material);
    }

    TPoint BottomLeft() const {
        return bottomLeft_;
    }

    TPoint TopRight() const {
        return bottomLeft_ + size_;
    }

private:
    TVector size_;
    TPoint bottomLeft_;
    std::vector<TCell> Field;
};


TField ScanFromText(std::istream&);
void PrintToText(std::ostream&, const TField&);

}
