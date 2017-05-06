#pragma once

#include "chain.h"
#include "world.h"

#include <deque>

namespace Map {

class SnakeObj
    : public IObject
{
public:
    explicit SnakeObj(
        Point head
        , std::vector<Map::Direction> tail
        , ObjectId id
    )
        : id_(id)
        , head_(head)
        , tail_(tail.begin(), tail.end())
    {
    }

    ~SnakeObj() override = default;

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
        , const std::vector<EMaterial>& body
        , ObjectId id
    );

    const World::Cell& lookTo(
        const World::Field& field
        , RelativeDirection to
        , size_t segment
    ) const override;

    std::vector<RelativeDirection> getPose() const override;

    ObjectId id() const override {
        return id_;
    }

private:
    /**
    * /^^^^^^\            /^^^\          /^^^\           /^^^\
    * | head_| tail_front |   | tail_... |   | tail_back |   |
    * |    <---------------* <------------* <-------------*  |
    * \______/            \___/          \___/           \___/
    */
    ObjectId id_;
    Map::Point head_;
    std::deque<Map::Direction> tail_;
};

}  // namespace Map
