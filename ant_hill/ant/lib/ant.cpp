#include "ant.h"

#include <tools/tests/ut.h>

#include <algorithm>

namespace Ant {

SnakeAntBody::SnakeAntBody(
    Field::TPoint head
    , Field::TPoint tail
)
    : head_{head}
{
    tail_.push_back(
        Field::Direction::Diff(head, tail)
    );
}

std::vector<Field::ShortMovement>
SnakeAntBody::DiffHeadMove(
    Field::Direction direction
) const {
    auto diff = std::vector<Field::ShortMovement>{};
    auto nextPoint = Field::TPoint{head_};
    diff.emplace_back(nextPoint, direction);
    for (const auto& hDir : tail_) {
       nextPoint = Field::MovePoint(nextPoint, hDir.Inverse());
       diff.emplace_back(nextPoint, hDir);
    }
    if (direction == tail_.begin()->Inverse()) {
        std::reverse(diff.begin(), diff.end());
    }
    return diff;
}

void
SnakeAntBody::HeadMove(
    Field::Direction direction
) {
    head_ = Field::MovePoint(head_, direction);
    tail_.insert(tail_.begin(), direction);
    tail_.pop_back();
}

void
SnakeAntBody::AppendPoint(
    Field::Direction direction
) {
    head_ = Field::MovePoint(head_, direction);
    tail_.push_back(direction);
}

void
SnakeAntBody::DropPoint(Field::Direction direction) {
    if (Size() < 3) {
        throw AntHill::TException("Ant length must be at list 2");
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
