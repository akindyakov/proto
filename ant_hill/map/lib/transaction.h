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
    };

public:
    ECompass compass_;

public:
    Direction(
        ECompass compass
    )
        : compass_(compass)
    {
    }

    Direction Inverse() const;

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

TPoint MovePoint(TPoint pt, Direction direction);

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

class TAppearanceTransaction {
public:
    TAppearanceTransaction& Add(const TPoint& pt, EMaterial material);
    TVector Apply(TField& where);

private:
    struct TFakeCell {
        TFakeCell(
            EMaterial material
            , const TPoint& point
        )
            : Material(material)
            , Point(point)
        {
        }

        EMaterial Material;
        TPoint Point;
    };

private:
    std::vector<TFakeCell> Cells;
};

}  // NField
