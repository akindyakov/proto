#include "ant.h"

#include <tools/tests/ut.h>

#include <algorithm>

namespace Ant {

SnakeAntBody::SnakeAntBody(
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
SnakeAntBody::DiffHeadMove(
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
SnakeAntBody::HeadMove(
    Map::Direction direction
) {
    head_ = direction.MovePoint(head_);
    tail_.insert(tail_.begin(), direction);
    tail_.pop_back();
}

void
SnakeAntBody::AppendPoint(
    Map::Direction direction
) {
    head_ = direction.MovePoint(head_);
    tail_.push_back(direction);
}

void
SnakeAntBody::DropPoint(Map::Direction direction) {
    if (Size() < 3) {
        throw AntHill::Exception("Ant length must be at list 2");
    }
    tail_.erase(tail_.begin());
}

size_t
SnakeAntBody::Size() const {
    return 1 + tail_.size();
}

bool FreeLoader::Step() {
    return true;
}

}
