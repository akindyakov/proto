#pragma once

#include "2d_field.h"
#include "direction.h"

#include <algorithm>
#include <queue>


namespace Map {

template<
    typename CostAccounter
>
DirectionCurve findFreeWay(
    const Point& start
    , const Point& finish
    , const Vector& areaSize
    , const Point& areaMin
    , CostAccounter cost = CostAccounter{}
);

}  // namespace Map

#include "find_a_way_impl.h"
