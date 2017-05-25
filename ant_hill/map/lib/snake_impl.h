#pragma once

namespace Map {

template<typename TField>
void SnakeObj<TField>::frontMove(
    TField& field
    , RelativeDirection frontDirection
) {
    // the last move vector is our direction now
    auto direction = frontDirection.Turn(tail_.front());
    auto pt = direction.MovePoint(head_);
    if (!field.at(pt).isFree()) {
        throw Forbidden() << "There is no free space for that move.";
    }
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

template<typename TField>
void SnakeObj<TField>::backMove(
    TField& field
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
    if (!field.at(prev).isFree()) {
        throw Forbidden() << "There is no free space for that move.";
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

template<typename TField>
typename SnakeObj<TField>::PointType
    SnakeObj<TField>::pushFrontGrain(
        RelativeDirection frontDirection
    )
{
    auto direction = frontDirection.Turn(tail_.front());
    auto pickedUpPt = direction.MovePoint(head_);
    std::swap(pickedUpPt, head_);
    tail_.push_front(direction);
    return head_;
}

template<typename TField>
typename SnakeObj<TField>::PointType
    SnakeObj<TField>::popFrontGrain(
    )
{
    if (tail_.empty()) {
        throw BadRequest() << "There is nothing to drop.";
    }
    auto droppedPt = head_;
    head_ = tail_.front().Inverse().MovePoint(head_);
    tail_.pop_front();
    return droppedPt;
}

template<typename TField>
typename SnakeObj<TField>::PointType
    SnakeObj<TField>::lookTo(
        RelativeDirection to
        , size_t segment
    ) const
{
    if (segment >= this->size()) {
        throw BadRequest() << "There is no segment with number " << segment;
    }
    auto vecIt = tail_.begin();
    auto pt = head_;
    for (size_t seg = 0; seg < segment; ++seg) {
        pt = vecIt->Inverse().MovePoint(pt);
        ++vecIt;
    }

    auto absTo = Direction::ToNowhere();
    if (vecIt == tail_.end()) {
        // the last segment situation
        absTo = tail_.back();
    } else {
        absTo = *vecIt;
    }
    auto toPt = to.Turn(absTo).MovePoint(pt);
    return toPt;
}

template<typename TField>
std::vector<RelativeDirection>
SnakeObj<TField>::getPose() const {
    auto pose_ = std::vector<RelativeDirection>{};
    auto prev = this->tail_.front();
    for (const auto& cur : tail_) {
        pose_.push_back(prev - cur);
    }
    return pose_;
}

template<typename TField>
size_t
SnakeObj<TField>::size() const {
    return tail_.size() + 1;
}

}  // namespace Map
