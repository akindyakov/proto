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

class Point;
class Vector;

std::ostream& operator<<(std::ostream& os, const Map::Point& pt);
std::ostream& operator<<(std::ostream& os, const Map::Vector& vect);
std::istream& operator>>(std::istream& is, Map::Point& pt);
std::istream& operator>>(std::istream& is, Map::Vector& vect);


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
    constexpr const Measure Lenght() const noexcept {
        return X*X + Y*Y;
    }

    /**
    * @return size of vector cube
    */
    constexpr const Measure cube() const noexcept {
        return X * Y;
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

template<typename TCell>
class Field {
private:
    size_t GetIndexByPoint(const Point& pt) const {
        auto signedIndex = (pt.X - min_.X) + (pt.Y - min_.Y) * size_.X;
        if (signedIndex < 0) {
            throw Exception("Access by outrange point");
        }
        auto index = static_cast<size_t>(signedIndex);
        // std::cerr << "index: " << index << std::endl;
        return index;
    }
public:
    using CellType = TCell;
    using FieldStorageType = std::vector<TCell>;

public:
    Field(Vector size, Point min=Point{0, 0})
        : size_(size)
        , min_(min)
        , field_(size_.cube())
    {
        // std::cerr << "min: " << min << std::endl;
        // std::cerr << "size: " << size << std::endl;
    }

    Field(const Field&) = delete;
    Field(Field&&) = default;

    Field& operator=(const Field&) = delete;
    Field& operator=(Field&&) = default;

    /**
    Measure GetXSize() const {
        return size_.X;
    }

    Measure GetYSize() const {
        return size_.Y;
    }
    */

    CellType& at(const Point& pt) {
        // std::cerr << pt.X << ", " << pt.Y << '\n';
        return field_.at(GetIndexByPoint(pt));
    }

    const CellType& at(const Point& pt) const {
        // std::cerr << pt.X << ", " << pt.Y << '\n';
        return field_.at(GetIndexByPoint(pt));
    }

    const Point min() const noexcept {
        return min_;
    }

    const Point max() const noexcept {
        return min_ + size_;
    }

    const Vector size() const noexcept {
        return size_;
    }

private:
    Vector size_;
    Point min_;
    FieldStorageType field_;
};

template<typename TCell>
Field<TCell> ScanFromText(std::istream&);

template<typename TCell>
void PrintToText(std::ostream&, const Field<TCell>&);

}  // namespace Map

#include "2d_field_impl.h"

using Map::operator<<;
using Map::operator>>;
