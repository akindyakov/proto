#pragma once

#include "transaction.h"

#include <tools/http_error.h>

#include <deque>
#include <vector>


namespace Map {

template<
    typename TField
>
class SnakeObj
{
public:
    using PointType = typename TField::PointType;
    using CellType = typename TField::CellType;

public:
    explicit SnakeObj(
        PointType head
        , std::vector<Direction> tail
    )
        : head_(head)
        , tail_(tail.begin(), tail.end())
    {
    }

    ~SnakeObj() = default;

    size_t size() const;

    void frontMove(
        TField& field
        , RelativeDirection frontDirection
    );

    /**
    * Move back to specified direction
    */
    void backMove(
        TField& field
        , RelativeDirection backDirection
    );

    /**
    * Add one more point to the front
    */
    PointType pushFrontGrain(
        RelativeDirection frontDirection
    );

    PointType popFrontGrain();

    PointType lookTo(
        RelativeDirection to
        , size_t segment
    ) const;

    std::vector<RelativeDirection> getPose() const;

private:
    /**
    * /^^^^^^\            /^^^^\          /^^^^\           /^^^^\
    * | head_| tail_front |    | tail_... |    | tail_back |    |
    * |    <---------------*  <------------*  <-------------*   |
    * \______/            \____/          \____/           \____/
    */
    PointType head_;
    std::deque<Direction> tail_;
};

#include "snake_impl.h"

}  // namespace Map
