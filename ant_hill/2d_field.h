#pragma once

#include <array>
#include <cstdint>

using TMeasure = std::uint32_t;

class T2dPoint {
public:
    TPoint(
        TMeasure x,
        TMeasure y,
    );


    using TDimensions = std::array<TMeasure, dimension>;
    class enum EDimensions: size_t {
        EX,
        EY,
        ECOUNT,
    };

    TMeasure X() const;
    TMeasure Y() const;

private:
    TDimensions Dimensions;
};

class enum EMaterial {
    Sand,
    Marble,
    Iron,
    Invalid
};

class TCell {
public:
    bool IsOccupied() const;

    void Occupy();
    void Release();

private:
    bool Occupied = false;
};

class TGrain: TCell {
public:

private:
    EMaterial Material;
};

template<size_t dimension>
class TField {
public:
    using TRow = std::vector<TGrain>;
    using TMatrix = std::vector<TRow>;
};
