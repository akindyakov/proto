#pragma once

#include "map_symbols.h"

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
        auto index = static_cast<size_t>(pt.X + pt.Y * XSize);
        // std::cerr << "index: " << index << std::endl;
        return index;
    }

public:
    TField(TMeasure xSize, TMeasure ySize)
        : XSize(xSize)
        , YSize(ySize)
        , Field(xSize * ySize)
    {
    }

    TField(const TField&) = delete;
    TField(TField&&) = default;

    TField& operator=(const TField&) = delete;
    TField& operator=(TField&&) = default;

    TMeasure GetXSize() const {
        return XSize;
    }

    TMeasure GetYSize() const {
        return YSize;
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

private:
    TMeasure XSize;
    TMeasure YSize;
    std::vector<TCell> Field;
};


TField ScanFromText(std::istream&);
void PrintToText(std::ostream&, const TField&);

}
