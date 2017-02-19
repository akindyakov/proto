#include "ant.h"

#include <tools/tests/ut.h>


namespace NAnt {

SnakeAntBody::SnakeAntBody(
    NField::TPoint body
    , NField::TPoint head // TODO: rename head -> ...
)
    : base_{body}
{
    head_.push_back(DirectionDiff(body, head));
}

std::vector<NField::ShortMovement>
SnakeAntBody::DiffHeadMove(
    NField::ECompass direction
) const {
    std::vector<NField::ShortMovement> ret;
    auto nextPoint = NField::TPoint{base_};
    for (const auto& direction : head_) {
       ret.emplace_back(nextPoint, direction);
       nextPoint = NField::MovePoint(nextPoint, direction);
    }
    return ret;
}

void
SnakeAntBody::HeadMove(NField::ECompass direction) {
    auto right = head_.begin();
    auto left = right;
    ++right;
    base_ = NField::MovePoint(base_, *left);
    while (right != head_.end()) {
        *left = *right;
        ++left;
        ++right;
    }
    *left = direction;
}

void
SnakeAntBody::AppendPoint(
    NField::ECompass direction
) {
    head_.push_back(direction);
}

void
SnakeAntBody::DropPoint(NField::ECompass direction) {
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
