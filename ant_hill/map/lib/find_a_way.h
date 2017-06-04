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

template<
    typename Accepter
>
class SquaresFinder
{
public:
    SquaresFinder(
        Point areaMin
        , Point areaMax
        , Accepter accepter = Accepter{}
    );

    std::unique_ptr<Square> next();

private:
    void initNewEdge();
    bool incrementIndex();
    std::unique_ptr<Square> makeAnAnswer(
        Point pt
        , Point fillValue
    );

private:
    Point min;
    Point max;
    Accepter accepter_;

    Point index;
    std::vector<Point> prevNotCompleted;
};

template<
    typename Accepter
>
SquaresFinder<Accepter> makeSquaresFinder(
    Point areaMin
    , Point areaMax
    , Accepter accepter = Accepter{}
) {
    return SquaresFinder<Accepter>(std::move(areaMin), std::move(areaMax), std::move(accepter));
}

}  // namespace Map

#include "find_a_way_impl.h"
