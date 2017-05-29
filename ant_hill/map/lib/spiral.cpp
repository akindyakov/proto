#include "spiral.h"

namespace Map {

Spiral::Spiral(
    const Point& center
)
    : radius_{0}
    , pt_(center)
    , goal_(center)
    , dir_(Direction::North())
{
}

Measure Spiral::radius() const {
    return this->radius_;
}

Direction Spiral::direction() const {
    return this->dir_;
}

const Point& Spiral::point() const {
    return this->pt_;
}

Point Spiral::next() {
    if (this->pt_ == this->goal_) {
        if (this->dir_ == Direction::North()) {
            this->pt_ = this->dir_.MovePoint(this->pt_);
            ++this->dir_;
            this->radius_ += 1;
            this->goal_ = this->dir_.MovePoint(this->pt_, 2 * this->radius_ - 1);
        } else {
            ++this->dir_;
            this->goal_ = this->dir_.MovePoint(this->pt_, 2 * this->radius_);
            this->pt_ = this->dir_.MovePoint(this->pt_);
        }
    } else {
        this->pt_ = this->dir_.MovePoint(this->pt_);
    }
    return this->pt_;
}

}  // namespace Map
