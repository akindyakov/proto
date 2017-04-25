#pragma once

#include <map/lib/transaction.h>

namespace Ant {

class IAnt {
public:
    virtual bool Step() = 0;
};

class SnakeAntBody {
public:
    SnakeAntBody(
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

private:
    /**
    * /^^^^^^^^^^\  /^^^^^^^^^^\ /^^^^^^^\
    * | tail_[1] |  | tail_[0] | | head_ |
    * |     *----------->  *-------->    |
    * \__________/  \__________/ \_______/
    */
    Map::Point head_;
    std::vector<Map::Direction> tail_;
};


class FreeLoader
    : public IAnt
{
public:
    bool Step() override;

private:
    SnakeAntBody body_;
};

}
