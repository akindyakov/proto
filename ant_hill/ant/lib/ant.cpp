#include "ant.h"

#include <tools/tests/ut.h>

#include <algorithm>

namespace NAnt {

SnakeAntBody::SnakeAntBody(
    NField::TPoint head
    , NField::TPoint tail
)
    : head_{head}
{
    tail_.push_back(
        NField::Direction::Diff(head, tail)
    );
}

std::vector<NField::ShortMovement>
SnakeAntBody::DiffHeadMove(
    NField::Direction direction
) const {
    auto diff = std::vector<NField::ShortMovement>{};
    auto nextPoint = NField::TPoint{head_};
    diff.emplace_back(nextPoint, direction);
    for (const auto& hDir : tail_) {
       nextPoint = NField::MovePoint(nextPoint, hDir.Inverse());
       diff.emplace_back(nextPoint, hDir);
    }
    if (direction == tail_.begin()->Inverse()) {
        std::reverse(diff.begin(), diff.end());
    }
    return diff;
}

void
SnakeAntBody::HeadMove(
    NField::Direction direction
) {
    head_ = NField::MovePoint(head_, direction);
    tail_.insert(tail_.begin(), direction);
    tail_.pop_back();
}

void
SnakeAntBody::AppendPoint(
    NField::Direction direction
) {
    head_ = NField::MovePoint(head_, direction);
    tail_.push_back(direction);
}

void
SnakeAntBody::DropPoint(NField::Direction direction) {
    if (Size() < 3) {
        throw NAntHill::TException("Ant length must be at list 2");
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
