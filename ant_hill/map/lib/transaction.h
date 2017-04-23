#pragma once

#include "2d_field.h"

namespace NField {

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

    static Direction Diff(const TPoint& to, const TPoint& from);
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

constexpr const TPoint MovePoint(TPoint pt, Direction direction) noexcept {
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

struct TMovement {
    TPoint To;
    TPoint From;

    TMovement(const TPoint& to, const TPoint& from);

    TMovement(const TMovement&) = delete;
    TMovement(TMovement&&);

    TMovement& operator=(const TMovement&) = delete;
    TMovement& operator=(TMovement&&);
};

class ShortMovement {
public:
    explicit ShortMovement(
        const TPoint& point
        , Direction direction
    )
        : direction_(direction)
        , point_(point)
    {
    }

public:
    Direction direction_;
    TPoint point_;
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


class TMoveTransaction {
public:
    explicit TMoveTransaction() = default;
    explicit TMoveTransaction(const std::vector<ShortMovement>& movement);

    TMoveTransaction& Add(const TPoint& pt, Direction direction);
    bool Apply(TField& where) const;

private:
    std::vector<TMovement> Actions;
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

class TAppearanceTransaction {
public:
    TAppearanceTransaction& Add(
        ChainNode<EMaterial> node
    );
    TPoint Apply(TField& where);

private:
    std::vector<ChainNode<EMaterial>> Chain;
};

}  // NField
