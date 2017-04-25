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

namespace Map {

using Measure = int;

class Point {
public:
    Measure X;
    Measure Y;

public:
    constexpr Point(
        Measure x,
        Measure y
    ) noexcept
        : X(x)
        , Y(y)
    {
    }

    ~Point() = default;
};

inline bool operator == (
    const Point& first
    , const Point& second
) {
    return (
        first.X == second.X
        && first.Y == second.Y
    );
}

class Vector: public Point {
public:
    constexpr Vector(
        Measure x,
        Measure y
    ) noexcept
        : Point(x, y)
    {
    }

    constexpr Vector(const Vector& other) noexcept
        : Point(other.X, other.Y)
    {
    }

    /**
    * @return length, raised to the power of 2
    */
    Measure Lenght() const {
        return X*X + Y*Y;
    }

};

bool operator!=(const Vector& first, const Vector& second);
bool operator!=(const Point& first, const Point& second);

Point& operator+=(Point& self, const Vector& shift);

Vector operator-(const Point& left, const Point& right);
Point operator+(const Point& base, const Vector& shift);

class Grain {
public:
    Grain(EMaterial material);

    Grain(Grain&& other);
    Grain(const Grain& other);
    Grain& operator=(Grain&& other);
    Grain& operator=(const Grain& other);

    EMaterial SeeMaterial() const noexcept {
        return Material;
    }

    bool IsNone() const {
        return Material == EMaterial::EmptySpace;
    }

private:

    EMaterial Material;
};

struct Cell {
    Cell()
        : Grain(EMaterial::EmptySpace)
    {
    }

    Grain Grain;
};


class Field {
private:
    size_t GetIndexByPoint(const Point& pt) const {
        auto signedIndex = (pt.X - min_.X) + (pt.Y - min_.Y) * size_.X;
        if (signedIndex < 0) {
            throw AntHill::Exception("Access by outrange point");
        }
        auto index = static_cast<size_t>(signedIndex);
        // std::cerr << "index: " << index << std::endl;
        return index;
    }

public:
    Field(Measure xSize, Measure ySize, Point minCorner=Point{0, 0})
        : size_(xSize, ySize)
        , min_(minCorner)
        , field_(xSize * ySize)
    {
    }

    Field(const Field&) = delete;
    Field(Field&&) = default;

    Field& operator=(const Field&) = delete;
    Field& operator=(Field&&) = default;

    Measure GetXSize() const {
        return size_.X;
    }

    Measure GetYSize() const {
        return size_.Y;
    }

    Cell& At(const Point& pt) {
        //std::cerr << pt.X << ", " << pt.Y << '\n';
        return field_.at(GetIndexByPoint(pt));
    }

    const Cell& At(const Point& pt) const {
        // std::cerr << pt.X << ", " << pt.Y << '\n';
        return field_.at(GetIndexByPoint(pt));
    }

    void Insert(const Point& pt, EMaterial material) {
        // std::cerr << pt.X << ", " << pt.Y << '\n';
        field_.at(GetIndexByPoint(pt)).Grain = Grain(material);
    }

    Point min() const {
        return min_;
    }

    Point max() const {
        return min_ + size_;
    }

private:
    Vector size_;
    Point min_;
    std::vector<Cell> field_;
};


Field ScanFromText(std::istream&);
void PrintToText(std::ostream&, const Field&);

}
