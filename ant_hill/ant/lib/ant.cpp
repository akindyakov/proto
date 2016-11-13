#include "ant.h"

#include <tools/tests/ut.h>


namespace NAnt {

SnakeAntBody::SnakeAntBody(
    TPoint body
    , TPoint head
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
    auto it = head_.begin();
    ret.emplace_back(base_, it);
    auto nextPt = MovePoint(base_, it);
    while (++it != head_.end()) {
        ret.emplace_back(nextPt, it);
        nextPt = MovePoint(nextPt, it);
    }
    return ret;
}

void
SnakeAntBody::HeadMove(NField::ECompass direction) {
    base_ = MovePoint(base_, it);
    auto right = head_.begin();
    auto left = right++;
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
