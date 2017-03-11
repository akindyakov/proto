#include "ant.h"

#include <tools/tests/ut.h>

#include <algorithm>

namespace NAnt {

SnakeAntBody::SnakeAntBody(
    NField::TPoint body
    , NField::TPoint head // TODO: rename head -> ...
)
    : base_{body}
{
    head_.push_back(NField::Direction::Diff(head, body));
}

std::vector<NField::ShortMovement>
SnakeAntBody::DiffHeadMove(
    NField::Direction direction
) const {
    auto diff = std::vector<NField::ShortMovement>{};
    auto nextPoint = NField::TPoint{base_};
    for (const auto& hDir : head_) {
       diff.emplace_back(nextPoint, hDir);
       nextPoint = NField::MovePoint(nextPoint, hDir);
    }
    diff.emplace_back(nextPoint, direction);
    if (direction != head_.back().Inverse()) {
        std::reverse(diff.begin(), diff.end());
    }
    return diff;
}

void
SnakeAntBody::HeadMove(NField::Direction direction) {
    auto right = head_.begin();
    auto left = right;
    ++right;
    base_ = NField::MovePoint(base_, *left);
    while (right != head_.end()) {
        *left = std::move(*right);
        ++left;
        ++right;
    }
    *left = direction;
}

void
SnakeAntBody::AppendPoint(
    NField::Direction direction
) {
    head_.push_back(direction);
}

void
SnakeAntBody::DropPoint(NField::Direction direction) {
    if (Size() < 3) {
        throw NAntHill::TException("Ant length must be at list 2");
    }
    head_.pop_back();
}

size_t
SnakeAntBody::Size() const {
    return 1 + head_.size();
}

bool FreeLoader::Step() {
    return true;
}

}
