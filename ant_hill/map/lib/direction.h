#pragma once

#include "2d_field.h"

namespace Map {

class Direction;
class RelativeDirection;

std::ostream& operator<<(std::ostream& os, const Map::Direction& adir);
std::istream& operator>>(std::istream&, Map::Direction&);

std::ostream& operator<<(std::ostream&, const Map::RelativeDirection&);
std::istream& operator>>(std::istream&, Map::RelativeDirection&);

class Direction {
private:
    using Type = int;
    Type compass_;
    Type counter_ = 0;

    static constexpr Type north_ = 0;
    static constexpr Type west_ = 1;
    static constexpr Type south_ = 2;
    static constexpr Type east_ = 3;
    static constexpr Type all_ = 4;

    explicit constexpr Direction(
        Type compass
    ) noexcept
        : compass_(compass)
    {
    }

    friend RelativeDirection constexpr operator - (
        Direction first
        , Direction second
    ) noexcept;

public:
    Direction() = delete;

    static constexpr Direction North() noexcept {
        return Direction{north_};
    }
    static constexpr Direction West() noexcept {
        return Direction{west_};
    }
    static constexpr Direction South() noexcept {
        return Direction{south_};
    }
    static constexpr Direction East() noexcept {
        return Direction{east_};
    }

    static constexpr Direction ToNowhere() noexcept {
        return Direction{all_};
    }

    int toInt() const {
        return static_cast<int>(this->compass_);
    }

    static Direction fromInt(int value) {
        if (value >= all_) {
            throw Exception()
                << "Direction::FromInt() error. Wrong int value: ("
                << value << ")"
            ;
        }
        return Direction(value);
    }

    constexpr void normalize() noexcept {
        counter_ += compass_ / all_;
        compass_ = compass_ % all_;
        compass_ = compass_ < 0 ? all_ + compass_ : compass_;
    }

    constexpr Direction Inverse() const noexcept {
        auto to = Direction(compass_ + all_ / 2);
        to.normalize();
        return to;
    }

    static Direction Diff(const Point& to, const Point& from);

    constexpr Point MovePoint(
        Point pt
        , const Measure distance = 1
    ) const noexcept {
        switch (compass_) {
            case north_:
                pt.Y += distance;
                break;
            case west_:
                pt.X -= distance;
                break;
            case south_:
                pt.Y -= distance;
                break;
            case east_:
                pt.X += distance;
                break;
        }
        return pt;
    }

    Direction& operator ++ () noexcept {
        ++compass_;
        normalize();
        return *this;
    }

    Direction& operator -- () noexcept {
        --compass_;
        normalize();
        return *this;
    }

    constexpr Type counter() const noexcept {
        return counter_;
    }

    friend bool constexpr operator == (
        Direction first
        , Direction second
    ) noexcept;

    friend class RelativeDirection;

    friend std::ostream& operator<<(std::ostream& os, const Map::Direction& adir);
    friend std::istream& operator>>(std::istream&, Map::Direction&);
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

class RelativeDirection {
private:
    using Type = int;
    Type rdir_;

    static constexpr Type forward_ = 0;
    static constexpr Type left_ = 1;
    static constexpr Type backward_ = 2;
    static constexpr Type right_ = 3;
    static constexpr Type all_ = 4;

    friend std::ostream& operator<<(std::ostream&, const Map::RelativeDirection&);
    friend std::istream& operator>>(std::istream&, Map::RelativeDirection&);

    friend RelativeDirection constexpr operator - (
        Direction first
        , Direction second
    ) noexcept;

    friend bool constexpr operator == (
        RelativeDirection first
        , RelativeDirection second
    ) noexcept;

public:
    static constexpr RelativeDirection Forward() noexcept {
        return RelativeDirection{forward_};
    }
    static constexpr RelativeDirection Left() noexcept {
        return RelativeDirection{left_};
    }
    static constexpr RelativeDirection Backward() noexcept {
        return RelativeDirection{backward_};
    }
    static constexpr RelativeDirection Right() noexcept {
        return RelativeDirection{right_};
    }

public:
    RelativeDirection() = delete;

    explicit constexpr RelativeDirection(
        Type dir
    ) noexcept
        : rdir_(dir)
    {
        this->normalize();
    }

    constexpr RelativeDirection Inverse() const noexcept {
        auto to = RelativeDirection(this->rdir_ + all_ / 2);
        to.normalize();
        return to;
    }

    constexpr void normalize() noexcept {
        rdir_ = rdir_ % all_;
        rdir_ = rdir_ < 0 ? all_ + rdir_ : rdir_;
    }

    constexpr Direction Turn(
        Direction dir
    ) const noexcept {
        dir.compass_ += rdir_;
        dir.normalize();
        return dir;
    }

    int toInt() const {
        return static_cast<int>(this->rdir_);
    }

    static RelativeDirection fromInt(int value) {
        return RelativeDirection(static_cast<Type>(value));
    }
};

inline bool constexpr operator == (
    RelativeDirection first
    , RelativeDirection second
) noexcept {
    return first.rdir_ == second.rdir_;
}

inline bool constexpr operator != (
    RelativeDirection first
    , RelativeDirection second
) noexcept {
    return !(first == second);
}

inline RelativeDirection constexpr operator - (
    Direction first
    , Direction second
) noexcept {
    auto dir = RelativeDirection{
        static_cast<RelativeDirection::Type>(
            first.compass_ - second.compass_
        )
    };
    dir.normalize();
    return dir;
}

using DirectionCurve = std::vector<Direction>;
using RelativeDirectionCurve = std::vector<RelativeDirection>;

RelativeDirectionCurve CurveToRelative(
    const DirectionCurve& curve
    , Direction start=Direction::North()
);

DirectionCurve RelativeCurveToCurve(
    const RelativeDirectionCurve& curve
    , Direction base=Direction::North()
);

struct Movement {
    Point To;
    Point From;

    Movement(const Point& to, const Point& from);

    Movement(const Movement&) = delete;
    Movement(Movement&&) = default;

    Movement& operator=(const Movement&) = delete;
    Movement& operator=(Movement&&) = default;
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

}  // namespace Map
