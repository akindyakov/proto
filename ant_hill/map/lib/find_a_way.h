#pragma once

#include "2d_field.h"
#include "direction.h"

#include <algorithm>
#include <queue>


namespace Map {

struct Cell_ {
    explicit Cell_() = default;
    explicit Cell_(
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

std::ostream& operator<<(std::ostream& out, const Cell_& cell) {
    out << "[" << cell.weight << "]";
    return out;
}

template<
    typename CostAccounter
>
DirectionCurve findFreeWay(
    const Point& start
    , const Point& finish
    , const Vector& areaSize
    , const Point& areaMin
    , CostAccounter cost = CostAccounter{}
) {
    using Weights = Field<Cell_>;
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
    auto heap = std::deque<Point>{};
    heap.push_back(start);
    weights.at(start).weight = 0;
    weights.at(finish).visited = true;
    auto minWeight = weights.size().cube();

    //*dbg*/ auto count = size_t{0};
    while (!heap.empty()) {
        const auto pt = heap.front();
        heap.pop_front();
        auto& cell = weights.at(pt);
        //*dbg*/ std::cerr << "Pt: " << pt << "; weight: " << cell.weight << std::endl;

        if (!cell.visited && minWeight > cell.weight) {  // new way is not too long as before found?
            auto dir = Direction::North();
            while (dir.counter() == 0) {
                auto nPt = dir.MovePoint(pt);
                //*dbg*/ std::cerr << "dir: " << dir << "; pt: " << pt << "; nPt: " << nPt << '\n';
                if (weights.inRange(nPt)) {
                    auto& neighbor = weights.at(nPt);
                    neighbor.weight = std::min(
                        neighbor.weight,
                        cell.weight + cost(nPt)
                    );
                    //*dbg*/ std::cerr << "new weight: " << neighbor.weight << '\n';
                    if (nPt == finish) {
                        minWeight = std::min(neighbor.weight, minWeight);
                    }
                    if (!neighbor.visited && minWeight > cell.weight) {
                        heap.push_back(nPt);
                    }
                }
                ++dir;
            }
        }
        cell.visited = true;
        //*dbg*/ std::cerr << "end\n";
        //*dbg*/ std::cerr << ++count << std::endl;
    }
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

}  // namespace Map

#include "find_a_way_impl.h"
