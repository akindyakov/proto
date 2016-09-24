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

using TMeasure = unsigned;

struct TPoint {
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

// TVector operator-(const TPoint& left, const TPoint& right) {
//     return TVector(
//         left.X() - right.X(),
//         left.Y() - right.Y()
//     );
// }

class TGrain {
public:
    TGrain(EMaterial material);

    TGrain(TGrain&& other);
    TGrain(const TGrain& other);
    TGrain& operator=(TGrain&& other);
    TGrain& operator=(const TGrain& other);

    EMaterial SeeMaterial() const {
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

    std::mutex Mutex;
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

private:
    TMeasure XSize;
    TMeasure YSize;
    std::vector<TCell> Field;
};


// class IField {
// public:
//     TGrainId CreateGrain(TGrain&&);
// 
//     const TGrain& GetGrain(TGrainId) const;
//     const TGrain& GetGrain(const TPoint&) const;
// 
//     bool TryMoveGrain(TGrainId, const TVector& to) const;
//     void MoveGrain(TGrainId, const TVector& to);
// };


TField ScanFromText(std::istream&);
void PrintToText(std::ostream&, const TField&);

}
