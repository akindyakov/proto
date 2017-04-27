#pragma once

#include "2d_field.h"

namespace Map {

namespace Axis {

enum class Relative {
    Forward,
    Left,
    Backward,
    Right,
    Nowhere,
};

}  // namespace Axis

class Direction {
public:
    enum ECompass {
        North,
        West,
        South,
        East,
        Nowhere,
    };

private:
    ECompass compass_;

public:
    constexpr Direction(
        ECompass compass
    ) noexcept
        : compass_(compass)
    {
    }

    static constexpr const Direction ToNowhere() noexcept {
        return Direction(Nowhere);
    }

    static Direction FromInt(int value) {
        if (value > Nowhere) {
            throw AntHill::Exception()
                << "Direction::FromInt() error. Wrong int value: ("
                << value << ")"
            ;
        }
        return Direction(
            static_cast<ECompass>(value)
        );
    }

    constexpr const Direction Inverse() const noexcept {
        auto to = Direction::ToNowhere();
        switch (compass_) {
            case East:
                to = West;
                break;
            case West:
                to = East;
                break;
            case North:
                to = South;
                break;
            case South:
                to = North;
                break;
            case Nowhere:
                break;
        }
        return to;
    }

    static Direction Diff(const Point& to, const Point& from);

    constexpr const Point MovePoint(Point pt) const noexcept {
        switch (compass_) {
            case Direction::North:
                pt.Y += 1;
                break;
            case Direction::West:
                pt.X -= 1;
                break;
            case Direction::South:
                pt.Y -= 1;
                break;
            case Direction::East:
                pt.X += 1;
                break;
            case Direction::Nowhere:
                // do nothing
                break;
        }
        return pt;
    }

    friend bool constexpr operator == (
        Direction first
        , Direction second
    ) noexcept;
};

inline bool constexpr operator == (
    Direction first
    , Direction second
) noexcept {
    return first.compass_ == second.compass_;
}

inline bool constexpr operator != (
    Direction first
    , Direction second
) noexcept {
    return !(first == second);
}

struct Movement {
    Point To;
    Point From;

    Movement(const Point& to, const Point& from);

    Movement(const Movement&) = delete;
    Movement(Movement&&);

    Movement& operator=(const Movement&) = delete;
    Movement& operator=(Movement&&);
};

class ShortMovement {
public:
    explicit ShortMovement(
        const Point& point
        , Direction direction
    )
        : direction_(direction)
        , point_(point)
    {
    }

public:
    Direction direction_;
    Point point_;
};

inline bool operator == (
    const ShortMovement& first
    , const ShortMovement& second
) {
    return (
        first.direction_ == second.direction_
        && first.point_ == second.point_
    );
}


class MoveTransaction {
public:
    explicit MoveTransaction() = default;
    explicit MoveTransaction(const std::vector<ShortMovement>& movement);

    MoveTransaction& Add(const Point& pt, Direction direction);
    bool Apply(Field& where) const;

private:
    std::vector<Movement> Actions;
};

template<typename Value>
class ChainNode
{
public:
    explicit constexpr ChainNode(
        Value value_
        , Direction from_ = Direction::Nowhere
    ) noexcept
        : from(from_)
        , value(value_)
    {
    }

public:
    Direction from;
    Value value;
};

class AppearanceTransaction {
public:
    AppearanceTransaction& Add(
        ChainNode<EMaterial>&& node
    ) {
        this->chain_.push_back(std::move(node));
        return *this;
    }
    Point Apply(Field& where);

private:
    std::vector<ChainNode<EMaterial>> chain_;
};

}  // namespace Map
