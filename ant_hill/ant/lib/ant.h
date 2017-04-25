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
        Field::Point head
        , Field::Point tail
    );

    /**
    * Get diff for moving head to specified direction
    */
    std::vector<Field::ShortMovement>
    DiffHeadMove(Field::Direction direction) const;

    /**
    * Move head to specified direction
    */
    void HeadMove(Field::Direction direction);

    /**
    * Add one more point to head side
    */
    void AppendPoint(Field::Direction direction);

    /**
    * Drop first point from head side
    */
    void DropPoint(Field::Direction direction);

    size_t Size() const;

private:
    /**
    * /^^^^^^^^^^\  /^^^^^^^^^^\ /^^^^^^^\
    * | tail_[1] |  | tail_[0] | | head_ |
    * |     *----------->  *-------->    |
    * \__________/  \__________/ \_______/
    */
    Field::Point head_;
    std::vector<Field::Direction> tail_;
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
