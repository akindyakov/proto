#include "ant.h"

#include <tools/tests/ut.h>

#include <algorithm>

namespace Map {

SnakeObj::SnakeObj(
    Map::Point head
    , Map::Point tail
)
    : head_{head}
{
    tail_.push_back(
        Map::Direction::Diff(head, tail)
    );
}

std::vector<Map::ShortMovement>
SnakeObj::DiffHeadMove(
    Map::Direction direction
) const {
    auto diff = std::vector<Map::ShortMovement>{};
    auto nextPoint = Map::Point{head_};
    diff.emplace_back(nextPoint, direction);
    for (const auto& hDir : tail_) {
       nextPoint = hDir.Inverse().MovePoint(nextPoint);
       diff.emplace_back(nextPoint, hDir);
    }
    if (direction == tail_.begin()->Inverse()) {
        std::reverse(diff.begin(), diff.end());
    }
    return diff;
}

void
SnakeObj::frontMove(
    World::Field& field
    , RelativeDirection frontDirection
) {
    auto direction = 0;
    // lock here
    auto pt = direction.MovePoint(head_);
    if (!field.at(newHead).isFree()) {
        throw AntHill::Exception("There is no free space for that move.");
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

void
SnakeObj::backMove(
    World::Field& field
    , Map::Direction backDirection
) {
    // lock here
    auto pt = direction.MovePoint(head_);

    tail_.push_front(direction);
    tail_.pop_back();
}

void
SnakeObj::AppendPoint(
    Map::Direction direction
) {
    head_ = direction.MovePoint(head_);
    tail_.push_back(direction);
}

void
SnakeObj::DropPoint(Map::Direction direction) {
    if (Size() < 3) {
        throw AntHill::Exception("Ant length must be at list 2");
    }
    tail_.erase(tail_.begin());
}

size_t
SnakeObj::Size() const {
    return tail_.size();
}

bool FreeLoader::Step() {
    return true;
}

}
