#pragma once

#include "world.h"

namespace Map {

class SnakeObj
    : public IObject
{
public:
    SnakeObj(
        Map::Point head
        , Map::Point tail
    );

    /**
    * Get diff for moving head to specified direction
    */
    std::vector<Map::ShortMovement>
    DiffHeadMove(Map::Direction direction) const;

    /**
    * Move head to specified direction
    */
    void HeadMove(Map::Direction direction);

    /**
    * Add one more point to head side
    */
    void AppendPoint(Map::Direction direction);

    /**
    * Drop first point from head side
    */
    void DropPoint(Map::Direction direction);

    size_t Size() const;

    void frontMove(
        World::Field& field
        , Map::Direction frontDirection
    ) override;

    /**
    * Move back to specified direction
    */
    void backMove(
        World::Field& field
        , Map::Direction backDirection
    ) override;

    /**
    * Add one more point to the front
    */
    void pushFrontGrain(
        World::Field& field
        , Map::Direction frontDirection
    ) override;

    void popFrontGrain(
        World::Field& field
    ) override;

    /**
    * Add one more point to the back
    */
    void pushBackGrain(
        World::Field& field
        , Map::Direction backDirection
    ) override;

    void popBackGrain(
        World::Field& field
    ) override;

    void appear(
        World::Field& field
        , std::vector<Map::RelativeDirection>
        , std::vector<Map::EMaterial>
    ) override;

    void look(
        World::Field& field
        , Map::RelativeDirection
        , size_t segment = 0
    ) const override;

private:
    /**
    *          /^^^\          /^^^\          /^^^^^^\
    * tail_[0] |   | tail_[1] |   | tail_[2] | head_|
    *   *-------> *------------> *--------------->  |
    *          \___/          \___/          \______/
    */
    Map::Point head_;
    std::deque<Map::Direction> tail_;
};

}  // namespace Map
