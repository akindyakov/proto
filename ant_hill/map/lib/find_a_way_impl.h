#pragma once


namespace Map {

namespace {

struct PathfinderCell {
    explicit PathfinderCell() = default;
    explicit PathfinderCell(
        Measure weight_
        , bool visited_
    )
        : weight(weight_)
        , visited(visited_)
    {
    }

    Measure weight = std::numeric_limits<Measure>::max();
    bool visited = false;
};

// std::ostream& operator<<(std::ostream& os, const PathfinderCell& cell) {
//     os << "[" << cell.weight << "]";
//     return os;
// }

using Weights = Field<PathfinderCell>;

inline DirectionCurve findAWayByWeights(
    const Weights& weights
    , const Point& start
    , const Point& finish
) {
    auto way = DirectionCurve{};
    if (std::numeric_limits<Measure>::max() == weights.at(finish).weight) {
        return way;
    }
    auto toPt = finish;
    auto fromPt = toPt;
    auto fromWeight = weights.at(fromPt).weight;
    while (fromPt != start) {
        auto dir = Direction::North();
        while (dir.counter() == 0) {
            auto nPt = dir.MovePoint(toPt);
            if (weights.inRange(nPt)) {
                auto nWeight = weights.at(nPt).weight;
                if (nWeight < fromWeight) {
                    fromWeight = nWeight;
                    fromPt = nPt;
                }
            }
            ++dir;
        }
        way.push_back(Direction::Diff(toPt, fromPt));
        toPt = fromPt;
    }
    std::reverse(way.begin(), way.end());
    return way;
}

}

template<
    typename CostAccounter
    , typename IsItOne
>
DirectionCurve findSmthOnTheField(
    const Point& start
    , const Vector& areaSize
    , const Point& areaMin
    , CostAccounter cost
    , IsItOne check
) {
    auto weights = Weights(
        areaSize,
        areaMin
    );
    if (!weights.inRange(start)) {
        throw Exception() << "Start position is out of area range";
    }

    auto queue = std::deque<Point>{};
    queue.push_back(start);
    weights.at(start).weight = 0;

    auto finish = start;

    auto minWeight = std::numeric_limits<Measure>::max();

    while (!queue.empty()) {
        const auto pt = queue.front();
        queue.pop_front();
        auto& cell = weights.at(pt);
        if (!cell.visited) {
            cell.visited = true;
            if (check(pt)) {
                if (cell.weight < minWeight) {
                    finish = pt;
                    minWeight = cell.weight;
                }
            }
            auto dir = Direction::North();
            while (dir.counter() == 0) {
                auto nPt = dir.MovePoint(pt);
                if (weights.inRange(nPt)) {
                    auto nCost = cost(nPt);
                    if (nCost > 0) {
                        auto& neighbor = weights.at(nPt);
                        neighbor.weight = std::min(
                            neighbor.weight,
                            cell.weight + nCost
                        );
                        if (!neighbor.visited) {
                            queue.push_back(nPt);
                        }
                    }
                }
                ++dir;
            }
        }
    }
    //*dbg*/ Map::PrintToText(std::cerr, weights);
    return findAWayByWeights(
        weights,
        start,
        finish
    );
}


template<
    typename CostAccounter
>
DirectionCurve findFreeWay(
    const Point& start
    , const Point& finish
    , const Vector& areaSize
    , const Point& areaMin
    , CostAccounter cost
) {
    auto check = [&finish](const Map::Point& pt) {
        return pt == finish;
    };
    return findSmthOnTheField(
        start,
        areaSize,
        areaMin,
        std::move(cost),
        std::move(check)
    );
}

template<
    typename Accepter
>
SquaresFinder<Accepter>::SquaresFinder(
    Point areaMin
    , Point areaMax
    , Accepter accepter
)
    : min(std::move(areaMin))
    , max(std::move(areaMax))
    , accepter_(std::move(accepter))
    , index(min)
    , prevNotCompleted(max.X, max)
{
    --index.X;
}

template<
    typename Accepter
>
std::unique_ptr<Square>
    SquaresFinder<Accepter>::next()
{
    while (true) {
        if (!incrementIndex()) {
            while (index.X < this->max.X) {
                if (prevNotCompleted[index.X] != this->max) {
                    return makeAnAnswer(index, this->max);
                }
                ++index.X;
            }
            return std::unique_ptr<Square>();
        }
        if (
            index.X > this->min.X
            && prevNotCompleted[index.X - 1] == prevNotCompleted[index.X]
        ) {
            if (this->accepter_(index)) {
                if (prevNotCompleted[index.X] == this->max) {
                    // prev: ..
                    // line: .a
                    initNewEdge();
                } else {
                    // prev: aa
                    // line: aa
                    // just continue
                }
            } else {
                // prev: ??
                // line: ?.
                if (prevNotCompleted[index.X] != this->max) {
                    // prev: aa
                    // line: a.
                    auto aMin = prevNotCompleted[index.X - 1];

                    // rewind to the start of this square in that row
                    auto pt = index;
                    --pt.X;
                    while (prevNotCompleted[pt.X] == aMin) {
                        --pt.X;
                    }
                    ++pt.X;
                    auto ans = makeAnAnswer(pt, pt);
                    prevNotCompleted[index.X] = this->max;
                    return ans;
                }
            }
        } else {
            if (this->accepter_(index)) {
                if (prevNotCompleted[index.X] == this->max) {
                    // prev: b.
                    // line: ba
                    initNewEdge();
                } else {
                    // prev: ab
                    // line: ab
                    // just continue
                }
            } else {
                // prev: ab
                // line: a.
                // b is finished and we should return it
                if (prevNotCompleted[index.X] != this->max) {
                    return makeAnAnswer(index, this->max);
                }
            }
        }
    }
    return std::unique_ptr<Square>();
}

template<
    typename Accepter
>
void SquaresFinder<Accepter>::initNewEdge()
{
    auto aMin = index;
    while (
        index.X < this->max.X
        && this->accepter_(index)
        && prevNotCompleted[index.X] == this->max
    ) {
        prevNotCompleted[index.X] = aMin;
        index.X += 1;
    }
}

template<
    typename Accepter
>
bool SquaresFinder<Accepter>::incrementIndex() {
    index.X += 1;
    if (index.X >= this->max.X) {
        index.X = this->min.X;
        index.Y += 1;
    }
    if (index.Y >= this->max.Y) {
        return false;
    }
    return true;
}


template<
    typename Accepter
>
std::unique_ptr<Square>
    SquaresFinder<Accepter>::makeAnAnswer(
        Point pt
        , Point fillValue
    )
{
    /*
    *        aaaaaa...
    *  pt.Y >.........
    *        ^
    *        pt.X
    *
    * fillValue - fill the prevNotCompleted[] vector by this values
    */
    auto bMin = prevNotCompleted[pt.X];
    auto areaMaxX = this->max.X;
    while (pt.X != areaMaxX && prevNotCompleted[pt.X] == bMin) {
        prevNotCompleted[pt.X] = fillValue;
        ++pt.X;
    }
    --pt.X;  // b max x
    --pt.Y;  // b max y
    return std::make_unique<Square>(pt - bMin, bMin);
}


}  // namespace Map
