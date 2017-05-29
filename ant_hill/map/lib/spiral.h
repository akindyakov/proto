#pragma once

#include "2d_field.h"
#include "direction.h"


namespace Map {

class Spiral
{
public:
    explicit Spiral(
        const Point& center
    );

    Point next();

    Measure radius() const;
    Direction direction() const;
    const Point& point() const;

private:
    Measure radius_;
    Point pt_;
    Point goal_;
    Direction dir_;
};

}  // namespace Map
