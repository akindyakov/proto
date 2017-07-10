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
    auto pt = this->tail();
    auto prev = direction.MovePoint(pt);
    /*
    [head]
     ...
    [tail] <- pt
    [new place] <- prev
    */
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
        throw Lib::HTTP::BadRequestError() << "There is nothing to drop.";
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
        throw Lib::HTTP::BadRequestError() << "There is no segment with number " << segment;
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
RelativeDirectionCurve
SnakeObj<TField>::getPose() const {
    auto curve = DirectionCurve(this->tail_.begin(), this->tail_.end());
    return Map::CurveToRelative(curve);
}

template<typename TField>
std::vector<Point>
    SnakeObj<TField>::getBody(
    ) const
{
    auto ret = std::vector<Point>{};
    auto pt = head_;
    ret.push_back(pt);
    for (const auto& t : this->tail_) {
        pt = t.Inverse().MovePoint(pt);
        ret.push_back(pt);
    }
    return ret;
}

template<typename TField>
const Point&
    SnakeObj<TField>::head() const
{
    return this->head_;
}

template<typename TField>
Point
    SnakeObj<TField>::tail() const
{
    auto point = head_;
    for (const auto& t : this->tail_) {
        point = t.Inverse().MovePoint(point);
    }
    return point;
}

template<typename TField>
Direction
    SnakeObj<TField>::forward() const
{
    return tail_.front();
}

template<typename TField>
size_t
SnakeObj<TField>::size() const {
    return tail_.size() + 1;
}

}  // namespace Map
