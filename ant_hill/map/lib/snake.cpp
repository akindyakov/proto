#include "snake.h"

#include <tools/tests/ut.h>

#include <algorithm>

namespace Map {

// std::vector<Map::ShortMovement>
// SnakeObj::DiffHeadMove(
//     Map::Direction direction
// ) const {
//     auto diff = std::vector<Map::ShortMovement>{};
//     auto nextPoint = Map::Point{head_};
//     diff.emplace_back(nextPoint, direction);
//     for (const auto& hDir : tail_) {
//        nextPoint = hDir.Inverse().MovePoint(nextPoint);
//        diff.emplace_back(nextPoint, hDir);
//     }
//     if (direction == tail_.begin()->Inverse()) {
//         std::reverse(diff.begin(), diff.end());
//     }
//     return diff;
// }

void
SnakeObj::frontMove(
    World::Field& field
    , RelativeDirection frontDirection
) {
    // the last move vector is our direction now
    auto direction = frontDirection.Turn(tail_.front());
    // lock here
    auto pt = direction.MovePoint(head_);
    if (!field.at(pt).isFree()) {
        throw Exception("There is no free space for that move.");
    }
    using std::swap;
    std::swap(head_, pt);
    std::swap(
        field.at(head_),
        field.at(pt)
    );
    for (const auto& dir : tail_) {
        auto prev = dir.Inverse().MovePoint(pt);
        std::swap(
            field.at(pt),
            field.at(prev)
        );
        std::swap(pt, prev);
    }
    tail_.push_front(direction);
    tail_.pop_back();
}

void SnakeObj::backMove(
    World::Field& /*field*/
    , RelativeDirection /*backDirection*/
) {
}

void SnakeObj::pushFrontGrain(
    World::Field& field
    , RelativeDirection frontDirection
) {
}

void SnakeObj::popFrontGrain(
    World::Field& field
) {
}

void SnakeObj::pushBackGrain(
    World::Field& field
    , RelativeDirection backDirection
) {
}

void SnakeObj::popBackGrain(
    World::Field& field
) {
}

void SnakeObj::look(
    World::Field& field
    , RelativeDirection
    , size_t segment
) const {
}

// void
// SnakeObj::AppendPoint(
//     Map::Direction direction
// ) {
//     head_ = direction.MovePoint(head_);
//     tail_.push_back(direction);
// }
//
// void
// SnakeObj::DropPoint(Map::Direction direction) {
//     if (Size() < 3) {
//         throw AntHill::Exception("Ant length must be at list 2");
//     }
//     tail_.erase(tail_.begin());
// }

SnakeObj SnakeObj::appear(
    World::Field& where
    , const Chain<RelativeDirection, EMaterial>& chain
    , ObjectId id
) {
    auto start = Point{0, 0};
    for (start.Y = where.min().Y; start.Y < where.max().Y; ++start.Y) {
        for (start.X = where.min().X; start.X < where.max().X; ++start.X) {
            auto startDir = Direction::North();
            while (startDir.counter() == 0) {
                bool vacant = true;
                {
                    auto pt = start;
                    for (auto& el : chain) {
                        pt = el.from.Turn(startDir).MovePoint(pt);
                        if (!where.inRange(pt) || !where.at(pt).isFree()) {
                            vacant = false;
                            break;
                        }
                    }
                }
                if (vacant) {
                    auto pt = start;
                    auto tail = std::vector<Map::Direction>{};
                    for (auto& el : chain) {
                        auto from = el.from.Turn(startDir);
                        tail.push_back(from);
                        pt = from.MovePoint(pt);
                        auto& cell = where.at(pt);
                        cell.grain = el.value;
                        cell.objectId = id;
                    }
                    return SnakeObj(pt, tail, id);
                }
                ++startDir;
            }
        }
    }
    throw Exception("There is no vacant position");
}

size_t
SnakeObj::size() const {
    return tail_.size();
}

}
