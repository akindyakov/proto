#pragma once

#include <map/lib/transaction.h>

namespace NAnt {

class IAnt {
public:
    virtual bool Step() = 0;
};

class SnakeAntBody {
public:
    SnakeAntBody(
        NField::TPoint head
        , NField::TPoint tail
    );

    /**
    * Get diff for moving head to specified direction
    */
    std::vector<NField::ShortMovement>
    DiffHeadMove(NField::Direction direction) const;

    /**
    * Move head to specified direction
    */
    void HeadMove(NField::Direction direction);

    /**
    * Add one more point to head side
    */
    void AppendPoint(NField::Direction direction);

    /**
    * Drop first point from head side
    */
    void DropPoint(NField::Direction direction);

    size_t Size() const;

private:
    /**
    * /^^^^^^^^^^\  /^^^^^^^^^^\ /^^^^^^^\
    * | tail_[1] |  | tail_[0] | | head_ |
    * |     *----------->  *-------->    |
    * \__________/  \__________/ \_______/
    */
    NField::TPoint head_;
    std::vector<NField::Direction> tail_;
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
