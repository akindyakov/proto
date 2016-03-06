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
public:
    TPoint(
        TMeasure x,
        TMeasure y
    );

    TMeasure X() const;
    TMeasure Y() const;

private:
    static const size_t Dimension = 2;
    enum EDim {
        EX,
        EY,
        ECOUNT,
    };
    using TPointStorage = std::array<TMeasure, Dimension>;

private:
    TPointStorage Dimensions;
};

class TGrain {
public:
    TGrain(EMaterial material);
    const EMaterial Material;
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
};

class TField {
public:
    TCell& Get(const TPoint& pt);
    const TCell& Get(const TPoint& pt) const;

    void ScanFromText(std::istream&);
    void PrintToText(std::ostream&) const;

private:
    TMatrix Matrix;
};
