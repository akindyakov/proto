#pragma once

#include "2d_field.h"

namespace NField {

enum class ECompass {
    North,
    West,
    South,
    East,
};

struct TMovement {
    TPoint To;
    TPoint From;

    TMovement(const TPoint& to, const TPoint& from);

    TMovement(const TMovement&) = delete;
    TMovement(TMovement&&);

    TMovement& operator=(const TMovement&) = delete;
    TMovement& operator=(TMovement&&);
};

class TTransaction {
public:
    void Move(const TPoint& pt, ECompass direction);
    bool Apply(TField& where) const;

private:
    std::vector<TMovement> Actions;
};

}  // NField
