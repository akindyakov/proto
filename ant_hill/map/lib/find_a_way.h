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

template<
    typename CostAccounter
    , typename IsItOne
>
DirectionCurve findSmthOnTheField(
    const Point& start
    , const Vector& areaSize
    , const Point& areaMin
    , CostAccounter cost = CostAccounter{}
    , IsItOne check = IsItOne{}
);

}  // namespace Map

#include "find_a_way_impl.h"
