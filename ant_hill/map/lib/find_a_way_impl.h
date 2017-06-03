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

using Weights = Field<PathfinderCell>;

inline DirectionCurve findAWayByWeights(
    const Weights& weights
    , const Point& start
    , const Point& finish
) {
    auto way = DirectionCurve{};
    if (weights.size().cube() < weights.at(finish).weight) {
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

template<
    typename CostAccounter
>
inline void buildWeights(
    Weights& weights
    , const Point& start
    , const Point& finish
    , CostAccounter cost
) {
    auto queue = std::deque<Point>{};
    queue.push_back(start);
    weights.at(start).weight = 0;
    weights.at(finish).visited = true;
    auto minWeight = weights.size().cube();

    while (!queue.empty()) {
        const auto pt = queue.front();
        queue.pop_front();
        auto& cell = weights.at(pt);

        if (!cell.visited && minWeight > cell.weight) {  // new way is not too long as before found?
            auto dir = Direction::North();
            while (dir.counter() == 0) {
                auto nPt = dir.MovePoint(pt);
                if (weights.inRange(nPt)) {
                    auto& neighbor = weights.at(nPt);
                    neighbor.weight = std::min(
                        neighbor.weight,
                        cell.weight + cost(nPt)
                    );
                    if (nPt == finish) {
                        minWeight = std::min(neighbor.weight, minWeight);
                    }
                    if (!neighbor.visited && minWeight > cell.weight) {
                        queue.push_back(nPt);
                    }
                }
                ++dir;
            }
        }
        cell.visited = true;
    }
}

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
    auto weights = Weights(
        areaSize,
        areaMin
    );
    if (!weights.inRange(start)) {
        throw Exception() << "Start position is out of area range";
    }
    if (!weights.inRange(finish)) {
        throw Exception() << "Finish position is out of area range";
    }
    buildWeights(
        weights,
        start,
        finish,
        std::move(cost)
    );
    return findAWayByWeights(
        weights,
        start,
        finish
    );
}

}  // namespace Map
