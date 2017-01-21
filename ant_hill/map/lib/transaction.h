#pragma once

#include "2d_field.h"

namespace NField {

enum class ECompass {
    North,
    West,
    South,
    East,
};

ECompass DirectionDiff(const TPoint& to, const TPoint& from);

TPoint MovePoint(TPoint pt, ECompass direction);

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
    ShortMovement(
        const TPoint& point
        , ECompass direction
    )
        : direction_(direction)
        , point_(point)
    {
    }

    /**
     * Make step to the owned direction and get new one from argument
     */
    void Next(ECompass direction);

private:
    ECompass direction_;
    TPoint point_;
};

class TMoveTransaction {
public:
    TMoveTransaction& Add(const TPoint& pt, ECompass direction);
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
