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
    auto pt = direction.MovePoint(head_);
    // lock here
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
    World::Field& field
    , RelativeDirection backDirection
) {
    auto direction = backDirection.Turn(tail_.back());
    // get last point
    auto pt = head_;
    for (const auto& dir : tail_) {
        pt = dir.Inverse().MovePoint(pt);
    }
    auto prev = direction.MovePoint(pt);
    /*
    [head]
     ...
    [tail] <- pt
    [new place] <- prev
    */
    // lock here
    if (!field.at(prev).isFree()) {
        throw Exception("There is no free space for that move.");
    }
    std::swap(
        field.at(prev),
        field.at(pt)
    );
    std::swap(prev, pt);
    /*
    [head]
     ...
    [tail] <- prev
    [new place] <- pt
    */
    for (const auto& dir : tail_) {
        pt = dir.MovePoint(prev);
        std::swap(
            field.at(prev),
            field.at(pt)
        );
        std::swap(prev, pt);
    }
    /*
    [old head] <- prev
    [head] <- pt
     ...
    [tail]
    */
    std::swap(pt, head_);
    tail_.push_back(direction.Inverse());
    tail_.pop_front();
}

void SnakeObj::pushFrontGrain(
    World::Field& field
    , RelativeDirection frontDirection
) {
    auto direction = frontDirection.Turn(tail_.front());
    auto pt = direction.MovePoint(head_);
    // lock here
    auto& cell = field.at(pt);
    if (cell.objectId.isValid()) {
        throw Exception("This grain belongs to other ant. Theft is outlow!");
    }
    cell.objectId = id_;
    std::swap(pt, head_);
    tail_.push_front(direction);
}

void SnakeObj::popFrontGrain(
    World::Field& field
) {
    if (tail_.empty()) {
        throw Exception("There is nothing to drop.");
    }
    // lock here
    field.at(head_).objectId = ObjectId::Invalid();
    head_ = tail_.front().Inverse().MovePoint(head_);
    tail_.pop_front();
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
    , const std::vector<EMaterial>& body
    , ObjectId id
) {
    /**
    // сделай тут тупой поиск свободных мест нужного размера без всяких цепочек
    */
    auto chain = std::vector<RelativeDirection>(
        body.size() - 1,
        Map::RelativeDirection::Forward()
    );
    auto start = Point{0, 0};
    for (start.Y = where.min().Y; start.Y < where.max().Y; ++start.Y) {
        for (start.X = where.min().X; start.X < where.max().X; ++start.X) {
            auto startDir = Direction::North();
            while (startDir.counter() == 0) {
                bool vacant = true;
                if (where.inRange(start) && where.at(start).isFree()) {
                    auto pt = start;
                    for (auto& el : chain) {
                        pt = el.Turn(startDir).MovePoint(pt);
                        if (!where.inRange(pt) || !where.at(pt).isFree()) {
                            vacant = false;
                            break;
                        }
                    }
                } else {
                    vacant = false;
                }
                if (vacant) {
                    auto materialIt = body.begin();
                    auto& cell = where.at(start);
                    cell.objectId = id;
                    cell.grain = *materialIt;
                    ++materialIt;
                    auto tail = std::vector<Map::Direction>{};
                    auto pt = start;
                    for (auto& el : chain) {
                        auto from = el.Turn(startDir);
                        tail.push_back(from);
                        pt = from.MovePoint(pt);
                        auto& cell = where.at(pt);
                        cell.objectId = id;
                        cell.grain = *materialIt;
                        ++materialIt;
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
    return tail_.size() + 1;
}

}
