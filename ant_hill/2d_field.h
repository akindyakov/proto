#pragma once

#include "map_symbols.h"

#include <array>
#include <cstdint>
#include <memory>
#include <vector>
#include <istream>
#include <ostream>

using TMeasure = std::uint32_t;

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


class TGrain {
public:
    TGrain(EMaterial material);
    const EMaterial Material;
    // Weight
    // Form
    // 
};

using TOwnerId = std::uint64_t;
TOwnerId IdGenerator();

class TCell {
public:
    TCell(const TCell&) = delete;
    TCell& operator=(const TCell&) = delete;

    TCell() = default;
    explicit TCell(EMaterial material)
        : Grain(std::make_unique<TGrain>(material))
    {
    }
    TCell(TCell&& cell)
        : Grain(std::move(cell.Grain))
    {
    }
    TCell& operator=(TCell&& cell) {
        Grain = std::move(cell.Grain);
        return *this;
    }

    bool IsOccupied() const;
    void Release();

    std::unique_ptr<TGrain> GetGrain();
    void PutGrain(std::unique_ptr<TGrain>&& grain);
    void PutGrain(EMaterial material);
    const TGrain* SeeGrain() const;

private:
    std::unique_ptr<TGrain> Grain;
    // TOwnerId OwnerId;
};

<<<<<<< HEAD
class TMatrix {
public:
    TMatrix(TMeasure xSize, TMeasure ySize);

    void Resize(TMeasure xSize, TMeasure ySize);

    TMeasure GetXSize() const;
    TMeasure GetYSize() const;

    TCell& At(const TPoint& pt);
    const TCell& At(const TPoint& pt) const;

private:
    TMeasure XSize;
    std::vector<TCell> Field;
=======
class TAntField {
public:
    const TCell& See(const TPoint& pt) const;
    std::unique_ptr<TGrain> GetGrain(const TPoint& pt);
    std::unique_ptr<TGrain> PutGrain(const TPoint& pt);
    void MoveCell(
        const TPoint& pt,
        const TVector& vec
    );
>>>>>>> 0d1c9b12a474747af295a76c1349ceac17335ed6
};

class TField {
public:
    TCell& Get(const TPoint& pt);
    const TCell& Get(const TPoint& pt) const;

    std::unique_ptr<TGrain> GetGrain(const TPoint& pt);
    void PutGrain(
        const TPoint& pt,
        std::unique_ptr<TGrain> grain
    );

    void ScanFromText(std::istream&);
    void PrintToText(std::ostream&) const;

private:
    TMatrix Matrix;
};
