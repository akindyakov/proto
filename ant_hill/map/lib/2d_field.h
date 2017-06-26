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

private:
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

    static constexpr SquareIterator begin(
        Square square
    ) {
        return SquareIterator(square);
    }

    static constexpr SquareIterator end(
        Square square
    ) {
        auto it = SquareIterator(square, square.max());
        --it.pt_.Y;
        return it;
    }

    constexpr SquareIterator& operator ++ () noexcept {
        ++this->pt_.X;
        if (this->pt_.X >= this->square_.max().X) {
            this->pt_.X = this->square_.min().X;
            ++this->pt_.Y;
        }
        return *this;
    }

    constexpr SquareIterator& operator -- () noexcept {
        --this->pt_.X;
        if (this->pt_.X < this->square_.min().X) {
            this->pt_.X = this->square_.max().X - 1;
            --this->pt_.Y;
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

    constexpr const Square& square() const noexcept {
        return square_;
    }

private:
    const Square square_;
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
    explicit Field(
        Vector size
        , Point min=Point{0, 0}
    )
        : area_(
            std::move(size),
            std::move(min)
        )
        , field_(area_.size().cube())
    {
    }

    Field(const Field&) = delete;
    Field(Field&&) = default;

    Field& operator=(const Field&) = delete;
    Field& operator=(Field&&) = default;

    bool inRange(const Point& pt) const noexcept {
        return this->area_.inRange(pt);
    }

    CellType& at(const Point& pt) {
        return field_[safeIndexByPoint(pt)];
    }

    const CellType& at(const Point& pt) const {
        return field_[safeIndexByPoint(pt)];
    }

    const Point min() const noexcept {
        return this->area_.min();
    }

    const Point max() const noexcept {
        return this->area_.max();
    }

    const Vector size() const noexcept {
        return this->area_.size();
    }

    const Square& area() const noexcept {
        return this->area_;
    }

    SquareIterator begin() const noexcept {
        return SquareIterator::begin(this->area_);
    }

    SquareIterator end() const noexcept {
        return SquareIterator::end(this->area_);
    }

    /**
         ________________
        |    new size    |
        |   __________   |
        |  | old size |  |
        |  |          |  |
        |  |          |  |
        |  |__________|  |
        |________________|
    */
    void resize(
          Vector newSize
        , Vector shift = Vector{0, 0}
        , const CellType initCellValue = CellType()
    ) {
        this->field_.resize(newSize.cube(), initCellValue);
        auto oldIter = --this->end();
        this->area_ = Square(newSize, this->area_.min() + shift);
        using std::swap;
        while (oldIter.isValid()) {
            if (this->inRange(oldIter.point())) {
                auto oldIndex = static_cast<size_t>(signedIndexByPoint(oldIter.point(), oldIter.square()));
                auto newIndex = static_cast<size_t>(signedIndexByPoint(oldIter.point(), this->area_));
                swap(
                    this->field_[oldIndex],
                    this->field_[newIndex]
                );
            }
            --oldIter;
        }
    }

private:
    static constexpr inline Measure signedIndexByPoint(
        const Point& pt
        , const Square& sq
    ) noexcept {
        return (pt.X - sq.min().X) + (pt.Y - sq.min().Y) * sq.size().X;
    }

    size_t safeIndexByPoint(const Point& pt) const {
        auto signedIndex = signedIndexByPoint(pt, this->area_);
        auto index = static_cast<size_t>(signedIndex);
        if (signedIndex < 0 || index >= this->field_.size()) {
            throw Exception("Access by out range point: ")
                << pt << " not in [" << this->min() << ", " << this->max() << "]"
            ;
        }
        return index;
    }

private:
    Square area_;
    FieldStorageType field_;
};

template<typename TCell>
Field<TCell> ScanFromText(std::istream&);

template<typename TCell>
void PrintToText(std::ostream&, const Field<TCell>&);

}  // namespace Map

#include "2d_field_impl.h"

