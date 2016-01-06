#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

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

enum class EMaterial {
    Sand,
    Marble,
    Iron,
    Invalid
};

class TGrain {
public:
    TGrain(EMaterial material);
    const EMaterial Material;
};

class TCell {
public:
    bool IsOccupied() const;

    void Release();

    std::unique_ptr<TGrain> GetGrain();
    void PutGrain(std::unique_ptr<TGrain>&& grain);
    const TGrain* SeeGrain();

private:
    std::unique_ptr<TGrain> Grain;
};

template<size_t dimension>
class TField {
public:
    TCell& Get(const TPoint& pt);
    const TCell& Get(const TPoint& pt) const;

private:
    using TRow = std::vector<TCell>;
    using TMatrix = std::vector<TRow>;

    TMatrix Matrix;
};
