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

std::ostream& operator<<(std::ostream& os, const PathfinderCell& cell) {
    os << "[" << cell.weight << "]";
    return os;
}

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
    /*dbg*/ Map::PrintToText(std::cerr, weights);
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

}  // namespace Map
