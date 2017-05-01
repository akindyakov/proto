#pragma once

#include "world.h"

#include <deque>

namespace Map {

class SnakeObj
    : public IObject
{
public:
    SnakeObj(
        Point head
        , std::vector<Map::Direction> tail
        , ObjectId id
    )
        : id_(id)
        , head_(head)
        , tail_(tail.begin(), tail.end())
    {
    }

    size_t size() const;

    void frontMove(
        World::Field& field
        , RelativeDirection frontDirection
    ) override;

    /**
    * Move back to specified direction
    */
    void backMove(
        World::Field& field
        , RelativeDirection backDirection
    ) override;

    /**
    * Add one more point to the front
    */
    void pushFrontGrain(
        World::Field& field
        , RelativeDirection frontDirection
    ) override;

    void popFrontGrain(
        World::Field& field
    ) override;

    /**
    * Add one more point to the back
    */
    void pushBackGrain(
        World::Field& field
        , RelativeDirection backDirection
    ) override;

    void popBackGrain(
        World::Field& field
    ) override;

    static SnakeObj appear(
        World::Field& where
        , std::vector<ChainNode<EMaterial>> chain
        , ObjectId id
    );

    void look(
        World::Field& field
        , RelativeDirection
        , size_t segment = 0
    ) const override;

    ObjectId id() const override {
        return id_;
    }

private:
    /**
    *          /^^^\          /^^^\          /^^^^^^\
    * tail_[0] |   | tail_[1] |   | tail_[2] | head_|
    *   *-------> *------------> *--------------->  |
    *          \___/          \___/          \______/
    */
    ObjectId id_;
    Map::Point head_;
    std::deque<Map::Direction> tail_;
};

}  // namespace Map
