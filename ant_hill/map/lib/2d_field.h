#pragma once

#include "map_symbols.h"

#include <lib/validate.h>

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

std::ostream& operator<<(std::ostream& os, const Point& pt);
std::ostream& operator<<(std::ostream& os, const Vector& vect);
std::istream& operator>>(std::istream& is, Point& pt);
std::istream& operator>>(std::istream& is, Vector& vect);


class Point {
public:
    Measure X;
    Measure Y;

public:
    constexpr Point(
        Measure x
        , Measure y
    ) noexcept
        : X(x)
        , Y(y)
    {
    }

    ~Point() = default;
};

inline constexpr bool operator == (
    const Point& first
    , const Point& second
) noexcept {
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

constexpr bool operator!=(const Vector& first, const Vector& second) noexcept {
    return first.X != second.X || first.Y != second.Y;
}

constexpr bool operator!=(const Point& first, const Point& second) noexcept {
    return first.X != second.X || first.Y != second.Y;
}

constexpr Point& operator+=(Point& self, const Vector& shift) noexcept {
    self.X += shift.X;
    self.Y += shift.Y;
    return self;
}

constexpr Vector operator-(const Point& left, const Point& right) noexcept {
    return Vector(
        left.X - right.X,
        left.Y - right.Y
    );
}

constexpr Point operator+(const Point& base, const Vector& shift) noexcept {
    return Point(
        base.X + shift.X,
        base.Y + shift.Y
    );
}

class Square {
public:
    constexpr explicit Square(
        Vector size
        , Point min=Point{0, 0}
    )
        : min_(std::move(min))
        , size_(std::move(size))
    {
    }

    constexpr const Point max() const noexcept {
        return this->min() + this->size();
    }

    constexpr const Point& min() const noexcept {
        return this->min_;
    }

    constexpr const Vector& size() const noexcept {
        return this->size_;
    }

    constexpr bool inRange(const Point& pt) const noexcept {
        auto m = this->max();
        return (
            pt.X < m.X
            && pt.Y < m.Y
            && pt.X >= this->min_.X
            && pt.Y >= this->min_.Y
        );
    }

public:
    Point min_;
    Vector size_;
};

class SquareIterator
{
public:
    constexpr explicit SquareIterator(
        Square square
    )
        : square_(std::move(square))
        , pt_(square_.min())
    {
    }

    constexpr explicit SquareIterator(
        Square square
        , Point start
    )
        : square_(std::move(square))
        , pt_(std::move(start))
    {
    }

    constexpr SquareIterator& operator ++ () noexcept {
        if (this->isValid()) {
            ++this->pt_.X;
            if (this->pt_.X >= this->square_.max().X) {
                this->pt_.X = this->square_.min().X;
                ++this->pt_.Y;
            }
        }
        return *this;
    }

    constexpr SquareIterator& operator -- () noexcept {
        if (this->isValid()) {
            --this->pt_.X;
            if (this->pt_.X < this->square_.min().X) {
                this->pt_.X = this->square_.max().X - 1;
                --this->pt_.Y;
            }
        }
        return *this;
    }

    constexpr bool isValid() const noexcept {
        return this->square_.inRange(this->pt_);
    }

    constexpr Point point() const noexcept {
        return pt_;
    }

    constexpr const Point& operator * () const noexcept {
        return pt_;
    }

private:
    Square square_;
    Point pt_;
};


bool operator==(const Square& first, const Square& second);
std::ostream& operator<<(std::ostream& out, const Square& square);

struct SimpleCell {
    explicit SimpleCell() = default;

    explicit SimpleCell(
        EMaterial grain_
    )
        : grain(grain_)
    {
    }

    bool isFree() const {
        return grain == EMaterial::EmptySpace;
    }

    EMaterial grain = EMaterial::EmptySpace;
};

std::ostream& operator<<(std::ostream& out, const SimpleCell& cell);
std::istream& operator>>(std::istream& in,        SimpleCell& cell);

template<
    typename TCell
>
class Field {
public:
    using CellType = TCell;
    using PointType = Point;
    using SizeType = Vector;
    using FieldStorageType = std::vector<CellType>;

public:
    explicit Field(SizeType size, PointType min=PointType{0, 0})
        : size_(size)
        , min_(min)
        , field_(size_.cube())
    {
    }

    Field(const Field&) = delete;
    Field(Field&&) = default;

    Field& operator=(const Field&) = delete;
    Field& operator=(Field&&) = default;

    bool inRange(const PointType& pt) const noexcept {
        auto m = this->max();
        return (
            pt.X < m.X
            && pt.Y < m.Y
            && pt.X >= this->min_.X
            && pt.Y >= this->min_.Y
        );
    }

    CellType& at(const PointType& pt) {
        return field_[safeIndexByPoint(pt)];
    }

    const CellType& at(const PointType& pt) const {
        return field_[safeIndexByPoint(pt)];
    }

    const PointType min() const noexcept {
        return min_;
    }

    const PointType max() const noexcept {
        return min_ + size_;
    }

    const SizeType size() const noexcept {
        return size_;
    }

private:
    Measure signedIndexByPoint(const PointType& pt) const noexcept {
        return (pt.X - min_.X) + (pt.Y - min_.Y) * size_.X;
    }

    size_t safeIndexByPoint(const PointType& pt) const {
        auto signedIndex = signedIndexByPoint(pt);
        auto index = static_cast<size_t>(signedIndex);
        if (signedIndex < 0 || index >= this->field_.size()) {
            throw Exception("Access by out range point: ")
                << pt << " not in [" << this->min() << ", " << this->max() << "]"
            ;
        }
        return index;
    }

private:
    SizeType size_;
    PointType min_;
    FieldStorageType field_;
};

template<typename TCell>
Field<TCell> ScanFromText(std::istream&);

template<typename TCell>
void PrintToText(std::ostream&, const Field<TCell>&);

}  // namespace Map

#include "2d_field_impl.h"

