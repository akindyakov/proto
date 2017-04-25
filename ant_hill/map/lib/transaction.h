#pragma once

#include "2d_field.h"

namespace Field {

class Direction {
public:
    enum ECompass {
        North,
        West,
        South,
        East,
        Nowhere,
    };

public:
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

    constexpr const Direction Inverse() noexcept {
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
};

inline bool operator == (
    Direction first
    , Direction second
) {
    return first.compass_ == second.compass_;
}

inline bool operator != (
    Direction first
    , Direction second
) {
    return first.compass_ != second.compass_;
}

constexpr const Point MovePoint(Point pt, Direction direction) noexcept {
    switch (direction.compass_) {
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
        ChainNode<EMaterial> node
    );
    Point Apply(Field& where);

private:
    std::vector<ChainNode<EMaterial>> Chain;
};

}  // Field
